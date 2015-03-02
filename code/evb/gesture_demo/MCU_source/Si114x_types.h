#include "global.h"

#ifndef SI114X_TYPES
#define SI114X_TYPES

typedef signed char       s8;
typedef signed int        s16;
typedef signed long       s32;
typedef unsigned char     u8;
typedef unsigned int      u16;
typedef unsigned long     u32;

typedef unsigned char     HANDLE;
typedef char *            STRING;
typedef s16               PT_RESULT;
typedef s8                PT_BOOL;
typedef char BIT;

typedef struct 
{
    u16  sequence;       // sequence number
    u16  timestamp;      // 16-bit Timestamp to record
    u8   pad;
    u8   irqstat;        // 8-bit irq status
    u16  vis;            // VIS
    u16  ir;             // IR
    u16  ps1;            // PS1
    u16  ps2;            // PS2
    u16  ps3;            // PS3
    u16  aux;            // AUX
} SI114X_IRQ_SAMPLE;

typedef union uu16
{
   u16 u16;
   s16 s16;
   u8 u8[2];
   s8 s8[2];
} uu16;

typedef union uu32
{
   u32 u32;
   s32 s32;
   uu16 uu16[2];
   u16 u16[2];
   s16 s16[2];
   u8 u8[4];
   s8 s8[4];
} uu32;

#ifndef LSB
#define LSB 0
#endif


#ifndef MSB
#define MSB 1
#endif

#ifndef b0
#define b0  0
#endif


#ifndef b1
#define b1  1
#endif


#ifndef b2
#define b2  2
#endif


#ifndef b3
#define b3  3
#endif

#ifndef NULL
#define NULL    0
#endif
#endif

