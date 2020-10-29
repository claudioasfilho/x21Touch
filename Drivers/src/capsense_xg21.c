/***************************************************************************//**
 * @file
 * @brief xg21 capacitive sense driver
 *******************************************************************************
 * # License
 * <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 *******************************************************************************
 * # Experimental Quality
 * This code has not been formally tested and is provided as-is. It is not
 * suitable for production environments. In addition, this code will not be
 * maintained and there may be no bug maintenance planned for these resources.
 * Silicon Labs may update projects from time to time.
 ******************************************************************************/

#include "em_device.h"
#include "em_acmp.h"
#include "em_cmu.h"
#include "em_emu.h"
#include "em_prs.h"
#include "em_timer.h"
#include "capsense.h"

/***************************************************************************//**
 * @addtogroup kitdrv
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup CapSense
 * @brief Capacitive sensing driver
 *
 * @details
 *  Capacitive sensing driver using TIMER and ACMP peripherals.
 *
 * @{
 ******************************************************************************/

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */

/** The current channel we are sensing. */
static volatile uint8_t currentChannel;
/** Flag for measurement completion. */
static volatile bool measurementComplete;

#if defined(CAPSENSE_CH_IN_USE)
/**************************************************************************//**
 * @brief
 *   A bit vector which represents the channels to iterate through
 *
 * @note
 *   This API is deprecated and new application should define
 *   CAPSENSE_CHANNELS instead of CAPSENSE_CH_IN_USE.
 *
 * @param ACMP_CHANNELS
 *   Vector of channels.
 *****************************************************************************/
static const bool channelsInUse[ACMP_CHANNELS] = CAPSENSE_CH_IN_USE;
#elif defined(CAPSENSE_CHANNELS)
/**************************************************************************//**
 * @brief
 *   An array of channels that the capsense driver should iterate through.
 *
 * @param CAPSENSE_CHANNELS
 *   Initializer list that contains all the channels that the application
 *   would like to use for capsense.
 *
 * @param ACMP_CHANNELS
 *   The number of channels in the array
 *****************************************************************************/
static const ACMP_Channel_TypeDef channelList[ACMP_CHANNELS] = CAPSENSE_CHANNELS;
#endif

/**************************************************************************//**
 * @brief The NUM_SLIDER_CHANNELS specifies how many of the ACMP_CHANNELS
 *        are used for a touch slider
 *****************************************************************************/
#if !defined(NUM_SLIDER_CHANNELS)
#define NUM_SLIDER_CHANNELS 4
#endif

/**************************************************************************//**
 * @brief This vector stores the latest read values from the ACMP
 * @param ACMP_CHANNELS Vector of channels.
 *****************************************************************************/
static volatile uint32_t channelValues[ACMP_CHANNELS] = { 0 };

/**************************************************************************//**
 * @brief  This stores the maximum values seen by a channel
 * @param ACMP_CHANNELS Vector of channels.
 *****************************************************************************/
static volatile uint32_t channelMaxValues[ACMP_CHANNELS] = { 0 };

/** @endcond */

/**************************************************************************//**
 * @brief
 *   TIMER0 interrupt handler.
 *
 * @details
 *   When TIMER0 expires the number of pulses on TIMER1 is inserted into
 *   channelValues. If this values is bigger than what is recorded in
 *   channelMaxValues, channelMaxValues is updated.
 *   Finally, the next ACMP channel is selected.
 *****************************************************************************/
void TIMER0_IRQHandler(void)
{
  uint32_t count;

  // Stop timers
  TIMER_Enable(TIMER0, false);
  TIMER_Enable(TIMER1, false);

  // Clear interrupt flag
  TIMER_IntClear(TIMER0, TIMER_IF_OF);

  // Save TIMER1 count
  count = TIMER_CounterGet(TIMER1);

  // Store value in channelValues
  channelValues[currentChannel] = count;

  // Update channelMaxValues
  if (count > channelMaxValues[currentChannel])
    channelMaxValues[currentChannel] = count;

  measurementComplete = true;
}

/**************************************************************************//**
 * @brief Get the current channelValue for a channel
 * @param channel The channel.
 * @return The channelValue.
 *****************************************************************************/
uint32_t CAPSENSE_getVal(uint8_t channel)
{
  return channelValues[channel];
}

/**************************************************************************//**
 * @brief Get the current normalized channelValue for a channel
 * @param channel The channel.
 * @return The channel value in range (0-256).
 *****************************************************************************/
