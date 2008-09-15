// $Id$
/*
 * Checks the handling of a button/LED matrix
 *
 * ==========================================================================
 *
 *  Copyright (C) 2008 Thorsten Klose (tk@midibox.org)
 *  Licensed for personal non-commercial use only.
 *  All other rights reserved.
 * 
 * ==========================================================================
 */

/////////////////////////////////////////////////////////////////////////////
// Include files
/////////////////////////////////////////////////////////////////////////////

#include <mios32.h>

#include <stm32f10x_type.h>
#include <stm32f10x_gpio.h>

#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>

#include <blm.h>

/////////////////////////////////////////////////////////////////////////////
// Task Priorities
/////////////////////////////////////////////////////////////////////////////

#define PRIORITY_TASK_LED_TOGGLE	( tskIDLE_PRIORITY + 1 )
#define PRIORITY_TASK_DIN_CHECK		( tskIDLE_PRIORITY + 2 )


/////////////////////////////////////////////////////////////////////////////
// Prototypes
/////////////////////////////////////////////////////////////////////////////
static void TASK_LED_Toggle(void *pvParameters);
static void TASK_DIN_Check(void *pvParameters);


/////////////////////////////////////////////////////////////////////////////
// Main function
/////////////////////////////////////////////////////////////////////////////
int main( void )
{
  GPIO_InitTypeDef GPIO_InitStructure;

  // initialize hardware and MIOS32 modules
  MIOS32_SYS_Init(0);
  MIOS32_SRIO_Init(0, 16);
  MIOS32_DIN_Init(0);
  MIOS32_DOUT_Init(0);

  // initialize BLM driver
  BLM_Init(0);
  BLM_DebounceDelaySet(10);

#if defined(_STM32_PRIMER_)
  /* Configure LED pins as output push-pull. */
  GPIO_StructInit(&GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init( GPIOB, &GPIO_InitStructure );
#else
  XXX unsupported derivative XXX
#endif


  // start the tasks
  xTaskCreate(TASK_LED_Toggle, (signed portCHAR *)"LED_Toggle", configMINIMAL_STACK_SIZE, NULL, PRIORITY_TASK_LED_TOGGLE, NULL);
  xTaskCreate(TASK_DIN_Check,  (signed portCHAR *)"DIN_Check", configMINIMAL_STACK_SIZE, NULL, PRIORITY_TASK_DIN_CHECK, NULL);

  // start the scheduler
  vTaskStartScheduler();

  // Will only get here if there was not enough heap space to create the idle task
  return 0;
}


/////////////////////////////////////////////////////////////////////////////
// Application Tick Hook (called by FreeRTOS each mS)
/////////////////////////////////////////////////////////////////////////////
void vApplicationTickHook( void )
{
  // prepare DOUT registers to drive the column
  BLM_PrepareCol();

  // start next SRIO scan
  // call the BLM_GetRow function after scan is finished to capture the read DIN values
  MIOS32_SRIO_ScanStart(BLM_GetRow);
}


/////////////////////////////////////////////////////////////////////////////
// DIN Handler
/////////////////////////////////////////////////////////////////////////////

// will be called on BLM pin changes (see TASK_DIN_Check)
static void DIN_BLM_NotifyToggle(u32 pin, u32 value)
{
  // map pin and value:
  // - DOUT pins of a SR are mirrored
  // - invert DIN value (so that LED lit when button pressed)
  BLM_DOUT_PinSet(0, pin ^ 7, value ? 0 : 1); // red, pin, value
  BLM_DOUT_PinSet(1, pin ^ 7, value ? 0 : 1); // green, pin, value
}

// will be called on remaining pin changes (see TASK_DIN_Check)
static void DIN_NotifyToggle(u32 pin, u32 value)
{
  // map pin and value:
  // - DOUT pins of a SR are mirrored
  // - invert DIN value (so that LED lit when button pressed)
  MIOS32_DOUT_PinSet(pin ^ 7 , value ? 0 : 1);
}

// checks for toggled DIN pins
static void TASK_DIN_Check(void *pvParameters)
{
  portTickType xLastExecutionTime;

  // Initialise the xLastExecutionTime variable on task entry
  xLastExecutionTime = xTaskGetTickCount();

  while( 1 ) {
    vTaskDelayUntil(&xLastExecutionTime, 1 / portTICK_RATE_MS);

    // check for BLM pin changes, call DIN_BLM_NotifyToggle on each toggled pin
    BLM_ButtonHandler(DIN_BLM_NotifyToggle);

    // check for remaining pin changes, call DIN_NotifyToggle on each toggled pin
    MIOS32_DIN_Handler(DIN_NotifyToggle);
  }
}



/////////////////////////////////////////////////////////////////////////////
// LED Toggle Task (sends a sign of life)
/////////////////////////////////////////////////////////////////////////////
static void TASK_LED_Toggle(void *pvParameters)
{
  portTickType xLastExecutionTime;

  // Initialise the xLastExecutionTime variable on task entry
  xLastExecutionTime = xTaskGetTickCount();

  while( 1 ) {
    // toggle the LEDs periodically
    vTaskDelayUntil(&xLastExecutionTime, (portTickType)1000 / portTICK_RATE_MS);

#if defined(_STM32_PRIMER_)
    // toggle LEDs at pin B8/B9
    if( GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_8) ) {
      GPIO_ResetBits(GPIOB, GPIO_Pin_8);
      GPIO_SetBits(GPIOB, GPIO_Pin_9);
    } else {
      GPIO_SetBits(GPIOB, GPIO_Pin_8);
      GPIO_ResetBits(GPIOB, GPIO_Pin_9);
    }
#else
  XXX unsupported derivative XXX
#endif

  }
}