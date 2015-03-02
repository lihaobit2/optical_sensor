//-----------------------------------------------------------------------------
// Si114x_debug.c
//-----------------------------------------------------------------------------
// Copyright 2013 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// File Description:  
//
// Command Line:   None
//
//-----------------------------------------------------------------------------
#include "Si114x_PGM_Toolkit.h"
#include <string.h>
#include <stdio.h>
#include "Si114x_debug.h"

s16 setup_debug(HANDLE si114x_handle)
{
    DEBUG_DESCRIPTOR dbg;
    FILE             *file;

    // Debug Channel 0 is for the UDPWaveformViewer
    memset((char *)(&dbg), 0, sizeof(DEBUG_DESCRIPTOR));
    dbg.sequence  = TRUE;
    dbg.timestamp = TRUE;
    dbg.irqstat   = TRUE;
    dbg.vis       = TRUE;
    dbg.ir        = TRUE;
    dbg.ps1       = TRUE;
    dbg.ps2       = TRUE;
    dbg.ps3       = TRUE;
    dbg.aux       = TRUE;
    dbg.gen0      = TRUE;
    dbg.gen1      = TRUE;
    dbg.inet_addr = NULL;           // Loopback
    dbg.udp_port  = 8000;           // UDP Port 
    PT_configDebugChannel( si114x_handle, 0, &dbg );

    // Debug Channel 1 is to create an CSV file that can easily
    // be opened by Excel. 
    memset((char *)(&dbg), 0, sizeof(DEBUG_DESCRIPTOR));
    dbg.sequence  = TRUE;
    dbg.timestamp = TRUE;
    dbg.irqstat   = TRUE;
    dbg.vis       = TRUE;
    dbg.ir        = TRUE;
    dbg.ps1       = TRUE;
    dbg.ps2       = TRUE;
    dbg.ps3       = TRUE;
    dbg.aux       = TRUE;
    dbg.gen0      = TRUE;
    dbg.gen1      = TRUE;
    dbg.filename  = "output.csv";   // Filename
    dbg.limit     = 100000;         // Stops after this many lines
    dbg.headername= "header.txt";
    // Uncomment the line below to turn on file logging
    // PT_configDebugChannel( si114x_handle, 1, &dbg );

    // Debug Channel to custom UDP port 
    memset((char *)(&dbg), 0, sizeof(DEBUG_DESCRIPTOR));
    dbg.sequence  = TRUE ;
    dbg.timestamp = TRUE ;
    dbg.irqstat   = TRUE ;
    dbg.vis       = TRUE ;
    dbg.ir        = TRUE ;
    dbg.ps1       = TRUE ;
    dbg.ps2       = TRUE ;
    dbg.ps3       = TRUE ;
    dbg.aux       = TRUE ;
    dbg.gen0      = TRUE ;
    dbg.gen1      = TRUE ;
    dbg.inet_addr = NULL;           // Loopback
    dbg.udp_port  = DEBUG_CH2_UDP;   // UDP Port
    PT_configDebugChannel( si114x_handle, 2, &dbg );

    return 0;
}