uint32_t CAPSENSE_getNormalizedVal(uint8_t channel)
{
  uint32_t max = channelMaxValues[channel];
  return (channelValues[channel] << 8) / max;
}

/**************************************************************************//**
 * @brief Get the state of the Gecko Button
 * @param channel The channel.
 * @return true if the button is "pressed"
 *         false otherwise.
 *****************************************************************************/
bool CAPSENSE_getPressed(uint8_t channel)
{
  uint32_t treshold;
  /* Treshold is set to 12.5% below the maximum value */
  /* This calculation is performed in two steps because channelMaxValues is
   * volatile. */
  treshold  = channelMaxValues[channel];
  treshold -= channelMaxValues[channel] >> 2;

  if (channelValues[channel] < treshold) {
    return true;
  }
  return false;
}

/**************************************************************************//**
 * @brief Get the position of the slider
 * @return The position of the slider if it can be determined,
 *         -1 otherwise.
 *****************************************************************************/
int32_t CAPSENSE_getSliderPosition(void)
{
  int      i;
  int      minPos = -1;
  uint32_t minVal = 224; /* 0.875 * 256 */
  /* Values used for interpolation. There is two more which represents the edges.
   * This makes the interpolation code a bit cleaner as we do not have to make special
   * cases for handling them */
  uint32_t interpol[(NUM_SLIDER_CHANNELS + 2)];
  for (i = 0; i < (NUM_SLIDER_CHANNELS + 2); i++) {
    interpol[i] = 255;
  }

  /* The calculated slider position. */
  int position;

  /* Iterate through the slider bars and calculate the current value divided by
   * the maximum value multiplied by 256.
   * Note that there is an offset of 1 between channelValues and interpol.
   * This is done to make interpolation easier.
   */
  for (i = 1; i < (NUM_SLIDER_CHANNELS + 1); i++) {
    /* interpol[i] will be in the range 0-256 depending on channelMax */
    interpol[i]  = channelValues[i - 1] << 8;
    interpol[i] /= channelMaxValues[i - 1];
    /* Find the minimum value and position */
    if (interpol[i] < minVal) {
      minVal = interpol[i];
      minPos = i;
    }
  }
  /* Check if the slider has not been touched */
  if (minPos == -1) {
    return -1;
  }

  /* Start position. Shift by 4 to get additional resolution. */
  /* Because of the interpol trick earlier we have to substract one to offset that effect */
  position = (minPos - 1) << 4;

  /* Interpolate with pad to the left */
  position -= ((256 - interpol[minPos - 1]) << 3)
              / (256 - interpol[minPos]);

  /* Interpolate with pad to the right */
  position += ((256 - interpol[minPos + 1]) << 3)
              / (256 - interpol[minPos]);

  return position;
}

/**************************************************************************//**
 * @brief
 *   Start a capsense measurement of a specific channel and wait for
 *   it to complete.
 *****************************************************************************/
static void CAPSENSE_Measure(ACMP_Channel_TypeDef channel)
{
  // Set up the specified channel
  ACMP_CapsenseChannelSet(ACMP0, channel);

  // Reset timers
  TIMER_CounterSet(TIMER0, 0);
  TIMER_CounterSet(TIMER1, 0);

  measurementComplete = false;

  TIMER_IntClear(TIMER0, TIMER_IEN_OF);

  // Start timers
  TIMER_Enable(TIMER0, true);
  TIMER_Enable(TIMER1, true);

  // Wait for measurement to complete
  while ( measurementComplete == false );
    {
	  //EMU_EnterEM1();
    }

}

/**************************************************************************//**
 * @brief
 *   This function iterates through all the capsense channels and
 *   initiates a reading. Uses EM1 while waiting for the result from
 *   each sensor.
 *****************************************************************************/
void CAPSENSE_Sense(void)
{
  // Use the default STK capacative sensing setup and enable it
  ACMP_Enable(ACMP0);

#if defined(CAPSENSE_CHANNELS)
  /* Iterate through only the channels in the channelList */
  for (currentChannel = 0; currentChannel < ACMP_CHANNELS; currentChannel++)
    CAPSENSE_Measure(channelList[currentChannel]);
#else
  /* Iterate through all channels and check which channel is in use */
  for (currentChannel = 0; currentChannel < ACMP_CHANNELS; currentChannel++) {
    /* If this channel is not in use, skip to the next one */
    if (!channelsInUse[currentChannel]) {
      continue;
    }

    CAPSENSE_Measure((ACMP_Channel_TypeDef) currentChannel);
  }
#endif

  /* Disable ACMP while not sensing to reduce power consumption */
//  ACMP_Disable(ACMP0);
}

