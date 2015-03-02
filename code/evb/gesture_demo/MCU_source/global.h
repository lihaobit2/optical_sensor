/*
 * Various header includes
 */
#include "compiler_defs.h"
#include "User_defs.h"

//-----------------------------------------------------------------------------
// BDNUM indicates to the code and compiler what board this code is compiled for
// 0 - IRSLIDER2EK, 1 - SI1140DK, 2 - IRMFB-EK Top, 3 - IRMFB-EK Bottom, 
// 5 - Module EVB , 6 - UVSLIDER2EK
//
#ifndef BDNUM
#define BDNUM 1
#endif

//-----------------------------------------------------------------------------
#ifndef SLAVE_ADDR
#if BDNUM == 6 
#define SLAVE_ADDR 0x60
#else
#define SLAVE_ADDR 0x5A
#endif
#endif
#define _SA SLAVE_ADDR

//-----------------------------------------------------------------------------
#define SYSCLK          24500000        // SYSCLK frequency in Hz

#ifndef I2CSPEED
#define I2CSPEED        400000
#endif

#ifndef BAUDRATE
#define BAUDRATE        453703          // Baud rate of UART in bps
#endif

//-----------------------------------------------------------------------------
#define FWID 0x04

//-----------------------------------------------------------------------------
#if ((BDNUM == 0) || (BDNUM == 6))
#include <c8051f800_defs.h>
#elif BDNUM == 1
#include "c8051f800_defs.h"
#elif ((BDNUM == 2) || (BDNUM == 3))
#include "c8051f930_defs.h"
#elif BDNUM == 4
#include <c8051f930_defs.h>
#elif BDNUM == 5
#include <c8051f930_defs.h>
#endif

//-----------------------------------------------------------------------------
#define FALSE 0
#define TRUE  1

