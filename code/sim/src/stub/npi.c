#include <stdio.h>
#include "hal_types.h"

uint16 NPI_WriteStrValue(char *buf, uint16 value)
{
	printf(buf, value);
	return 0;
}

uint16 NPI_WriteString( char *buf)
{
	printf(buf);

	return 0;
}



