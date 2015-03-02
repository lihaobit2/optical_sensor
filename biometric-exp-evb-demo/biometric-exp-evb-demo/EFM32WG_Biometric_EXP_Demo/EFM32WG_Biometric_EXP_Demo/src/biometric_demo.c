/**************************************************************************//**
 * @file
 * @brief Demo for EFM32WG_STK3800 and Biometric Sensor EXP
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

#include "em_device.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "em_emu.h"
#include "em_gpio.h"
#include "i2cdrv.h"
#include "i2c1drv.h"
#include "si7013.h"
#include "heart_rate_monitor.h"
#include "rtcdriver.h"
#include "graphics.h"
#include "em_adc.h"
#include "udelay.h"
#include "si114x_functions.h"
#include "si114x_uv_index.h"
#include "usb_composite.h"
#include "si114x_sys_out.h"
#include "si114x_functions.h"
#define BIOMETRIC_DEMO_VERSION "1.0"


/**************************************************************************//**
 * Local defines
 *****************************************************************************/
/** Time (in ms) between periodic updates of the measurements. */
#define PERIODIC_UPDATE_MS     1000
/** Time (in ms) between characters of the scrolling text. */
#define UPDATE_SCROLL_MS     250
/** Time (in ms) between starts of the scrolling text. */
#define START_SCROLL_MS     6000
/** Voltage defined to indicate dead battery. */
#define LOW_BATTERY_THRESHOLD    2900
#define SI7013_ADDR_0 (0x41<<1)
#define SI7013_ADDR_1 (0x40<<1)

/**************************************************************************//**
 * Local variables
 *****************************************************************************/
/* RTC callback parameters. */
static void          (*rtcCallback)(void*) = NULL;
static void          * rtcCallbackArg = 0;


/** This flag tracks if we need to update the display 
 *  (measurements). */
static volatile bool updateDisplay   = true;
/** This flag tracks if we need to scroll a new
 *  character. */
static volatile bool updateScrollText   = false;
/** This flag tracks if we need to perform a new 
 *  measurement. */
static volatile bool updateMeasurement   = true;
/** Flag used to indicate ADC is finished */
static volatile bool adcConversionComplete = false;
/** Flag used to indicate lcd boost enabled */
static bool oldBoost = false;
/** Flag used to indicate what to diplay */
static volatile displayType_t    displayType = LCD_HRM;
/** Flag used to indicate whether HRM is active. */
static bool hrmActive = false;
/** HRM hardware configuration */
static HeartRateMonitor_Config_t hrmConfig = BIOMETRIC_EXP;
/** Flag used to restart HRM algorithm */
static bool             reinitHRM = false;
/** Flag used to determine if USB is enabled */
static bool             usbEnabled = false;
/** Flag used to determine if advanced board selection is enabled*/
static bool             advMenuEnabled = false;
Si114xPortConfig_t uvI2C;
Si114xPortConfig_t hrmI2C;

/** Timer used for periodic update of the measurements. */
RTCDRV_TimerID_t     periodicUpdateTimerId;
/** Timer used for scrolling text. */
RTCDRV_TimerID_t     updateScrollTimerId;



/**************************************************************************//**
 * Local prototypes
 *****************************************************************************/
static void gpioSetup(void);
static void periodicUpdateCallback(RTCDRV_TimerID_t id, void *user);
static void updateScrollTextCallback(RTCDRV_TimerID_t id, void *user);
static uint32_t checkBattery (void);
static void adcInit( void );
static void configSi114xIntInput ();

/**************************************************************************//**
 * @brief  Helper function to perform data measurements.
 *****************************************************************************/
static int performMeasurements(uint32_t *rhData, int32_t *tData, 
                               uint16_t *uvData, uint32_t *vBat)
{
  bool vboost;
  *rhData = 1234;
  *tData = 5678;
  *vBat = checkBattery();
  Si7013_MeasureRHAndTemp(I2C1, SI7013_ADDR_0, rhData, tData);
  Si114x_MeasureUVIndex(uvData);
  if (*vBat < 2900)
  {
    vboost = true;
  }
  else
    vboost = false;
  
  if (vboost != oldBoost)
  {
    GRAPHICS_Init(vboost);
    oldBoost = vboost;
  }
  return 0;
}

