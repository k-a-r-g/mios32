// $Id$
/*
 * Header file for MIOS32
 *
 * ==========================================================================
 *
 *  Copyright (C) 2008 Thorsten Klose (tk@midibox.org)
 *  Licensed for personal non-commercial use only.
 *  All other rights reserved.
 * 
 * ==========================================================================
 */

#ifndef _MIOS32_H
#define _MIOS32_H

#ifdef __cplusplus
extern "C" {
#endif

/////////////////////////////////////////////////////////////////////////////
// include local config file
// (see MIOS32_CONFIG.txt for available switches)
/////////////////////////////////////////////////////////////////////////////

#include "mios32_config.h"


/////////////////////////////////////////////////////////////////////////////
// processor specific variable types
/////////////////////////////////////////////////////////////////////////////

#if defined(MIOS32_FAMILY_STM32F10x)
# include <stm32f10x_lib.h>
#else
   XXX derivative not supported XXX
#endif


/////////////////////////////////////////////////////////////////////////////
// include C headers
/////////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <stdio.h>


/////////////////////////////////////////////////////////////////////////////
// include mios32_*.h files of all MIOS modules
/////////////////////////////////////////////////////////////////////////////

#include <mios32_sys.h>
#include <mios32_srio.h>
#include <mios32_din.h>
#include <mios32_dout.h>
#include <mios32_enc.h>
#include <mios32_lcd.h>
#include <mios32_midi.h>
#include <mios32_usb.h>
#include <mios32_usb_desc.h>
#include <mios32_board.h>


/////////////////////////////////////////////////////////////////////////////
// Global definitions
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// Global Types
/////////////////////////////////////////////////////////////////////////////


#ifdef __cplusplus
}
#endif

#endif /* _MIOS32_H */
