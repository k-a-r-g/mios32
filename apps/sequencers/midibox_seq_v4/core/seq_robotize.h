// $Id$
/*
 * Header file for robotize routines
 *
 * ==========================================================================
 *
 *  Copyright (C) 2008 Thorsten Klose (tk@midibox.org)
 *  Licensed for personal non-commercial use only.
 *  All other rights reserved.
 * 
 * ==========================================================================
 */

#ifndef _SEQ_ROBOTIZE_H
#define _SEQ_ROBOTIZE_H

#include "seq_layer.h"

/////////////////////////////////////////////////////////////////////////////
// Global definitions
/////////////////////////////////////////////////////////////////////////////
	


/////////////////////////////////////////////////////////////////////////////
// Global Types
/////////////////////////////////////////////////////////////////////////////

typedef union {
  u8 ALL;
  struct {
    u8 SUSTAIN:1;
    u8 NOFX:1;
    u8 ECHO:1;
    u8 DUPLICATE:1;
  };
} seq_robotize_flags_t;

/////////////////////////////////////////////////////////////////////////////
// Prototypes
/////////////////////////////////////////////////////////////////////////////

extern s32 SEQ_ROBOTIZE_Init(u32 mode);

extern seq_robotize_flags_t SEQ_ROBOTIZE_Event(u8 track, u8 step, seq_layer_evnt_t *e);


/////////////////////////////////////////////////////////////////////////////
// Export global variables
/////////////////////////////////////////////////////////////////////////////


#endif /* _SEQ_ROBOTIZE_H */
