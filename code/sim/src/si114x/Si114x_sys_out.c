#include <stdio.h>
#include "Si114x_types.h"
#include "Si114x_PGM_Toolkit.h"
#include "Si114x_functions.h"

extern HANDLE mcu_handle;

void PortSet(u8 port, u8 portdata)
{
	return;
}

s16 PortRead(u8 port)
{
	return 0;
}


s16 Si114xWriteToRegister(HANDLE si114x_handle, u8 address, u8 value)
{
    return 0;
  
}

s16 Si114xReadFromRegister(HANDLE si114x_handle, u8 address)
{
    return 0;
}

s16 Si114xBlockWrite(HANDLE si114x_handle, 
                        u8 address, u8 length, u8 *values)
{
    return 0;
}

s16 Si114xBlockRead(HANDLE si114x_handle, 
                        u8 address, u8 length, u8 *values)
{
    return 0;
}

void delay_10ms(void)
{
    // This is needed immediately after a reset command to the Si114x
    // In the PGM_Toolkit, there is sufficient latency, so none is added
    // here. This is a reminder that when porting code, that this must
    // be implemented.
   u16 counter,i;
   for(i = 0; i < 2; i ++)
   {
	   for(counter = 24500; counter > 0; counter--);     
   }
  
}


