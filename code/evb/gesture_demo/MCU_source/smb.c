#include "global.h"

// For BDNUM less than or equal to 6, we typically assume that the virtual address
// is synonymous with i2c bus number. So, there would be a total of 8 si114x devices, 
// each in their own bus. The i2c address is always whatever is passed by the 
// SLAVE_ADDR in global.h. This is for  'backward compatibility' for the older boards.
// However, the reality is that the i2c_bus is completely configurable from the 
// PC host. The host can program up to eight devices, each of which can reside in
// any bus, and can have any i2c address specified below. The items below
// just says what the default values are.
//                    0     1     2     3     4     5     6     7
U8 xdata i2c_addr[8] = {  _SA,  _SA,  _SA,  _SA,  _SA,  _SA,  _SA,  _SA };
U8 xdata i2c_bus[8]  = {    0,    1,    2,    3,    4,    5,    6,    7 };

static idata U8 current_i2c_addr = _SA; 

#if BDNUM == 5
SBIT (SDA,  SFR_P0, 1);
SBIT (SCL0, SFR_P0, 2);
SBIT (SCL1, SFR_P0, 3);
SBIT (SCL2, SFR_P0, 7);
SBIT (SCL3, SFR_P0, 2);
SBIT (SCL4, SFR_P0, 2);
SBIT (SCL5, SFR_P0, 2);
SBIT (SCL6, SFR_P0, 2);
SBIT (SCL7, SFR_P0, 2);
#elif BDNUM == 4
SBIT (SDA,  SFR_P0, 1);
SBIT (SCL0, SFR_P0, 3);
SBIT (SCL1, SFR_P0, 2);
SBIT (SCL2, SFR_P0, 2);
SBIT (SCL3, SFR_P0, 2);
SBIT (SCL4, SFR_P0, 2);
SBIT (SCL5, SFR_P0, 2);
SBIT (SCL6, SFR_P0, 2);
SBIT (SCL7, SFR_P0, 2);

#elif ((BDNUM == 2) || (BDNUM == 3))
// Define lines used for SMBus communication with the 114x
SBIT (SDA, SFR_P0, 2);
//SBIT (SCL, SFR_P0, 3);
SBIT (SCL0, SFR_P0, 3);       
SBIT (SCL1, SFR_P1, 1);
SBIT (SCL2, SFR_P1, 2);       
SBIT (SCL3, SFR_P1, 3);
SBIT (SCL4, SFR_P1, 4);       
SBIT (SCL5, SFR_P1, 5);
SBIT (SCL6, SFR_P1, 6);       
SBIT (SCL7, SFR_P1, 7);
#elif ((BDNUM == 0) || (BDNUM == 6))
// Define lines used for SMBus communication with the 114x
SBIT (SDA, SFR_P0, 2);
//SBIT (SCL, SFR_P0, 3);
SBIT (SCL0, SFR_P0, 3);       
SBIT (SCL1, SFR_P1, 1);
SBIT (SCL2, SFR_P1, 2);       
SBIT (SCL3, SFR_P1, 3);
SBIT (SCL4, SFR_P1, 4);       
SBIT (SCL5, SFR_P1, 5);
SBIT (SCL6, SFR_P1, 6);       
SBIT (SCL7, SFR_P1, 7);
#elif BDNUM == 1
// Define lines used for SMBus communication with the 114x
SBIT (SDA, SFR_P0, 6);
SBIT (SCL0, SFR_P0, 7);       
SBIT (SCL1, SFR_P1, 1);
SBIT (SCL2, SFR_P1, 2);       
SBIT (SCL3, SFR_P1, 3);
SBIT (SCL4, SFR_P1, 4);       
SBIT (SCL5, SFR_P1, 5);
SBIT (SCL6, SFR_P1, 6);       
SBIT (SCL7, SFR_P1, 7);
#endif


static volatile bit smbBusy;    // Software flag to indicate when the
                         // SMB_Read() or SMB_Write() functions
                         // have claimed the SMBus

static volatile bit smbRW;      // Software flag to indicate the
                         // direction of the current transfer
                         // 0 = Write
                         // 1 = Read

static volatile bit SMBUS_FAIL = 0;                       // Used by the ISR to flag failed transfers

static volatile bit SLAVENACK = 0;  

static U8 idata    numberOfBytesToWrite; 
static U8 idata    numberOfBytesToRead;

static U8 idata    *SMB_DATA_IN; 
static U8 idata    *SMB_DATA_OUT;


