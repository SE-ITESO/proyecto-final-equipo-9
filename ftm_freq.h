/*
 * @file     ftm_freq.c
 *
 * @Authors  Juan Pablo Villanueva
 *           Jose Angel Gonzalez
 *
 * @brief    This is the header file used for using FTM in input
 *           capture mode.
 */

#ifndef FTM_FREQ_H_
#define FTM_FREQ_H_

#include "fsl_ftm.h"
#include "fsl_clock.h"
#include "fsl_port.h"
#include "fsl_pit.h"
#include "fsl_gpio.h"
#include "NVIC.h"
#include "PIT.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define BOARD_FTM_BASEADDR FTM0

/* FTM channel used for input capture */
#define FTM_INPUT_CAPTURE_CHANNEL    kFTM_Chnl_1

/* Interrupt number and interrupt handler for the FTM base address used */
#define FTM_INTERRUPT_NUMBER         FTM0_IRQn
#define FTM_INPUT_CAPTURE_HANDLER    FTM0_IRQHandler

/* Interrupt to enable and flag to read */
#define FTM_CHANNEL_INTERRUPT_ENABLE kFTM_Chnl1InterruptEnable
#define FTM_CHANNEL_FLAG             kFTM_Chnl1Flag
#define FTM_OVERFLOW_FLAG            kFTM_TimeOverflowFlag

/* Get source clock for FTM driver */
#define FTM_SOURCE_CLOCK             CLOCK_GetFreq(kCLOCK_BusClk)

#define FTM_COUNT 					 0xFFFF
/*
 * ******************************************************************
 * Function prototypes:
 * ******************************************************************
 */

/*
 * @brief: Does the necessary configuration for the FTM to
 *         work in input capture mode, as well as the configuration for the
 *	       pin to be used as an input
 */
void ftm_freq_init(void);

/*
 * @brief: Calculates the actual frequency of the external signal
 */

uint32_t ftm_freq_get_freq(void);

#endif /* FTM_FREQ_H_ */
