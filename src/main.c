/***************************************************************************//**
 * @file
 * @brief Capacitive touch example for EFR32xG21
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
#include "em_chip.h"
#include "em_cmu.h"
#include "em_gpio.h"

#include "capsense.h"

#include "bsp.h"

// Count 1 ms ticks
static volatile uint32_t msTicks;

/***************************************************************************//**
 * @brief
 *   SysTick_Handler. Interrupt Service Routine for system tick counter
 ******************************************************************************/
void SysTick_Handler(void)
{
  msTicks++;
}

/***************************************************************************//**
 * @brief
 *   Delays number of msTick Systicks (typically 1 ms)
 *
 * @param dlyTicks
 *   Number of ticks (ms) to delay
 ******************************************************************************/
static void Delay(uint32_t dlyTicks)
{
  uint32_t curTicks;

  curTicks = msTicks;
  while ((msTicks - curTicks) < dlyTicks);
}

/***************************************************************************//**
 * @brief
 *   Main function
 ******************************************************************************/
int main(void)
{
  CHIP_Init();

  // Setup SysTick 1 ms interrupts
  if (SysTick_Config(SystemCoreClockGet() / 1000))
    while (1) ;

  // Initialize STK LEDs
  BSP_LedsInit();

  // Start capacitive sense buttons
  CAPSENSE_Init();

  //BSP_LedSet(0);

  while (1)
  {
    Delay(100);

    CAPSENSE_Sense();

    // Turn on LED0 if capsense button 0 pressed; otherwise turn it off
    if (CAPSENSE_getPressed(BUTTON0_CHANNEL))
      BSP_LedSet(0);
    else
      BSP_LedClear(0);

    // Turn on LED1 if capsense button 1 pressed; otherwise turn it off
    if (CAPSENSE_getPressed(BUTTON1_CHANNEL))
      BSP_LedSet(1);
    else
      BSP_LedClear(1);
  }
}