/**************************************************************************//**
 * @brief  Detects HRM device on ribbon cable.
 *****************************************************************************/
static void detectHRMDevice()
{
  hrmConfig = BIOMETRIC_EXP;
  /* Check for Si1143 or other device on i2c1 6 pin flex cable connector */
  /* If we detect another EVB connected via flex cable switch heart rate
   * configuration to use the EVB connected instead of the Si1146 on the
   * EXP board.
   */
  // First check for Si1143 on address 0x5A on flex cable.
  hrmI2C.i2c_address = 0x5A;
  hrmI2C.i2c_bus_select = 1;
  if (Si114xInit(&hrmI2C,0,0)<0)
  {
    // We didn't find an Si1143. 
    // If no Si1143 detected, check for the Si1147 postage stamp
    hrmI2C.i2c_address = 0x60;
    if (Si114xInit(&hrmI2C,0,0)<0)
      hrmI2C.i2c_bus_select = 0;
    else
      // Found a Si1147 so reconfigure. 
      hrmConfig = SI1147_PS;
  }
  else
  {
    /* Found an Si1143. Need to discover which EVB and reconfigure. */
    /* Set up HRM i2c parameters for Si1143 on i2c bus 1. */
    /* Figure out which board we are connected to.
     * If we detect an Si7013 also on i2c 1 then we have a fitness EVB. */
    if (Si7013_Detect(I2C1, SI7013_ADDR_1))
    {
      /* Fitness evb detected. - Need to prompt user to choose GGG or GRI
       * LED configuration if advanced menu enabled.
       */
      if (advMenuEnabled)
      {
        RTCDRV_StartTimer(updateScrollTimerId, rtcdrvTimerTypePeriodic,
                        UPDATE_SCROLL_MS, updateScrollTextCallback, NULL);
        /* displayType is changed by push button press. */
        while (displayType == LCD_HRM) //wait for push button
        {
          if (updateScrollText)
          {
            GRAPHICS_ShowLEDSelectMenu();
          }
          if ( (USBD_SafeToEnterEM2() && usbEnabled) || (usbEnabled == false))
            EMU_EnterEM2(usbEnabled);
          else
          {
            RTCDRV_Delay(250);
            updateScrollText = true;
          }
        }
        /* User has chosen PB0 or PB1.*/
        hrmConfig = FITNESS_EVB_GGG;
        if (displayType == LCD_UV)
          hrmConfig = FITNESS_EVB_GRI;
        RTCDRV_StopTimer(updateScrollTimerId);
        displayType = LCD_HRM;
      }
      else
      {
    	  hrmConfig = FITNESS_EVB_GGG;
      }
      /* Change instruction text to "wrist" instead of "finger" for fitness EVB. */
      GRAPHICS_ChangeScrollText (true);
    }
    else
    {
      /* If we did not find a Si7013 then it must be a Si1143 postage stamp. */
      hrmConfig = SI1143_PS;
    }
  }
}

/**************************************************************************//**
 * @brief  Main function
 *****************************************************************************/
