// $Id$
/*
 * Encoder access functions for MIDIbox NG
 *
 * ==========================================================================
 *
 *  Copyright (C) 2012 Thorsten Klose (tk@midibox.org)
 *  Licensed for personal non-commercial use only.
 *  All other rights reserved.
 * 
 * ==========================================================================
 */

/////////////////////////////////////////////////////////////////////////////
// Include files
/////////////////////////////////////////////////////////////////////////////

#include <mios32.h>

#include "app.h"
#include "mbng_enc.h"
#include "mbng_lcd.h"
#include "mbng_patch.h"
#include "mbng_event.h"


/////////////////////////////////////////////////////////////////////////////
// local variables
/////////////////////////////////////////////////////////////////////////////

static u8 enc_group;
static u16 enc_value[MBNG_PATCH_NUM_ENC];

/////////////////////////////////////////////////////////////////////////////
// This function initializes the ENC handler
/////////////////////////////////////////////////////////////////////////////
s32 MBNG_ENC_Init(u32 mode)
{
  if( mode != 0 )
    return -1; // only mode 0 supported

  enc_group = 0;

  int i;
  for(i=0; i<MBNG_PATCH_NUM_ENC; ++i)
    enc_value[i] = 0;
  
  return 0; // no error
}


/////////////////////////////////////////////////////////////////////////////
// Get/Set the selected encoder group
/////////////////////////////////////////////////////////////////////////////
s32 MBNG_ENC_GroupGet(void)
{
  return enc_group;
}

s32 MBNG_ENC_GroupSet(u8 new_group)
{
  enc_group = new_group;
  return 0; // no error
}


/////////////////////////////////////////////////////////////////////////////
// This function should be called from APP_ENC_NotifyChange when an encoder
// has been moved
/////////////////////////////////////////////////////////////////////////////
s32 MBNG_ENC_NotifyChange(u32 encoder, s32 incrementer)
{
  if( encoder >= MBNG_PATCH_NUM_ENC )
    return -1; // invalid encoder

  if( debug_verbose_level >= DEBUG_VERBOSE_LEVEL_INFO ) {
    DEBUG_MSG("MBNG_ENC_NotifyChange(%d, %d)\n", encoder, incrementer);
  }

  // search for ENC
  int enc_ix = enc_group * mbng_patch_cfg.enc_group_size + encoder;
  mbng_event_item_t item;
  if( MBNG_EVENT_ItemSearchById(MBNG_EVENT_CONTROLLER_ENC + enc_ix + 1, &item) < 0 ) {
    if( debug_verbose_level >= DEBUG_VERBOSE_LEVEL_INFO ) {
      DEBUG_MSG("No event assigned to ENC_IX %d\n", enc_ix);
    }
    return -2; // no event assigned
  }

  if( debug_verbose_level >= DEBUG_VERBOSE_LEVEL_INFO ) {
    MBNG_EVENT_ItemPrint(&item);
  }

  int value = 0;
  switch( item.flags.ENC.enc_mode ) {
  case MBNG_EVENT_ENC_MODE_40SPEED:
    value = 0x40 + incrementer;
    if( value < 0 )
      value = 0;
    else if( value >= 0x7f )
      value = 0x7f;
    break;

  case MBNG_EVENT_ENC_MODE_00SPEED:
    value = incrementer & 0x7f;
    break;

  case MBNG_EVENT_ENC_MODE_40_1:
    value = incrementer > 0 ? 0x41 : 0x3f;
    break;

  case MBNG_EVENT_ENC_MODE_00_1:
    value = incrementer > 0 ? 0x01 : 0x7f;
    break;

  case MBNG_EVENT_ENC_MODE_INC_DEC:
    // TODO
    // dummy:
    value = incrementer > 0 ? 0x41 : 0x3f;
    break;

  default: // MBNG_EVENT_ENC_MODE_ABSOLUTE
    value = enc_value[enc_ix] + incrementer;
    if( value < item.min )
      value = item.min;
    else if( value > item.max )
      value = item.max;

    if( value == enc_value[enc_ix] )
      return 0; // no change

    enc_value[enc_ix] = value;
  }

  // send MIDI event
  MBNG_EVENT_ItemSend(&item, value);

  // forward
  MBNG_EVENT_ItemForward(&item, value);

  // print label
  MBNG_LCD_PrintItemLabel(&item, value);

  return 0; // no error
}


/////////////////////////////////////////////////////////////////////////////
// This function is called by MBNG_EVENT_ItemReceive when a matching value
// has been received
/////////////////////////////////////////////////////////////////////////////
s32 MBNG_ENC_NotifyReceivedValue(mbng_event_item_t *item, u16 value)
{
  int enc_ix = item->id & 0xfff;

  if( debug_verbose_level >= DEBUG_VERBOSE_LEVEL_INFO ) {
    DEBUG_MSG("MBNG_ENC_NotifyReceivedValue(%d, %d)\n", enc_ix, value);
  }

  if( enc_ix && enc_ix < MBNG_PATCH_NUM_ENC )
    enc_value[enc_ix-1] = value;

  // forward
  if( item->fwd_id ) {
    u16 item_id_lower = (item->id & 0xfff) - 1;
    if( item_id_lower >= enc_group*mbng_patch_cfg.enc_group_size &&
	item_id_lower < (enc_group+1)*mbng_patch_cfg.enc_group_size ) {
      MBNG_EVENT_ItemForward(item, value);
    }
  }

  return 0; // no error
}