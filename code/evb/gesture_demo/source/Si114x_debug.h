//-----------------------------------------------------------------------------
// Si114x_debug.h
//-----------------------------------------------------------------------------
// Copyright 2013 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// File Description:  
//
// Function Prototypes, register and bit definitions for enabling virtual
// oscilloscope and file logging via Si114x_PGM_Toolkit dll
//
// Target:         Si114x 
// Command Line:   None
//
//-----------------------------------------------------------------------------

#include <stdio.h>
#include "Si114x_PGM_Toolkit.h"

#ifndef SI114X_DEBUG_H
#define SI114X_DEBUG_H

//
// Macros
//
#ifndef DBG_CH
#define DBG_CH        2
#endif

#define REPLACE_VIS( X )    PT_replaceSample( si114x_handle, DBG_CH, 0, X ) 
#define REPLACE_IR( X )     PT_replaceSample( si114x_handle, DBG_CH, 1, X ) 
#define REPLACE_PS1( X )    PT_replaceSample( si114x_handle, DBG_CH, 2, X ) 
#define REPLACE_PS2( X )    PT_replaceSample( si114x_handle, DBG_CH, 3, X ) 
#define REPLACE_PS3( X )    PT_replaceSample( si114x_handle, DBG_CH, 4, X ) 
#define REPLACE_AUX( X )    PT_replaceSample( si114x_handle, DBG_CH, 5, X ) 
#define REPLACE_GEN0( X )   PT_replaceSample( si114x_handle, DBG_CH, 6, X ) 
#define REPLACE_GEN1( X )   PT_replaceSample( si114x_handle, DBG_CH, 7, X ) 

#define REPLACE_0_VIS( X )  PT_replaceSample( si114x_handle,      0, 0, X ) 
#define REPLACE_0_IR( X )   PT_replaceSample( si114x_handle,      0, 1, X ) 
#define REPLACE_0_PS1( X )  PT_replaceSample( si114x_handle,      0, 2, X ) 
#define REPLACE_0_PS2( X )  PT_replaceSample( si114x_handle,      0, 3, X ) 
#define REPLACE_0_PS3( X )  PT_replaceSample( si114x_handle,      0, 4, X ) 
#define REPLACE_0_AUX( X )  PT_replaceSample( si114x_handle,      0, 5, X ) 
#define REPLACE_0_GEN0( X ) PT_replaceSample( si114x_handle,      0, 6, X ) 
#define REPLACE_0_GEN1( X ) PT_replaceSample( si114x_handle,      0, 7, X ) 

#define REPLACE_1_VIS( X )  PT_replaceSample( si114x_handle,      1, 0, X ) 
#define REPLACE_1_IR( X )   PT_replaceSample( si114x_handle,      1, 1, X ) 
#define REPLACE_1_PS1( X )  PT_replaceSample( si114x_handle,      1, 2, X ) 
#define REPLACE_1_PS2( X )  PT_replaceSample( si114x_handle,      1, 3, X ) 
#define REPLACE_1_PS3( X )  PT_replaceSample( si114x_handle,      1, 4, X ) 
#define REPLACE_1_AUX( X )  PT_replaceSample( si114x_handle,      1, 5, X ) 
#define REPLACE_1_GEN0( X ) PT_replaceSample( si114x_handle,      1, 6, X ) 
#define REPLACE_1_GEN1( X ) PT_replaceSample( si114x_handle,      1, 7, X ) 

#define REPLACE_2_VIS( X )  PT_replaceSample( si114x_handle,      2, 0, X ) 
#define REPLACE_2_IR( X )   PT_replaceSample( si114x_handle,      2, 1, X ) 
#define REPLACE_2_PS1( X )  PT_replaceSample( si114x_handle,      2, 2, X ) 
#define REPLACE_2_PS2( X )  PT_replaceSample( si114x_handle,      2, 3, X ) 
#define REPLACE_2_PS3( X )  PT_replaceSample( si114x_handle,      2, 4, X ) 
#define REPLACE_2_AUX( X )  PT_replaceSample( si114x_handle,      2, 5, X ) 
#define REPLACE_2_GEN0( X ) PT_replaceSample( si114x_handle,      2, 6, X ) 
#define REPLACE_2_GEN1( X ) PT_replaceSample( si114x_handle,      2, 7, X ) 

#ifndef DEBUG_CH2_UDP
#define DEBUG_CH2_UDP 8100
#endif

//
// How to create a printf substitute
// #define myprintf   (printf)

//
// Use this REMOVE_PRINTF switch to "define away" printf() from the source
//
#ifdef REMOVE_PRINTF
#define printf(...) do {} while(0)
#endif

//
// Function Prototypes
//
s16 setup_debug(HANDLE si114x_handle) ;

#ifndef PGM_TOOLKIT_TYPES
#define PGM_TOOLKIT_TYPES
typedef signed char       s8;
typedef signed short      s16;
typedef signed int        s32;
typedef unsigned char     u8;
typedef unsigned short    u16;
typedef unsigned int      u32;

typedef void *            HANDLE;
typedef char *            STRING; 
typedef s16               PT_RESULT;
typedef s8                PT_BOOL;
#endif

#ifndef TRUE
#define TRUE    0xff
#endif
#ifndef FALSE
#define FALSE   0
#endif
#ifndef NULL
#define NULL    0
#endif


#endif