int main(void)
{
  I2C_Init_TypeDef i2cInit = I2C_INIT_DEFAULT;
  uint32_t         rhData;
  bool             si7013_status = true, si1146_status;
  int32_t          tempData;
  uint16_t         uvData = 50;
  uint32_t         vBat = 3300;
  bool             lowBatPrevious = true;
  bool             lowBat = false;

  char             hrmVersion[10];
  bool             scrollStatus;
  bool             hrmCheckSkinContact = true;
  uvI2C.i2c_address = 0x60;
  uvI2C.i2c_bus_select = 0;
  hrmI2C.irqPin = 5;
  hrmI2C.irqPort = gpioPortD;
  uvI2C.irqPin = 5;
  uvI2C.irqPort = gpioPortD;
  
  /* Chip errata */
  CHIP_Init();

  /* Misc initalizations. */
  gpioSetup();
  adcInit();

  /* Misc timers. */
  RTCDRV_Init();
 
  RTCDRV_AllocateTimer(&periodicUpdateTimerId);
  RTCDRV_AllocateTimer(&updateScrollTimerId);
  
  /* Initialize display. */
  GRAPHICS_Init(false);

  updateMeasurement = false;

  /*Check if we need to enable USB. */
  /*USB is enabled by holding down PB0 during reset*/
  if (GPIO_PinInGet(gpioPortB, 9)==0) /*PB0 is pressed*/
  {
    /* USB initialization */
    USBComposite_Init();
    usbEnabled = true;
  }

  if (GPIO_PinInGet(gpioPortB, 10)==0) /*PB1 is pressed*/
    advMenuEnabled = true;

  CMU_OscillatorEnable(cmuOsc_HFXO, true, true);
  /* Enable HFXO as the main clock */
  CMU_ClockSelectSet( cmuClock_HF, cmuSelect_HFXO );

  if (usbEnabled == false)
    CMU_ClockDivSet(cmuClock_HF,cmuClkDiv_8); /*save power if we are not using USB peripheral*/

  /* Initialize I2C driver, using standard rate. */
   I2CDRV_Init(&i2cInit);
   I2C1DRV_Init(&i2cInit);

  /* Detect Si7013 device on EXP board. */
  si7013_status = Si7013_Detect(I2C1, SI7013_ADDR_0);

  /* Detect Si1146 device on EXP board. */

  /*check for Si1146*/
  if (Si114xInit(&uvI2C,0,0)<0)
    si1146_status = false;
  else
    si1146_status = true;
  
  detectHRMDevice();
  I2C1DRV_Init(&i2cInit);
  I2CDRV_Init(&i2cInit);

  HeartRateMonitor_GetVersion(hrmVersion);

  /* Show splash screens informing user of configuration. */
  GRAPHICS_DrawInit(hrmConfig, hrmVersion,BIOMETRIC_DEMO_VERSION, usbEnabled);

  /* If EXP board fails display error message. */
  if ((si1146_status == false) || (si7013_status == false))
  {
    GRAPHICS_DrawError();
    if ( (USBD_SafeToEnterEM2() && usbEnabled) || (usbEnabled == false))
      EMU_EnterEM2(usbEnabled);
    while(1);
  }

  RTCDRV_StartTimer(periodicUpdateTimerId, rtcdrvTimerTypePeriodic, 
                    PERIODIC_UPDATE_MS, periodicUpdateCallback, NULL);

  updateDisplay = true;

  HeartRateMonitor_Init(&hrmI2C, hrmConfig, usbEnabled);

  /* Switch Si114x driver to talk to Si1146 on EXP board to initialize UV measurement. */
  if (hrmConfig != BIOMETRIC_EXP)
     Si114xInit(&uvI2C,0,0);
  Si114x_ConfigureUV(hrmConfig != BIOMETRIC_EXP);


  performMeasurements(&rhData, &tempData, &uvData, &vBat);


  /* Configure Si114x interrupt gpio input. */
  configSi114xIntInput ();
  reinitHRM = true;

  while (1)
  {
    /* There are two display mode. HRM-SpO2 and RH/T/UV modes.
     * HRM-SpO2 requires an accurate clock and so cannot use EM2
     * when HRM-SpO2 measurement is active. The HRM-SpO2 state
     * machine is implemented in heartratemonitor.c.
     */
    if ((displayType == LCD_HRM) || (displayType == LCD_SPO2))
    {
      if (reinitHRM)
      {
        /* If HRM-SpO2 was not previously displayed (ie we have changed
         * modes then perform some initialization.
         */

    	if (hrmConfig == BIOMETRIC_EXP)
    	  Si114x_EnableVisRange(0);  //HRM requires VIS_RANGE = 0

        HeartRateMonitor_Loop(false, true, false); /*shut down HRM*/
        reinitHRM = false;
        Si114xInit(&hrmI2C,0,0);
        if (displayType == LCD_HRM)
          GRAPHICS_ShowHRMStatus(false, 0, false);
        else
           GRAPHICS_ShowSpO2Status(false, 0, false);
        hrmCheckSkinContact = true;
        RTCDRV_StopTimer(updateScrollTimerId);
        RTCDRV_StartTimer(updateScrollTimerId, rtcdrvTimerTypePeriodic,
                                          START_SCROLL_MS, updateScrollTextCallback, NULL);
      } //if (reinitHRM)
      if (updateMeasurement)
      {
        updateMeasurement = false;
        hrmCheckSkinContact = true;
        updateDisplay = false;

      }
      /* Enter HRM loop.  */
      hrmActive = HeartRateMonitor_Loop(displayType == LCD_SPO2, false, hrmCheckSkinContact);
      hrmCheckSkinContact = false;
      if (hrmActive == false)
      {
        /* we are not actively processing samples so we just periodically
         * check for skin contact.
         */
        if (updateScrollText)
        {
          /* scroll instructions to user across LCD*/
          updateScrollText = false;
          if (displayType == LCD_HRM)
            scrollStatus =  GRAPHICS_ShowHRMStatus(false, 0, true);
          else
            scrollStatus =  GRAPHICS_ShowSpO2Status(false, 0, true);
          if (scrollStatus == false)
          {
            RTCDRV_StartTimer(updateScrollTimerId, rtcdrvTimerTypePeriodic,
                            UPDATE_SCROLL_MS, updateScrollTextCallback, NULL);
          }
          else
          {
            RTCDRV_StartTimer(updateScrollTimerId, rtcdrvTimerTypePeriodic,
                                  START_SCROLL_MS, updateScrollTextCallback, NULL);
            if (displayType == LCD_HRM)
              GRAPHICS_ShowHRMStatus(false, 0, false);
            else
              GRAPHICS_ShowSpO2Status(false, 0, false);
          }
        }
        if ( (USBD_SafeToEnterEM2() && usbEnabled) || (usbEnabled == false))
          EMU_EnterEM2(true); /*must restore clocks for HRM*/
        else
          EMU_EnterEM1();
      }// if (hrmActive == false)
      else
      {
        if (HeartRateMonitor_SamplesPending () == false)
          EMU_EnterEM1();
      } // else (hrmActive == true)
    }
    else  
    {
      if((reinitHRM == false) && (hrmConfig == BIOMETRIC_EXP))
		//did we just enter UV/RH/T mode
        Si114x_EnableVisRange(1);   //UV requires VIS_RANGE = 1

      reinitHRM = true;
      /*shut down HRM*/
      HeartRateMonitor_Loop(false, true, false);
      /*initialize UV mode*/
      Si114xInit(&uvI2C,0,0);

      if (updateMeasurement)
      {
        performMeasurements(&rhData, &tempData, &uvData, &vBat);
        updateMeasurement = false;
        if (lowBatPrevious)
            lowBat = (vBat <= LOW_BATTERY_THRESHOLD);
        else
            lowBat = false;
        lowBatPrevious = (vBat <= LOW_BATTERY_THRESHOLD);
        GRAPHICS_SetBatteryStatus(lowBat);
      }
  
      if (updateDisplay)
      { 
        switch (displayType)
        {
          case LCD_TEMPC:
            GRAPHICS_DrawTemperatureC(tempData);
            break;
          case LCD_TEMPF:
            GRAPHICS_DrawTemperatureF(tempData);
            break;
          case LCD_UV:
            GRAPHICS_DrawUV(uvData);
            break;
          case LCD_RH:
            GRAPHICS_DrawHumidity(rhData);
            break;
          default:
            GRAPHICS_DrawError ();
        }
        updateDisplay = false;
        /* Reset timer for periodic update of the display */
        RTCDRV_StartTimer(periodicUpdateTimerId, rtcdrvTimerTypePeriodic, 
                          PERIODIC_UPDATE_MS, periodicUpdateCallback, NULL);
        
      }
      if ( (USBD_SafeToEnterEM2() && usbEnabled) || (usbEnabled == false))
        EMU_EnterEM2(true);
    }
  }
}

