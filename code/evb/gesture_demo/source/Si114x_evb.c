#include <stdio.h>
#include <windows.h>

#ifndef NO_CONIO
#include <conio.h>
#endif

#include "Si114x_PGM_Toolkit.h"
#include "Si114x_functions.h"

HANDLE board_handle, si114x_handle, mcu_handle;

int si114x_evb_open(void)
{
    int    retval;

    // If only a single serial port is found, open it
    // otherwise, ports.txt may have the name of the serial port        
    {
        FILE   *file;
        char   serial_port_name[40];

        retval = PT_getNumSerialPorts();
        if(retval==1)
        {
           retval = PT_getSerialPortName(0,serial_port_name);
        }
        else
        {
            file = fopen("ports.txt","r");
            if ( file == NULL )
            {
                printf("\n");
                printf("    The EVB is not responding\n");
                printf("    1. If using the MFB, remove the debug cable.\n");
                printf("    2. If there is more than one EVBs, ports.txt is \n");
                printf("       necessary to specify the desired comm port.  \n");
                exit(0);
            }
            fscanf(file,"%s",serial_port_name);
            fclose(file);
        }

        board_handle = PT_openSerialPort(serial_port_name);

        if (board_handle==NULL) printf("Error opening %s \n", serial_port_name);
    }

    //
    // Retrieve the device handle for the Si114x and mcu
    //
    PT_setDebug(0x0);     // None
    mcu_handle    = PT_getDeviceHandle(board_handle, 0);
    si114x_handle = PT_getDeviceHandle(board_handle, 1);

    PT_i2c_smbus_write_byte_data(mcu_handle, 0xff, 0xff);

    return 0;
}

void si114x_main_loop(void)
{

    //
    // Process samples sent by the EVB Firmware MCU
    //
    while(1)
    {
        if(PT_si114x_irq_queue_numentries(si114x_handle)>0)
        {
            SI114X_IRQ_SAMPLE  samples;
            if (PT_si114x_irq_queue_pop(si114x_handle, &samples)==0);
            {
                ProcessSi114xSamples(si114x_handle, &samples);
            }
        }

        #ifndef NO_CONIO
        // Hit Any Key to Pause
        if (_kbhit()) 
        {
            char c = _getch();
            if (c=='q') break;
            printf("\nPress 'q' to quit\n");
            printf("Press any other key to continue\n");
            while(!_kbhit())
            {
                char c = getch();
                if(c != 'q' ) break;
                else return;
            }
        }
        #endif
    }
}

void si114x_evb_close(void) 
{
    PT_closeSerialPort(board_handle);
}

