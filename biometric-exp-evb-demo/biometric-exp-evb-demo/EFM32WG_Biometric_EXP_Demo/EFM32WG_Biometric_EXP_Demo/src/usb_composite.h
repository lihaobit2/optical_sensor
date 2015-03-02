/*
 * usbcomposite.h
 *
 *  Created on: Jul 11, 2014
 *      Author: reqian
 */
#include "si114xHRM.h"
#include "em_usb.h"

#ifndef USBCOMPOSITE_H_
#define USBCOMPOSITE_H_

void USBComposite_Init(void);
int USBComposite_SetupCmd(const USB_Setup_TypeDef *setup);
void USBComposite_StateChangeEvent( USBD_State_TypeDef oldState, USBD_State_TypeDef newState );
bool USBComposite_IsUSBConfigured();

#endif /* USBCOMPOSITE_H_ */