/**************************************************************************//**
 * @brief
 *   Initializes the ACMP and TIMER capacitive sensing.
 *
 * @details
 *   ACMP is set up in capsense (oscillator mode).
 *   TIMER1 counts the number of pulses generated by ACMP0.
 *   When TIMER0 expires an interruptis requested.
 *   The number of pulses counted by TIMER1 is stored in channelValues
 *****************************************************************************/
void CAPSENSE_Init(void)
{
	// Use the default STK capacative sensing setup
	ACMP_CapsenseInit_TypeDef capsenseInit = ACMP_CAPSENSE_INIT_DEFAULT;


	/* Enable TIMER0, TIMER1, ACMP_CAPSENSE and PRS clock */
	CMU_ClockEnable(cmuClock_ACMP0, true);
	CMU_ClockEnable(cmuClock_TIMER0, true);
	CMU_ClockEnable(cmuClock_TIMER1, true);

	CMU_ClockEnable(cmuClock_PRS, true);

	CMU_ClockEnable(cmuClock_GPIO, true);


	// Initialize TIMER0 but do not run yet
	TIMER_Init_TypeDef timer0_init = TIMER_INIT_DEFAULT;
	timer0_init.prescale = timerPrescale512;
	timer0_init.enable = false;
	TIMER_Init(TIMER0, &timer0_init);

	// Set TIMER0 top value to 10 and in
	TIMER_TopSet(TIMER0, 10);

	// Enable TIMER0 overflow interrupt
	TIMER_IntEnable(TIMER0, TIMER_IEN_OF);

	// Initialize TIMER1 but do not run yet
	TIMER_Init_TypeDef timer1_init = TIMER_INIT_DEFAULT;
	timer1_init.prescale = timerPrescale1024;
	timer1_init.clkSel = timerClkSelCC1;
	timer1_init.enable = false;
	TIMER_Init(TIMER0, &timer1_init);

	// Set TIMER1 top value to 0xFFFF
	TIMER_TopSet(0, 0xFFFF);

	// Set up TIMER1 CC1 to capture on PRS channel 0 input
	TIMER_InitCC_TypeDef cc1_init = TIMER_INITCC_DEFAULT;
	cc1_init.edge         = timerEdgeBoth;
	cc1_init.mode         = timerCCModeCapture;
	cc1_init.eventCtrl    = timerEventRising;
	cc1_init.prsInput     = true;
	cc1_init.prsInputType = timerPrsInputSync;
	cc1_init.prsSel       = 0;
	TIMER_InitCC(TIMER1, 1, &cc1_init);


	//PRS_SourceSignalSet(0,PRS_ASYNC_CH_CTRL_SOURCESEL_ACMP0,PRS_ASYNC_CH_CTRL_SIGSEL_ACMP0OUT,prsEdgePos);

	// Route the ACMP output using synchronous PRS channel 0
	PRS_ConnectSignal(0, prsTypeSync, prsSignalACMP0_OUT);


	// Set up ACMP1 in capsense mode
	ACMP_CapsenseInit(ACMP0, &capsenseInit);

	// Route the ACMP out to a pin for debugging purposes


	//PC3 - EXP Header 10

	// Assign the port C/D even pin analog inputs to CD ABUS 0
	GPIO->CDBUSALLOC |= GPIO_CDBUSALLOC_CDODD0_ACMP0;
	//GPIO->CDBUSALLOC |= GPIO_CDBUSALLOC_CDEVEN0_ACMP0;
	GPIO_PinModeSet(DEBUG_ACMP0OUT_PORT, DEBUG_ACMP0OUT_PIN, gpioModePushPull, 0);
	GPIO->ACMPROUTE[0].ACMPOUTROUTE = (DEBUG_ACMP0OUT_PORT << _GPIO_ACMP_ACMPOUTROUTE_PORT_SHIFT) | (DEBUG_ACMP0OUT_PIN << _GPIO_ACMP_ACMPOUTROUTE_PIN_SHIFT);
	GPIO->ACMPROUTE[0].ROUTEEN = 1;

	//TIMER_Enable(TIMER0, true);
	//TIMER_Enable(TIMER1, true);

	// Enable TIMER0 interrupt
	NVIC_EnableIRQ(TIMER0_IRQn);
}

/** @} (end group CapSense) */
/** @} (end group kitdrv) */
