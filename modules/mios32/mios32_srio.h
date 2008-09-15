// $Id$
/*
 * Header file for SRIO Driver
 *
 * ==========================================================================
 *
 *  Copyright (C) 2008 Thorsten Klose (tk@midibox.org)
 *  Licensed for personal non-commercial use only.
 *  All other rights reserved.
 * 
 * ==========================================================================
 */

#ifndef _MIOS32_SRIO_H
#define _MIOS32_SRIO_H

/////////////////////////////////////////////////////////////////////////////
// Global definitions
/////////////////////////////////////////////////////////////////////////////

// 16 should be maximum, more registers would require buffers at the SCLK/RCLK lines
// and probably also a lower scan frequency
#define MIOS32_SRIO_NUM_MAX 16


/////////////////////////////////////////////////////////////////////////////
// Global Types
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// Prototypes
/////////////////////////////////////////////////////////////////////////////

extern s32 MIOS32_SRIO_Init(u32 mode, u8 num);
extern u8 MIOS32_SRIO_NumberGet(void);

extern s32 MIOS32_SRIO_ScanStart(void *notify_hook);
extern void MIOS32_SRIO_IRQHandler(void);


/////////////////////////////////////////////////////////////////////////////
// Export global variables
/////////////////////////////////////////////////////////////////////////////

extern volatile u8 mios32_srio_dout[MIOS32_SRIO_NUM_MAX];
extern volatile u8 mios32_srio_din[MIOS32_SRIO_NUM_MAX];
extern volatile u8 mios32_srio_din_changed[MIOS32_SRIO_NUM_MAX];


#endif /* _MIOS32_SRIO_H */
