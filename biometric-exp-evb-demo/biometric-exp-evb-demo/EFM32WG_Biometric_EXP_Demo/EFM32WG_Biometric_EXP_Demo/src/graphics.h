/**************************************************************************//**
 * @brief Draws the graphics on the display
 * @version 3.20.3
 *******************************************************************************
 * @section License
 * <b>(C) Copyright 2014 Silicon Labs, http://www.silabs.com</b>
 *******************************************************************************
 *
 * This file is licensensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 *
 ******************************************************************************/

#ifndef __GRAPHICS_H
#define __GRAPHICS_H

#include <stdint.h>
#include <stdbool.h>
#include "heart_rate_monitor.h"
#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 *****************************   PROTOTYPES   **********************************
 ******************************************************************************/
void GRAPHICS_Init(bool boost);
bool GRAPHICS_ShowHRMStatus(bool acquiring, int32_t pulse, bool scrollInfo);
bool GRAPHICS_ShowSpO2Status(bool acquiring, int32_t spo2, bool scrollInfo);
void GRAPHICS_SetBatteryStatus(bool lowBat);
void GRAPHICS_DrawUV(uint32_t uvData);
void GRAPHICS_DrawHumidity(uint32_t rhData);
void GRAPHICS_DrawTemperatureC(int32_t tempData);
void GRAPHICS_DrawTemperatureF(int32_t tempData);
void GRAPHICS_DrawError ();
void GRAPHICS_DrawInit (HeartRateMonitor_Config_t hrm_config, char *hrmVersion, char*biodemoVersion, bool usbStatus);
void GRAPHICS_ShowLEDSelectMenu();
void GRAPHICS_ChangeScrollText (bool wrist);

typedef enum 
{
  LCD_HRM,
  LCD_SPO2,
  LCD_RH,
  LCD_TEMPC,
  LCD_TEMPF,
  LCD_UV
}displayType_t;

#ifdef __cplusplus
}
#endif


#endif /* __GRAHPHICS_H */