/**************************************************************************//**
 * @brief This function is called whenever we want to measure the supply v.
 *        It is reponsible for starting the ADC and reading the result.
 *****************************************************************************/
static uint32_t checkBattery (void)
{
  uint32_t vData;
  /* Sample ADC */
  adcConversionComplete = false;  
  ADC_Start(ADC0, adcStartSingle);
  while (!adcConversionComplete) EMU_EnterEM1();
  vData = ADC_DataSingleGet( ADC0 );
  return vData;
}

/**************************************************************************//**
 * @brief ADC Interrupt handler (ADC0)
 *****************************************************************************/
void ADC0_IRQHandler( void ) {
  
   uint32_t flags;
  
   /* Clear interrupt flags */
   flags = ADC_IntGet( ADC0 );
   ADC_IntClear( ADC0, flags );
  
   adcConversionComplete = true;
   
}

/**************************************************************************//**
 * @brief ADC Initialization
 *****************************************************************************/
static void adcInit( void ) {
  
   ADC_Init_TypeDef       init       = ADC_INIT_DEFAULT;
   ADC_InitSingle_TypeDef initSingle = ADC_INITSINGLE_DEFAULT;
   
   /* Enable ADC clock */
   CMU_ClockEnable( cmuClock_ADC0, true );
   
   /* Initiate ADC peripheral */
   ADC_Init(ADC0, &init);
   
   /* Setup single conversions for internal VDD/3 */
   initSingle.acqTime = adcAcqTime16;
   initSingle.input   = adcSingleInpVDDDiv3;
   ADC_InitSingle( ADC0, &initSingle ); 
   
   /* Manually set some calibration values */
   ADC0->CAL = (0x7C << _ADC_CAL_SINGLEOFFSET_SHIFT) | (0x1F << _ADC_CAL_SINGLEGAIN_SHIFT);
   
   /* Enable interrupt on completed conversion */
   ADC_IntEnable(ADC0, ADC_IEN_SINGLE);
   NVIC_ClearPendingIRQ( ADC0_IRQn );
   NVIC_EnableIRQ( ADC0_IRQn );
}