//-----------------------------------------------------------------------------
// StuckSDA
//-----------------------------------------------------------------------------
//
// Makes sure no Si114x device is holding SDA low
//
// Parameters   : none
//
//-----------------------------------------------------------------------------
void StuckSDA()
{
   U8 idata i;
   U8 idata XBR0_SAVE;

   // If slave is holding SDA low because of an improper SMBus reset or error
   while(!SDA)
   {
      // Provide clock pulses to allow the slave to advance out
      // of its current state. This will allow it to release SDA.
      XBR0_SAVE = XBR0;                // Store XBR0 so we can disable SMBus in order to
                                       //  take control of the SCL line
      XBR0 = 0;
      SCL0 = 0;                        // Drive the clock low
      SCL1 = 0;                        // Drive the clock low
      SCL2 = 0;                        // Drive the clock low
      SCL3 = 0;                        // Drive the clock low
      SCL4 = 0;                        // Drive the clock low
      SCL5 = 0;                        // Drive the clock low
      SCL6 = 0;                        // Drive the clock low
      SCL7 = 0;                        // Drive the clock low
 
      for(i = 0; i < 255; i++);        // Hold the clock low
      SCL0 = 1;                        // Release the clock
      SCL1 = 1;                        // Release the clock
      SCL2 = 1;                        // Release the clock
      SCL3 = 1;                        // Release the clock
      SCL4 = 1;                        // Release the clock
      SCL5 = 1;                        // Release the clock
      SCL6 = 1;                        // Release the clock
      SCL7 = 1;                        // Release the clock
      for(i = 0; i < 10; i++);         // Hold the clock high
      XBR0 = XBR0_SAVE;                // Restore XBR0
   }   
}

