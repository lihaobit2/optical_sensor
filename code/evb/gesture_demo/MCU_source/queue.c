#include "global.h"
#include "queue.h"

/*
 * The queue can be called from both an interrupt and 
 * the main code. To allow this usage, these functions
 * are bracketed with EA=0 and EA=1
 *
 * In Keil, a 'WARNING L15: MULTIPLE CALL TO SEGMENT'
 * can be expected when these functions are used.
 *
 * Refer to http://www.keil.com/support/docs/2042.htm
 */

static U8 idata    TXQ_front;
static U8 idata    TXQ_rear;
static U8 idata    TXQ_array[TXQ_SIZE];

static U8 idata    RXQ_front;
static U8 idata    RXQ_rear;
static U8 idata    RXQ_array[RXQ_SIZE];


/*
 * Initializes the queue
 */
void TXQ_init()
{
    TXQ_front   = 0;
    TXQ_rear    = 0;
}

/*
 * Returns 1 if the queue is empty; zero otherwise
 */
bit  TXQ_isempty()
{
    bit status = 0;
    bit tmpEA = EA;
    EA = 0;
    if( TXQ_front == TXQ_rear ) status = 1; 
    EA = tmpEA;
    return status;
}



/*
 * Inserts an element into the queue.
 */
void  TXQ_insert(char value)
{
    bit tmpEA = EA;
    EA = 0;
    TXQ_rear = ( TXQ_rear + 1 ) % (TXQ_SIZE) ;
    TXQ_array[TXQ_rear] = value;
    EA = tmpEA;
}

/*
 * Removes an element from the queue. 
 */
void  TXQ_delete(char idata *value)
{
    bit tmpEA = EA;
    EA = 0;
    TXQ_front = ( TXQ_front + 1 ) % (TXQ_SIZE) ;
    *value = TXQ_array[TXQ_front];
    EA = tmpEA;
}

/*
 * Initializes the queue
 */
void RXQ_init()
{
    RXQ_front   = 0;
    RXQ_rear    = 0;
}

#if 0
/*
 * Returns 1 if the queue is empty; zero otherwise
 */
bit  RXQ_isempty()
{
    bit status = 0;
    bit tmpEA = EA;
    EA = 0;
    if( RXQ_front == RXQ_rear ) status = 1; 
    EA = tmpEA;
    return status;
}
#endif



/*
 * Inserts an element into the queue.
 */
void  RXQ_insert(char value)
{
    bit tmpEA = EA;
    EA = 0;
    RXQ_rear = ( RXQ_rear + 1 ) % (RXQ_SIZE) ;
    RXQ_array[RXQ_rear] = value;
    EA = tmpEA;
}

/*
 * Removes an element from the queue. 
 */
void  RXQ_delete(char idata *value)
{
    bit tmpEA = EA;
    EA = 0;
    RXQ_front = ( RXQ_front + 1 ) % (RXQ_SIZE) ;
    *value = RXQ_array[RXQ_front];
    EA = tmpEA;
}

// Converts a Hex value into an ASCII value
// ie.- 0xD = 'D'
U8 HEX_ASCII( U8 hex )
{
   if( hex < 0x0A )
   {
      return (hex + 0x30);
   } else
   {
      return (hex + 0x37);
   }
}

// Converts an 8-bit value into 2 ASCII chars and inserts into TX Q   
void TXQ_insert_hex( U8 hex )
{
   TXQ_insert(HEX_ASCII(hex >> 4));
   TXQ_insert(HEX_ASCII(hex & 0x0F));   
}

// Converts a 16-bit value into 4 ASCII chars and inserts into TX Q
void TXQ_insert_hex_sixteen( U16 hex )
{
   TXQ_insert(HEX_ASCII(hex>>12));
   TXQ_insert(HEX_ASCII((hex>>8) & 0x0F));
   TXQ_insert(HEX_ASCII((hex>>4) & 0x0F));
   TXQ_insert(HEX_ASCII(hex & 0x0F));   
}

//-----------------------------------------------------------------------------
// Queue_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
void Queue_Init (void)
{
   TXQ_init();
   RXQ_init();
}
