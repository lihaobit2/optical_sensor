#include "global.h"
#include "queue.h"
#include "Si114x_functions.h"

//-----------------------------------------------------------------------------
// Global Variables Defined here, and shared across files
//-----------------------------------------------------------------------------
//
U8 xdata     CMD_CNT = 0;
bit     PM_INT  = 0;                    // Port Match flag for Main routine
xdata   SI114X_IRQ_SAMPLE Si114xsamples;
bit     TXStartedFlag = 0;
bit     sw_reset = 0;
U16     Timestamp;

//-----------------------------------------------------------------------------
// Global Variables defined elsewhere
//-----------------------------------------------------------------------------
extern U8 xdata master;           // From foreground.c

//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------
//
void SMBus_Init (U8 i2c_speed);         // From smb.c
void Queue_Init (void);         // From queue.c

void Process_CMD(void);         // From foreground.c
void delay_10ms();              // From Si114x_sys_out.c
s16  si114x_init(HANDLE handle); // From si114x_init.c


//-----------------------------------------------------------------------------
// Disable_Watchdog
//-----------------------------------------------------------------------------
//
// Disables the watchdog timer
//
void Disable_Watchdog(void)
{
   PCA0MD &= ~0x40;                    // Disable watchdog timer
}


//-----------------------------------------------------------------------------
// SYSCLK_Init
//-----------------------------------------------------------------------------
//
// This routine initializes the system clock to use the internal precision 
// oscillator at its maximum frequency and enables the missing clock 
// detector.
// 
void SYSCLK_Init (void)
{
#if ((BDNUM == 0) || (BDNUM == 1) || (BDNUM == 6))
   OSCICN = 0x83;                      // Enable the precision internal osc.   
   RSTSRC = 0x04;                      // Enable missing clock detector and
                                       // disable VDD Monitor

   CLKSEL = 0x00;                      // Select precision internal osc.

#elif ((BDNUM == 2) || (BDNUM == 3) || (BDNUM == 4) || ( BDNUM == 5))
   OSCICN |= 0x80;                     // Enable the precision internal osc.
   while((OSCICN & 0x40) == 0);        // Wait till IFRDY is set

   RSTSRC = 0x04;                      // Enable missing clock detector and
                                       // disable VDD Monitor

   FLSCL  = 0x40;                      // Configre Flash read timing for
                                       // SYSCLK >10 MHz.

   CLKSEL = 0x00;                      // Select precision internal osc.
                                       // divided by 1 as the system clock
#endif
}

//-----------------------------------------------------------------------------
// UART0_Init
//-----------------------------------------------------------------------------
//
// Configure the UART0 using Timer1, for <BAUDRATE> and 8-N-1.
//

void UART0_Init ()
{
   SCON0 = 0x10;                       // SCON0: 8-bit variable bit rate
                                       //        level of STOP bit is ignored
                                       //        RX enabled
                                       //        ninth bits are zeros
   TMOD &= ~0xf0;                      // TMOD: timer 1 in 8-bit autoreload
                                       //        clear RI0 and TI0 bits
   
   TH1 = -(SYSCLK/BAUDRATE/2);
   CKCON |=  0x08;

   TL1 = TH1;                          // init Timer1
   TMOD &= ~0xf0;                      // TMOD: timer 1 in 8-bit autoreload
   TMOD |=  0x20;
   TR1 = 1;                            // START Timer1
   
   IE |= 0x10;

   IP &= ~0x10;                         // Make UART Low Priority Interrupt
                                        // to allow SMB to preempt UART

   ES0 = 1;                            // Enable UART0 interrupts

   TI0 = 1;
}

//-----------------------------------------------------------------------------
// StartTX
//-----------------------------------------------------------------------------
//
void StartTX(void)
{
    if (TXStartedFlag == 0)
    {
      TXStartedFlag = 1;
      TI0 = 1;
    }
}

//-----------------------------------------------------------------------------
// UART Interrupt Handler
//-----------------------------------------------------------------------------
//
INTERRUPT(UART0_ISR, INTERRUPT_UART0)
{

   if (RI0 == 1)
   {
      U8 byte;
      byte = SBUF0;                     // Clear interrupt flag      
      RI0 = 0;      
      // check if latest received byte is a carriage return. if so, new CMD received
      if( byte == 0x0D )
      {
         CMD_CNT++;
      }

      RXQ_insert(byte);
              // Read a character from UART
                                        // and insert it into the receive queue

   }

   if (TI0 == 1)                         // Check if transmit flag is set
   {
      U8 idata byte;
      TI0 = 0;                           // Clear interrupt flag
      if( !TXQ_isempty() )
      {
         TXQ_delete(&byte);
         SBUF0 = byte;
      } else TXStartedFlag = 0;               
   }

}

//-----------------------------------------------------------------------------
// Timer0_Init
//-----------------------------------------------------------------------------
//

void Timer0_Init()
{
   TMOD |= 0x02;                       // 8-bit counter/timer
   CKCON |= 0x02;                      // Sysclock / 48
   TH0 = -(SYSCLK/48/10000);           // 10 KHz (100us) interrupt frequency

   TR0 = 1;

   IP &= ~0x02;                        // Timer0 Low Priority Interrupt so that
                                       // it is safe to get counter value
                                       // while within any interrupt handler

   IE |= 0x02;                         // Enable Timer0 Interrupt
}


//-----------------------------------------------------------------------------
// Timer0 Interrupt Handler
//-----------------------------------------------------------------------------
//
static U16 idata Counter_100us_mem;
static U16 idata *Counter_100us  = &Counter_100us_mem;

INTERRUPT (TIMER0_ISR, INTERRUPT_TIMER0)
{
   Counter_100us_mem++;
}

