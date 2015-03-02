/*
 * usboutput.h
 *
 *  Created on: Jul 14, 2014
 *      Author: reqian
 */


#include "si114xhrm.h"



#include "em_usb.h"

#ifndef USBOUTPUT_H_
#define USBOUTPUT_H_

void USBOutput_ProcessUSBOutput(int8_t *data_buffer);
void USBOutput_ProcessConfigurationMessage(char *message, Si114xhrmConfiguration_t *configuration);

#endif /* USBOUTPUT_H_ */