static void configSi114xIntInput ()
{
  /* Configure PD5 as input and enable interrupt - si114x interrupt. */
  /* Interrupt is active low */
  GPIO_PinModeSet(gpioPortD, 5, gpioModeInputPull, 1);
  GPIO_IntConfig(gpioPortD, 5, false, true, true);
}

/**************************************************************************//**
 * @brief Setup GPIO interrupt for pushbuttons.
 *****************************************************************************/
static void gpioSetup(void)
{
  /* Enable GPIO clock. */
  CMU_ClockEnable(cmuClock_GPIO, true);

  /* Configure PB9 as input and enable interrupt. (PB0) */
  GPIO_PinModeSet(gpioPortB, 9, gpioModeInputPull, 1);
  GPIO_IntConfig(gpioPortB, 9, false, true, true);

  /* Configure PB10 as input and enable interrupt. (PB1) */
  GPIO_PinModeSet(gpioPortB, 10, gpioModeInputPull, 1);
  GPIO_IntConfig(gpioPortB, 10, false, true, true);

  NVIC_ClearPendingIRQ(GPIO_ODD_IRQn);
  NVIC_EnableIRQ(GPIO_ODD_IRQn);
  
  NVIC_ClearPendingIRQ(GPIO_EVEN_IRQn);
  NVIC_EnableIRQ(GPIO_EVEN_IRQn);

  /* Configure PD4 as pushpull. (5v enable signal) */
  GPIO_PinModeSet(gpioPortD, 4, gpioModePushPull, 0);
  GPIO_PinOutClear(gpioPortD, 4);
  
  /* Configure PC3 as pushpull. (Green LED) */
  GPIO_PinModeSet(gpioPortC, 3, gpioModeWiredAndPullUp, 0);
  GPIO_PinOutSet(gpioPortC, 3);
  
  /* Configure PC0 as pushpull. (Red LED) */
  GPIO_PinModeSet(gpioPortC, 0, gpioModeWiredAndPullUp, 0);
  GPIO_PinOutSet(gpioPortC, 0);

  /*si114x interrupt. we want this pin high while si114x starts up*/
  GPIO_PinModeSet(gpioPortD, 5, gpioModePushPull, 1);
  

  /* Configure PE2 as pushpull. (STK LED) */
  GPIO_PinModeSet(gpioPortE, 2, gpioModePushPull, 0);
  GPIO_PinOutClear(gpioPortE, 2);

  /* Configure PE3 as pushpull. (STK LED) */
  GPIO_PinModeSet(gpioPortE, 3, gpioModePushPull, 0);
  GPIO_PinOutClear(gpioPortE, 3);
}

