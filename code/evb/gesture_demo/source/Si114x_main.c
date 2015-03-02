#include <stdio.h>
#include <windows.h>
#include "Si114x_functions.h"

extern HANDLE si114x_handle;

s16 si114x_init(HANDLE si114x_handle);

int  si114x_evb_open(void);
void si114x_evb_close(void);
void si114x_main_loop(void);

int main(int argc, char *argv[])
{
    s16 retval;

    retval = si114x_evb_open();

    retval = setup_debug( si114x_handle );
    if (retval != 0 ) 
    {
        printf("si114x_setup_debug failed: %d\n", retval);
        goto exit;
    }

    retval = si114x_init( si114x_handle );
    if (retval == 0)
    {
        printf("si114x_init successful\n");
    } 
    else
    {
        printf("si114x_init failed\n");
        goto exit;
    }

    si114x_main_loop();

exit:
    si114x_evb_close();

    return 0;
}
