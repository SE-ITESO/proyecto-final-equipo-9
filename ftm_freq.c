/*
 * @file     ftm_freq.c
 *
 * @Authors  Juan Pablo Villanueva
 *           Jose Angel Gonzalez
 *
 * @brief    This source file contains the functions necessary to
 *           use FTM in input capture mode, to measure frequencies
 */

#include "ftm_freq.h"

/*
 * ******************************************************************
 * Global variables:
 * ******************************************************************
 */

volatile bool input_cap_flag = false;
volatile uint32_t g_first_captureVal = 0;
volatile uint32_t g_second_captureVal = 0;
volatile uint32_t signal_freq = 0;
volatile uint32_t g_counter = 0;
volatile uint32_t g_overflow_count = 0;


/*
 * ******************************************************************
 * Static function prototypes:
 * ******************************************************************
 */


/*
 * @brief: Function used as a callback for the FTM
 *         when a capture event has occurred
 */
void FTM_INPUT_CAPTURE_HANDLER(void)
{
    g_counter++;

    /*second signal rising edge detected*/
    if(g_counter == 2)
    {
    	FTM_StopTimer(BOARD_FTM_BASEADDR);
    	g_second_captureVal = FTM_GetInputCaptureValue(BOARD_FTM_BASEADDR, FTM_INPUT_CAPTURE_CHANNEL);
    	input_cap_flag = true;
    }

    /*first signal rising edge detected*/
    else if(g_counter == 1)
    {
    	g_first_captureVal = FTM_GetInputCaptureValue(BOARD_FTM_BASEADDR, FTM_INPUT_CAPTURE_CHANNEL);
    }

    FTM_ClearStatusFlags(BOARD_FTM_BASEADDR, FTM_CHANNEL_FLAG);
    __DSB();
}


/*
 * @brief: Does the necessary configuration for the FTM to
 *         work in input capture mode, as well as the configuration for the
 *	       pin to be used as an input
 */
void ftm_freq_init(void)
{
    /*Pin config*/
    CLOCK_EnableClock(kCLOCK_PortC);
    PORT_SetPinMux(PORTC, 2u, kPORT_MuxAlt4);

    ftm_config_t ftmInfo;

    FTM_GetDefaultConfig(&ftmInfo);
    /* Initialize FTM module */
    FTM_Init(BOARD_FTM_BASEADDR, &ftmInfo);

    /* Setup dual-edge capture on a FTM channel pair */
    FTM_SetupInputCapture(BOARD_FTM_BASEADDR, FTM_INPUT_CAPTURE_CHANNEL, kFTM_RisingEdge, 0);

    /* Set the timer to be in free-running mode */
    FTM_SetTimerPeriod(BOARD_FTM_BASEADDR, FTM_COUNT);

    /* Enable channel interrupt when the second edge is detected */
    FTM_EnableInterrupts(BOARD_FTM_BASEADDR, FTM_CHANNEL_INTERRUPT_ENABLE);

    /* Enable at the NVIC */
    EnableIRQ(FTM_INTERRUPT_NUMBER);

    FTM_StartTimer(BOARD_FTM_BASEADDR, kFTM_SystemClock);
}


/*
 * @brief: Calculates the actual frequency of the external signal
 */

uint32_t ftm_freq_get_freq(void)
{
	float signal_period = 0;
	float ftm_period = 0;

	ftm_period = 1.0 / (21000000 / 2);

	if((FTM_GetStatusFlags(BOARD_FTM_BASEADDR) & FTM_OVERFLOW_FLAG) == FTM_OVERFLOW_FLAG)
	{
		g_overflow_count++;
		FTM_ClearStatusFlags(BOARD_FTM_BASEADDR, FTM_OVERFLOW_FLAG);
	}

	if(true == input_cap_flag)
	{

		if(g_first_captureVal > g_second_captureVal)
		{

			if(g_overflow_count)
			{
				g_second_captureVal = g_second_captureVal + (g_overflow_count * FTM_COUNT);
			}

			signal_period = (g_second_captureVal - g_first_captureVal) * ftm_period;
		}
		else
		{

			if(g_overflow_count)
			{
				g_second_captureVal =  g_second_captureVal + (g_overflow_count * FTM_COUNT);
			}

			signal_period = (g_second_captureVal - g_first_captureVal) * ftm_period;
		}

		signal_freq = (1 / signal_period) * 2;
		input_cap_flag = false;
	}
	return signal_freq;
}