//-----------------------------------------------------------------------------
// getCounter ensures that the two bytes in 16-bit value are kept together
//-----------------------------------------------------------------------------
//
U16 getCounter(U16 idata *counter_ptr)
{
    U16 value;
    EA = 0;
    value = *counter_ptr; 
    EA = 1;

    return value;
}

//-----------------------------------------------------------------------------
// Use get100usCounter in foreground tasks so no pointers need to be passed
// across files.
//-----------------------------------------------------------------------------
//
U16 get100usCounter(void) 
{
    return getCounter(Counter_100us);
}


//-----------------------------------------------------------------------------
// PORT Match Interrupt Handler
//-----------------------------------------------------------------------------

INTERRUPT(PORT_Match, INTERRUPT_PORT_MATCH)
{

   PM_INT = 1;                                  // Set Port Match flag for Main routine
   EA = 0;
   Timestamp = Counter_100us_mem;
   EA = 1;
}


//-----------------------------------------------------------------------------
// PORT_Init
//-----------------------------------------------------------------------------
// Configure the Crossbar and GPIO ports.
//
void PORT_Init (void)
{

#if BDNUM == 5
   P0MDOUT  = 0x00;                    // All P0 pins as open-drain 
   P0MDOUT |= 0x10;                    // Enable UTX as push-pull output

   P0SKIP   = 0x01;                    // Makes P0.1 and P0.2 SMBUS
   P0MASK   = 0x40;                    // Si114x INT pin on P0.6 for Port Match

   P1MDOUT  = 0x00;                    // All P1 pins as open-drain

   SFRPAGE  = CONFIG_PAGE;
   P0DRV    = 0xFF;                    // Port 0 High Output Drive Strength
   P1DRV    = 0xFF;                    // Port 1 High Output Drive Strength
   SFRPAGE  = LEGACY_PAGE;

   XBR0     = 0x05;                    // Enable SMBus and UART pins
   XBR2     = 0x40;                    // Enable crossbar and weak pull-ups

   EIE2 = 0x02;                        // Enable Port Match Interrupts

#elif BDNUM == 4
   P0MDOUT  = 0x00;                    // All P0 pins as open-drain 
   P0MDOUT |= 0x10;                    // Enable UTX as push-pull output

   P0SKIP   = 0x05;                    // Makes P0.1 and P0.3 SMBUS
   P0MASK   = 0x40;                    // Si114x INT pin on P0.6 for Port Match

   P1MDOUT  = 0x00;                    // All P1 pins as open-drain

   SFRPAGE  = CONFIG_PAGE;
   P0DRV    = 0xFF;                    // Port 0 High Output Drive Strength
   P1DRV    = 0xFF;                    // Port 1 High Output Drive Strength
   SFRPAGE  = LEGACY_PAGE;

   XBR0     = 0x05;                    // Enable SMBus and UART pins
   XBR2     = 0x40;                    // Enable crossbar and weak pull-ups

   EIE2 = 0x02;                        // Enable Port Match Interrupts

#elif ((BDNUM == 2) || (BDNUM == 3))
   P0MDOUT  = 0x00;                    // All P0 pins as open-drain 
   P0MDOUT |= 0x10;                    // Enable UTX as push-pull output

   P0SKIP  = 0x03;                     // Skip 2 pins 
   P0MASK  = 0x40;                     // Port Match on P0.6
   P0MAT   = 0x40;
   
   XBR0 = 0x05;                        // Enable SMBus and UART pins
   XBR2 = 0x40;                        // Enable crossbar and weak pull-ups

   EIE2 = 0x02;

#elif ((BDNUM == 0) || (BDNUM == 6))
   P0MDOUT  = 0x00;                    // All P0 pins as open-drain 
   P0MDOUT |= 0x10;                    // Enable UTX as push-pull output

   P0SKIP  = 0x03;                     // Skip 2 pins 
   P0MASK  = 0x40;                     // Port Match on P0.6
   P0MAT   = 0x40;
   
   XBR0 = 0x05;                        // Enable SMBus and UART pins
   XBR1 = 0x40;                        // Enable crossbar and weak pull-ups

   EIE1 = 0x02;

#elif BDNUM == 1
   P0MDOUT  = 0x00;                    // All P0 pins as open-drain 
   P0MDOUT |= 0x10;                    // Enable UTX as push-pull output

   P0SKIP  = 0x0F;                     // Skip 4 pins 
   P1MASK  = 0x01;                     // Port Match on P1.0
   P1MAT   = 0x01;
   
   XBR0 = 0x05;                        // Enable SMBus and UART pins
   XBR1 = 0x40;                        // Enable crossbar and weak pull-ups

   EIE1 = 0x02;

#endif   
}

//-----------------------------------------------------------------------------
// main
//-----------------------------------------------------------------------------
//
void main (void)
{
   Disable_Watchdog();                 // Disables watchdog timer

   sw_reset = 1;
   if( RSTSRC&0x02 || RSTSRC&0x01) sw_reset = 0;     // Set sw_reset flag accordingly   
  

   SYSCLK_Init();                      // Initialize system clock
   UART0_Init();                       // Initialize UART 0
   Timer0_Init();                      // Initialize timer 0   
   PORT_Init();                        // Initialize port pins

   SMBus_Init(I2CSPEED/10000);        // Initialize I2C

   Queue_Init();                       // Initialize Queues

   EA = 1;                             // Enable interrupts
   
   delay_10ms();                       // Delay to ensure Si114x is completely booted
   delay_10ms();                       
   delay_10ms();                       

   if(!sw_reset) si114x_init( 0xB4+((master-1)<<1));

   while (1)
   {
      if(CMD_CNT)
      {
         Process_CMD();
      }

      if(PM_INT)
      {
          Process_INT();
      }
   } 
}

