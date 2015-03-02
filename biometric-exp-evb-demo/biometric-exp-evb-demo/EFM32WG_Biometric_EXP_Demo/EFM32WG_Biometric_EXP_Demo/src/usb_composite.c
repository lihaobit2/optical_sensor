/**************************************************************************//**
 * @file
 * @brief USB Composite Device
 * @version 3.20.5
 ******************************************************************************
 * @section License
 * <b>(C) Copyright 2014 Silicon Labs, http://www.silabs.com</b>
 *******************************************************************************
 *
 * This file is licensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 *
 ******************************************************************************/

#include <stdio.h>
#include "em_device.h"
#include "em_usb.h"
#include "em_assert.h"
#include "em_cmu.h"
#include "em_gpio.h"
#include "em_chip.h"
#include "segmentlcd.h"
#include "cdc.h"
#include "usbconfig.h"
#include "em_emu.h"
#include "i2cdrv.h"
#include "i2c1drv.h"
#include "si7013.h"
#include "rtcdriver.h"
#include "em_adc.h"
#include "udelay.h"
#include "si114xHRM.h"

extern const USBD_Init_TypeDef initstruct;
/* Flag used to determine if USB is connected */
static int usbConfigured = 0;

/**************************************************************************//**
 * @brief usbcomposite_Init - Initialize the Composite USB Device
 *****************************************************************************/
void USBComposite_Init( void )
{
  Si114xhrmHandle_t _handle = NULL;
  int debug = 0;
  CMU_ClockSelectSet( cmuClock_HF, cmuSelect_HFXO );
  CMU_OscillatorEnable(cmuOsc_LFXO, true, false);

  /* Setup USB debug function */
  si114xhrm_SetupDebug(_handle, (int *)&debug);

  /* Start USB */
  USBD_Init(&initstruct);
}

/**************************************************************************//**
 * @brief
 *   Called whenever a USB setup command is received.
 *
 * @param[in] setup
 *   Pointer to an USB setup packet.
 *
 * @return
 *   An appropriate status/error code. See USB_Status_TypeDef.
 *****************************************************************************/
int USBComposite_SetupCmd(const USB_Setup_TypeDef *setup)
{
  /* Call SetupCmd handlers for all functions within the composite device. */
  int retVal;
  retVal = CDC_SetupCmd( setup );
  return retVal;
}

/**************************************************************************//**
 * @brief
 *   Called whenever the USB device has changed its device state.
 *
 * @param[in] oldState
 *   The device USB state just leaved. See USBD_State_TypeDef.
 *
 * @param[in] newState
 *   New (the current) USB device state. See USBD_State_TypeDef.
 *****************************************************************************/
void USBComposite_StateChangeEvent( USBD_State_TypeDef oldState,
                       USBD_State_TypeDef newState )
{
  /* Call device StateChange event handlers for all relevant functions within the composite device. */
  Si114xhrmHandle_t _handle = NULL;
  int debug;
  /* Determine if USB is connected. Based on this, determine whether or not to */
  /* enable the data dump feature and/or to enter low power mode.              */
  if(newState == USBD_STATE_CONFIGURED)
  {
	  usbConfigured = 1;
      debug = 1;
  	  si114xhrm_SetupDebug(_handle, (int *)&debug);
  }
  else
  {
	  usbConfigured = 0;
      debug = 0;
      si114xhrm_SetupDebug(_handle, (int *)&debug);
  }
}

/**************************************************************************//**
 * @brief usbcomposite_isUSBConfigured - Check if USB is connected
 *****************************************************************************/
bool USBComposite_IsUSBConfigured()
{
	if (usbConfigured)
		return true;
	return false;
}

