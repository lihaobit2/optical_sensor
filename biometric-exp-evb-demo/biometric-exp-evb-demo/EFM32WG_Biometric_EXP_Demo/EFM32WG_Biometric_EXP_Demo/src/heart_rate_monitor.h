/**************************************************************************//**
 * @file
 * @brief Heart Rate and SpO2 state machine
 * @version 3.20.3
 ******************************************************************************
 * @section License
 * <b>(C) Copyright 2014 Silicon Labs, http://www.silabs.com</b>
 *******************************************************************************
 *
 * This file is licensensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 *
 ******************************************************************************/

#ifndef __HEARTRATEMONITOR_H
#define __HEARTRATEMONITOR_H
#include "si114x_sys_out.h"
typedef enum
{
  BIOMETRIC_EXP,
  SI1143_PS,
  FITNESS_EVB_GGG,
  FITNESS_EVB_GRI,
  SI1147_PS
}HeartRateMonitor_Config_t ;

void HeartRateMonitor_Init(Si114xPortConfig_t* i2c, HeartRateMonitor_Config_t , bool usbEnabled);
bool HeartRateMonitor_Loop ( bool displaySpO2, bool forceStop, bool checkSkinContact );
void HeartRateMonitor_TimerEventHandler(void);
void HeartRateMonitor_Interrupt (void);
bool HeartRateMonitor_SamplesPending ();
void HeartRateMonitor_GetVersion (int8_t *hrmVersion);

#endif /* __HEARTRATEMONITOR_H */
