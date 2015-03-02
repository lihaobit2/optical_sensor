#include "global.h"
#include "Si114x_types.h"
#include "Si114x_functions.h"
#include "smb.h"

void PortSet(u8 port, u8 portdata)
{
   switch(port)
   {
      case 0:  P0 = portdata;
               break;
      case 1:  P1 = portdata;
               break;
      case 2:  P2 = portdata;
               break;
   }
}

s16 PortRead(u8 port)
{
   switch(port)
   {
      case 0:  return P0;
      case 1:  return P1;
      case 2:  return P2;
   }

   return -1;        
}

s16 Si114xWriteToRegister(HANDLE si114x_handle, u8 address, u8 value)
{
    return i2c_smbus_write_byte_data(si114x_handle, address, value);
}

s16 Si114xReadFromRegister(HANDLE si114x_handle, u8 address)
{
    return i2c_smbus_read_byte_data(si114x_handle, address);
}

s16 Si114xBlockWrite(HANDLE si114x_handle, 
                        u8 address, u8 length, u8 *values)
{
   u8 retval, counter;

   for ( counter = 0; counter < length; counter++)
   {
      retval = Si114xWriteToRegister(si114x_handle, address+counter, values[counter]);
   }

   return retval;    
}

s16 Si114xBlockRead(HANDLE si114x_handle, 
                        u8 address, u8 length, u8 *values)
{
   return i2c_smbus_read_i2c_block_data ( si114x_handle, address, length, values );
}

// The following function will take 10 ms to complete when SYSCLK is 24.5 Mhz
void delay_10ms()
{
   u16 counter;
   for(counter = 24500; counter > 0; counter--);     
}