/**************************************************************************//**
 * @brief GPIO Interrupt handler (PB1)
 *****************************************************************************/
void GPIO_EVEN_IRQHandler(void)
{
  /* Acknowledge interrupt */
  GPIO_IntClear(1 << 10);

  /* Pushbutton IRQ */
  if (displayType == LCD_UV)
    displayType = LCD_HRM;
  else
    displayType++;
  if ((displayType == LCD_SPO2) && (hrmConfig != BIOMETRIC_EXP) && (hrmConfig != FITNESS_EVB_GRI))
    displayType = LCD_SPO2+1;
  if ((displayType == LCD_SPO2) || (displayType == LCD_HRM))
    reinitHRM = true;

  updateDisplay = true;
}

/**************************************************************************//**
 * @brief GPIO Interrupt handler (PB0,Si1147 INT)
 *****************************************************************************/
void GPIO_ODD_IRQHandler(void)
{
  uint32_t flags;
  flags = GPIO_IntGet();
  if (flags & (1 << 9))
  {

    /* Pushbutton IRQ */
    if (displayType == LCD_HRM)
      displayType = LCD_UV;
    else
      displayType--;
    if ((displayType == LCD_SPO2) && (hrmConfig != BIOMETRIC_EXP) && (hrmConfig != FITNESS_EVB_GRI))
      displayType = LCD_SPO2-1;
    if ((displayType == LCD_SPO2) || (displayType == LCD_HRM))
      reinitHRM = true;

    updateDisplay = true;
   
  }
  if (flags & (1 << 5))
  {
    /* Gesture IRQ line */
    HeartRateMonitor_Interrupt();
  }
  GPIO_IntClear(flags);
}

/**************************************************************************//**
 * @brief   The actual callback for Memory LCD toggling
 * @param[in] id
 *   The id of the RTC timer (not used)
 *****************************************************************************/
static void memLcdCallback(RTCDRV_TimerID_t id, void *user)
{
  (void) id;
  (void) user;
  rtcCallback(rtcCallbackArg);
}

/**************************************************************************//**
 * @brief   Register a callback function at the given frequency.
 *
 * @param[in] pFunction  Pointer to function that should be called at the
 *                       given frequency.
 * @param[in] argument   Argument to be given to the function.
 * @param[in] frequency  Frequency at which to call function at.
 *
 * @return  0 for successful or
 *         -1 if the requested frequency does not match the RTC frequency.
 *****************************************************************************/
int RtcIntCallbackRegister(void (*pFunction)(void*),
                           void* argument,
                           unsigned int frequency)
{
  RTCDRV_TimerID_t timerId;
  rtcCallback    = pFunction;
  rtcCallbackArg = argument;

  RTCDRV_AllocateTimer(&timerId);

  RTCDRV_StartTimer(timerId, rtcdrvTimerTypePeriodic, 1000 / frequency, 
                    memLcdCallback, NULL);

  return 0;
}

/**************************************************************************//**
 * @brief Callback used to count between measurement updates
 *****************************************************************************/
static void periodicUpdateCallback(RTCDRV_TimerID_t id, void *user)
{
  (void) id;
  (void) user;
  updateDisplay = true;
  updateMeasurement = true;
}

/**************************************************************************//**
 * @brief Callback used to count between scrolling updates
 *****************************************************************************/
static void updateScrollTextCallback(RTCDRV_TimerID_t id, void *user)
{
  (void) id;
  (void) user;
  updateScrollText = true;
}

/************************************************************
 *      @brief  SYSTICK interrupt handler
 *
 ************************************************************/
void SysTick_Handler( void )
{
  HeartRateMonitor_TimerEventHandler();
}

