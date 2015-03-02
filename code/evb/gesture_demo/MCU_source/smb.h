#include "global.h"

S16 i2c_smbus_read_byte( U8 i2c_handle );
S16 i2c_smbus_write_byte( U8 i2c_handle, U8 value );

S16 i2c_smbus_read_byte_data      ( U8 i2c_handle, U8 reg_addr );
S16 i2c_smbus_write_byte_data     ( U8 i2c_handle, U8 reg_addr, U8 reg_data );

S16 i2c_smbus_read_i2c_block_data ( U8 i2c_handle, U8 reg_addr, U8 length, U8 *values );

S16 i2c_smbus_read_i2c_block_idata ( U8 i2c_handle, U8 reg_addr, U8 length, U8 idata *values );
S16 i2c_smbus_read_block_idata  ( U8 i2c_handle, U8 length, U8 idata *values );
S16 i2c_smbus_write_block_idata ( U8 i2c_handle, U8 length, U8 idata *values );

S16 i2c_smbus_optimized( U8 i2c_handle, U8 idata *values );
