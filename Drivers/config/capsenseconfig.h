/***************************************************************************//**
 * @file
 * @brief capsense configuration parameters
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

#ifndef __SILICON_LABS_CAPSENSCONFIG_H__
#define __SILICON_LABS_CAPSENSCONFIG_H__
#ifdef __cplusplus
extern "C" {
#endif

/* Pin  | ABUS Channel (for ACMP0)
 * -------------------------
 * PD0  | acmpInputPD0
 * PD1  | acmpInputPD1
 */
#define CAPSENSE_CHANNELS       { acmpInputPC1, acmpInputPC2 }
#define BUTTON0_CHANNEL         0             /**< Button 0 channel */
#define BUTTON1_CHANNEL         1             /**< Button 1 channel */
#define ACMP_CHANNELS           1             /**< Number of channels in use for capsense */
#define NUM_SLIDER_CHANNELS     0             /**< The kit does not have a slider */

#define DEBUG_ACMP0OUT_PORT     gpioPortC
#define DEBUG_ACMP0OUT_PIN      3

#ifdef __cplusplus
}
#endif
#endif /* __SILICON_LABS_CAPSENSCONFIG_H__ */
