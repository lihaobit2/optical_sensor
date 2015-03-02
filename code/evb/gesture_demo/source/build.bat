@ECHO OFF
cl -o gesture_demo_indoors.exe -DINDOORS                                   /MT Si114x_main.c Si114x_evb.c Si114x_debug.c Si114x_functions.c Si114x_init.c Si114x_handler.c Si114x_sys_out.c  Si114x_PGM_Toolkit.lib

cl -o gesture_demo_general_clear_overlay.exe -DGENERAL_CLEAR_OVERLAY       /MT Si114x_main.c Si114x_evb.c Si114x_debug.c Si114x_functions.c Si114x_init.c Si114x_handler.c Si114x_sys_out.c Si114x_PGM_Toolkit.lib

cl -o gesture_demo_general_infrared_overlay.exe -DGENERAL_INFRARED_OVERLAY /MT Si114x_main.c Si114x_evb.c Si114x_debug.c Si114x_functions.c Si114x_init.c Si114x_handler.c Si114x_sys_out.c Si114x_PGM_Toolkit.lib
