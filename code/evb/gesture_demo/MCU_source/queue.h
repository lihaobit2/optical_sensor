
/*
 * Functions prototypes for circular queue
 */
#define   TXQ_SIZE  66
#define   RXQ_SIZE  34

extern U8 idata     TXQ_front; 
extern U8 idata     TXQ_rear; 
extern U8 idata     TXQ_array[TXQ_SIZE]; 

extern U8 idata     RXQ_front;
extern U8 idata     RXQ_rear;
extern U8 idata     RXQ_array[RXQ_SIZE];

void TXQ_init       ();
bit  TXQ_isempty    ();
void TXQ_insert     (char  value               );
void TXQ_delete     (char idata *value         );

void RXQ_init       ();
bit  RXQ_isempty    ();
void RXQ_insert     (char  value               );
void RXQ_delete     (char idata *value         );

void TXQ_insert_hex( U8 hex );
void TXQ_insert_hex_sixteen( U16 hex );