#if ((BDNUM == 0) || (BDNUM == 6))
static U8 code P0_SKIPS[8] = {0x03, 0xCB, 0xCB, 0xCB, 0xCB, 0xCB, 0xCB, 0xCB};
static U8 code P1_SKIPS[8] = {0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F};
static U8 code P2_SKIPS[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
#elif BDNUM == 1
static U8 code P0_SKIPS[8] = {0x0F, 0x8F, 0x8F, 0x8F, 0x8F, 0x8F, 0x8F, 0x8F};
static U8 code P1_SKIPS[8] = {0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F};
static U8 code P2_SKIPS[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
#elif ((BDNUM == 2) || (BDNUM == 3))
static U8 code P0_SKIPS[8] = {0x03, 0xCB, 0xCB, 0xCB, 0xCB, 0xCB, 0xCB, 0xCB};
static U8 code P1_SKIPS[8] = {0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F};
static U8 code P2_SKIPS[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
#elif (BDNUM == 4)
static U8 code P0_SKIPS[8] = {0x05, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01};
static U8 code P1_SKIPS[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static U8 code P2_SKIPS[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
#elif (BDNUM == 5)
//U8 code P0_SKIPS[8] = {0x01, 0x05, 0xD4, 0x01, 0x01, 0x01, 0x01, 0x01};
static U8 code P0_SKIPS[8] = {0x01, 0x05, 0x4D, 0x01, 0x01, 0x01, 0x01, 0x01};
static U8 code P1_SKIPS[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static U8 code P2_SKIPS[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
#endif


// To expand the PT_Bridge to support devices other than the Si114x, it is best
// to use the virtual address to tell us which i2c bus the device is connected
// to, as well as the device's slave address. We can simply use two byte
// arrays with a common index value
//
//

void Set_SMB_Pins(U8 virtual_addr)
{
   static U8 idata last_addr = 0xFF;
   U8 idata XBR0_SAVE;
   U8 idata index;

   // Do nothing if the virtual address is
   //  for the MCU
   if((virtual_addr&0xFE) == 0) return;


   if(last_addr != (virtual_addr&0xFE))
   {
      
      index = ((virtual_addr&0xFE) - 0xB4)>>1;

      current_i2c_addr = i2c_addr[index];
      
      StuckSDA();
       
      XBR0_SAVE = XBR0;                // Store XBR0 so we can disable SMBus in order to
                                        //  take control of the SCL line
      XBR0 &= ~0x04;     
      
      P0SKIP = P0_SKIPS[ i2c_bus[index] ];      // Change SKIP registers
      P1SKIP = P1_SKIPS[ i2c_bus[index] ];
      
      #if ((BDNUM == 2) || (BDNUM == 3))
      P2SKIP = P2_SKIPS[ i2c_bus[index] ];
      #endif
      
      XBR0 = XBR0_SAVE;                // Restore XBR0
      
      StuckSDA();
      
      last_addr = virtual_addr&0xFE;
   }

}

void SMB_Write (void)
{
   smbBusy = 1;                       // Claim SMBus (set to busy)
   smbRW   = 0;                       // Mark this transfer as a WRITE
   STA     = 1;                       // Start transfer
   while (smbBusy);                   // Wait for the transfer to complete
}

void SMB_Read (void)
{
   smbBusy = 1;                       // Claim SMBus (set to busy)
   smbRW   = 1;                       // Mark this transfer as a READ
   STA     = 1;                       // Start transfer
   while (smbBusy);                   // Wait for transfer to complete
}

//-----------------------------------------------------------------------------
// SMBus_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : i2c_speed
//
// SMBus configured as follows:
// - SMBus enabled
// - Slave mode inhibited
// - Timer2 used as clock source. The maximum SCL frequency will be
//   approximately 1/3 the Timer2 overflow rate
// - Setup and hold time extensions enabled
// - Bus Free and SCL Low timeout detection enabled
// - i2c_speed is desired speed divided by 10000
//   for example, to get 400000, pass 40
//
//-----------------------------------------------------------------------------


void SMBus_Init (U8 i2c_speed) 
{

   U8 idata i;
   U8 idata XBR0_SAVE;

   SMB0CF &= ~0x80;  // Disable SMBus
   EIE1   &= ~0x01;  // Disable SMBus interrupt
   TMR2CN  = 0x00;   // Turn off Timer2


   EIP1   != 0x01;    // SMB is High Priority Interrupt

   // Initialize Timer 2 first
   CKCON |= 0x30;                      // Timer2 uses system clock as source

   //TMR2RLL = -(SYSCLK / (I2CSPEED*3)); // Set Timer2 Auto reload to be 3x desired I2C speed
   TMR2RLL = (U8)(-((SYSCLK/1000)/((S16)i2c_speed*30)));

   TMR2L = TMR2RLL;
   
   TMR2CN  = 0x08;                     // Enable Timer2 in split auto-reload mode  

   SMB0CF = 0x5F;                      // Use Timer2L overflows as SMBus clock
                                       // source;
                                       // Disable slave mode;
                                       // Enable setup & hold time
                                       // extensions;
                                       // Enable SMBus Free timeout detect;
                                       // Enable SCL low timeout detect;

   SMB0CF |= 0x80;                     // Enable SMBus;
   EIE1   |= 0x01;                     // Enable SMBus interrupt

    // If slave is holding SDA low because of an improper SMBus reset or error
   while(!SDA)
   {
      // Provide clock pulses to allow the slave to advance out
      // of its current state. This will allow it to release SDA.
      XBR0_SAVE = XBR0;                // Store XBR0 so we can disable SMBus in order to
                                       //  take control of the SCL line
      XBR0 = 0;
      SCL0 = 0;                         // Drive the clock low
      for(i = 0; i < 255; i++);        // Hold the clock low
      SCL0 = 1;                         // Release the clock
      while(!SCL0);                     // Wait for open-drain
                                       // clock output to rise
      for(i = 0; i < 10; i++);         // Hold the clock high
      XBR0 = XBR0_SAVE;                // Restore XBR0
   }

}

INTERRUPT(SMBUS0_ISR, INTERRUPT_SMBUS0)
{
   static U8 idata sent_byte_counter;
   static U8 idata rec_byte_counter;

   if (ARBLOST == 1) SMBUS_FAIL = 1;
   else
   {
      // Normal operation
      switch (SMB0CN & 0xF0)           // Status vector
      {

         #define  SMB_MTSTA      0xE0   // (MT) start transmitted
         #define  SMB_MTDB       0xC0   // (MT) data byte transmitted
         #define  SMB_MRDB       0x80   // (MR) data byte received

         // Master Transmitter/Receiver: START condition transmitted.

         case SMB_MTSTA:
            SMB0DAT = current_i2c_addr<<1;   // Load address of the target slave   
            SMB0DAT &= 0xFE;           // Clear the LSB of the address for the
                                       // R/W bit
            SMB0DAT |= smbRW;          // Load R/W bit
            STA = 0;                   // Manually clear START bit
            rec_byte_counter  = 0;     // Reset the counter
            sent_byte_counter = 0;     // Reset the counter
            break;

         // Master Transmitter: Data byte transmitted
         case SMB_MTDB:
            if (ACK)                   // Slave ACK?
            {
               SLAVENACK = 0;          // Slave ACK'd, clear NACK flag
               if (smbRW == 0x00)      // If this transfer is a WRITE,
               {
                  if (sent_byte_counter < numberOfBytesToWrite)
                  {
                     // send data byte
                     SMB0DAT = SMB_DATA_OUT[sent_byte_counter];
                     sent_byte_counter++;
                  }
                  else
                  {
                     STO = 1;          // Set STO to terminate transfer
                     smbBusy = 0;      // And free SMBus interface
                  }
               }
               else {}                 // If this transfer is a READ,
                                       // proceed with transfer without
                                       // writing to SMB0DAT (switch
                                       // to receive mode)
            }
            else                       // If slave NACK,
            {
               STO = 1;                // Send STOP condition
               smbBusy = 0;            // Clear busy indicator
               SLAVENACK = 1;
            }
            break;

         // Master Receiver: byte received
         case SMB_MRDB:
            SMB_DATA_IN[rec_byte_counter] = SMB0DAT; // Store data
            rec_byte_counter++;        // Increment the byte counter
            if (rec_byte_counter < numberOfBytesToRead)
            {        
               ACK = 1;                // Send ACK to indicate byte received
            }
            else
            {   
               smbBusy = 0;            // Free SMBus interface
               ACK = 0;                // Send NACK to indicate last byte
                                       // of this transfer

               STO = 1;                // Send STOP to terminate transfer
            }
            break;

         default:
            SMBUS_FAIL = 1;            // Indicate failed transfer
                                       // and handle at end of ISR
            break;

      } // end switch
   }


   if (SMBUS_FAIL)                     // If the transfer failed,
   {
      SMB0CF &= ~0x80;                 // Reset communication
      SMB0CF |= 0x80;
      STA = 0;
      STO = 0;
      ACK = 0;
      smbBusy = 0;                     // Free SMBus
   }

   SI = 0;                             // Clear interrupt flag
}


//-----------------------------------------------------------------------------
// I2C Interface is modeled after like Linux-style i2c host interface. However,instead
// of signed 32-bit return results, signed 16-bit would be used instead. The example
// Si114x code can be written so that there is little change. Also, the i2c_clieant point
// will not be used. In Linux, it is a rather complicated structure. 
//
// Here is the Linux-style host interface:
// s32 i2c_smbus_read_byte_data(struct i2c_client *client, u8 command);
// Return:    Negative number if fail
//            byte value if successful
//
// s32 i2c_smbus_write_byte_data(struct i2c_client *client, u8 command, u8 value); 
// Return:    Negative number if fail
//            0 if successful
//
// s32 i2c_smbus_read_i2c_block_data(struct i2c_client *client, u8 command, u8 length, u8 *values)
//
// Return:    Negative number if fail
//            Number of bytes read if successful
//-----------------------------------------------------------------------------
//

static U8 idata read_buffer[1];
static U8 idata write_buffer[2];

S16 check_error(void)
{
    if( SLAVENACK )       return -1;
    else if( SMBUS_FAIL ) return -2;
    else return 0;
}

S16 check_error_return_byte(void)
{
    if( SLAVENACK )       return -1;
    else if( SMBUS_FAIL ) return -2;
    else return ( *SMB_DATA_IN & 0xff );
}

S16 i2c_smbus_read_byte_data (U8 i2c_handle, U8 reg_addr )
{
 
   Set_SMB_Pins( i2c_handle );
 
   SMB_DATA_IN  = read_buffer; 
   SMB_DATA_OUT = write_buffer;

   SMBUS_FAIL = 0;
   SLAVENACK  = 0;

   numberOfBytesToWrite = 1;
   *SMB_DATA_OUT = reg_addr;

   numberOfBytesToRead  = 1;

   SMB_Write(); 
   SMB_Read();     

   return check_error_return_byte() ;
}

S16 i2c_smbus_write_byte_data(U8 i2c_handle, U8 reg_addr, U8 reg_data )
{
   Set_SMB_Pins( i2c_handle );

   SMB_DATA_OUT = write_buffer;

   SMBUS_FAIL = 0;
   SLAVENACK  = 0;

   numberOfBytesToWrite = 2;
   SMB_DATA_OUT[0] = reg_addr;   
   SMB_DATA_OUT[1] = reg_data;

   SMB_Write();

   return check_error();  
}

S16 i2c_smbus_read_byte( U8 i2c_handle )
{
   Set_SMB_Pins( i2c_handle );

   SMB_DATA_IN = read_buffer;

   SMBUS_FAIL = 0;
   SLAVENACK  = 0;

   numberOfBytesToRead  = 1;

   SMB_Read();     

   return check_error_return_byte();
}

S16 i2c_smbus_write_byte( U8 i2c_handle, U8 value )
{
   Set_SMB_Pins( i2c_handle );

   SMB_DATA_OUT = write_buffer;

   SMBUS_FAIL = 0;
   SLAVENACK  = 0;

   numberOfBytesToWrite = 1;
   *SMB_DATA_OUT = value;   

   SMB_Write();

   return check_error();  
}

S16 i2c_smbus_read_i2c_block_idata ( U8 i2c_handle, U8 reg_addr, U8 length, U8 idata *values )
{
   Set_SMB_Pins( i2c_handle );

   SMB_DATA_OUT = write_buffer;
   SMB_DATA_IN  = (U8 idata *)values;

   SMBUS_FAIL = 0;
   SLAVENACK  = 0;

   numberOfBytesToWrite =        1;
   *SMB_DATA_OUT        = reg_addr;

   numberOfBytesToRead  =   length;  

   SMB_Write(); 
   SMB_Read(); 

   return check_error();  
}

//
// This is needed due to generic pointer vs memory-specific pointers in Keil Compiler
//
S16 i2c_smbus_read_i2c_block_data ( U8 i2c_handle, U8 reg_addr, U8 length, U8 *values )
{
    U8 idata ivalue[16], i;
    i2c_smbus_read_i2c_block_idata ( i2c_handle, reg_addr, length, ivalue );
    for( i = 0; i < length; i++ )
    {
        values[i] = ivalue[i];
    }
    return 0;
}

S16 i2c_smbus_read_block_idata  ( U8 i2c_handle, U8 length, U8 idata *values )
{
   Set_SMB_Pins( i2c_handle );

   SMB_DATA_IN  = (U8 idata *)values;

   SMBUS_FAIL = 0;
   SLAVENACK  = 0;

   numberOfBytesToRead  =   length;  

   SMB_Read(); 

   return check_error();  
}


S16 i2c_smbus_write_block_idata ( U8 i2c_handle, U8 length, U8 idata *values )
{
   Set_SMB_Pins( i2c_handle );

   SMB_DATA_OUT = (U8 idata *)values;

   SMBUS_FAIL = 0;
   SLAVENACK  = 0;

   numberOfBytesToWrite = length;

   SMB_Write();

   return check_error();  
}

S16 i2c_smbus_optimized( U8 i2c_handle, U8 idata *values )
{
    U8 irqstatus;

    Set_SMB_Pins( i2c_handle );

    SMB_DATA_OUT = write_buffer;
    SMB_DATA_IN  = values;

    numberOfBytesToWrite =        1;
    numberOfBytesToRead  =        1;  
    SMB_DATA_OUT[0]     =      0x21;
    SMB_Write(); 
    SMB_Read(); 

    irqstatus = SMB_DATA_IN[0];
    
    numberOfBytesToWrite =    2;
    SMB_DATA_OUT[0]      = 0x21;   
    SMB_DATA_OUT[1]      = 0xff;
    SMB_Write();

    numberOfBytesToWrite =    1;

    // If ALS interrupt, might as well grab everything
    if ( irqstatus & 0x03 )
    {
        SMB_DATA_OUT[0]      =      0x21;
        numberOfBytesToRead  =        13;  
        SMB_DATA_IN          =    values;
        SMB_Write(); 
        SMB_Read(); 
    }
    else
    {
        // Optimize for Single Proximity Cases and dual adjacent Prox cases
        switch( irqstatus & 0x1c )
        {
            // Single PS1 case
            case 0x04:
                SMB_DATA_OUT[0]      =      0x21+5;
                numberOfBytesToRead  =           2;  
                SMB_DATA_IN          =    values+5;
                break;
            // Single PS2 case
            case 0x08:
                SMB_DATA_OUT[0]      =      0x21+7;
                numberOfBytesToRead  =           2;  
                SMB_DATA_IN          =    values+7;
                break;
            // Single PS3 case
            case 0x10:
                SMB_DATA_OUT[0]      =      0x21+9;
                numberOfBytesToRead  =           2;  
                SMB_DATA_IN          =    values+9;
                break;

            // PS1 + PS2 case
            case 0x0c:
                SMB_DATA_OUT[0]      =      0x21+5;
                numberOfBytesToRead  =           4;  
                SMB_DATA_IN          =    values+5;
                break;
                
            // PS2 + PS3 case
            case 0x18:
                SMB_DATA_OUT[0]      =      0x21+7;
                numberOfBytesToRead  =           4;  
                SMB_DATA_IN          =    values+7;
                break;

            // For all other cases, get all PS channels
            default:
                SMB_DATA_OUT[0]      =      0x21+5;
                numberOfBytesToRead  =           6;  
                SMB_DATA_IN          =    values+5;
        }
        SMB_Write(); 
        SMB_Read(); 
    }

    return irqstatus;
}
