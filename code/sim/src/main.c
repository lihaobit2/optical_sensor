#include <stdio.h>
#include <string.h>
#include "Si114x_types.h"


#define MAX_DATA_NUM 32

extern U16 Timestamp = 0;	
extern bit sw_reset;

extern void Process_INT();

char g_DataIn[MAX_DATA_NUM];

void GestureInit()
{
	Timestamp = 0;	
	sw_reset = 0;
}

int main()
{
	FILE *fp;

    fp = fopen("data.txt","r");
	if(fp == NULL)
	{
		printf("File open error!");
		
		return 1;
	}
    
	memset(g_DataIn, 0, sizeof(g_DataIn));

	GestureInit();
	while(!feof(fp))
	{
		fscanf(fp, "%d,%d %d,%d, %d,%d,%d,%d, %d,%d,%d,%d, %d",\
	    			g_DataIn[0], g_DataIn[1], g_DataIn[2], g_DataIn[3],\
	    			g_DataIn[4], g_DataIn[5], g_DataIn[6], g_DataIn[7],\
	    			g_DataIn[8], g_DataIn[9], g_DataIn[10], g_DataIn[11],\
	    			g_DataIn[12]);
	    			
		Process_INT();

	}

	fclose(fp);
	
	return 0;
}
