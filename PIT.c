/*
 * @file     PIT.c
 *
 * @Authors  Juan Pablo Villanueva
 *           Jose Angel Gonzalez
 *
 * @brief    This source file contains the functions necessary to
 *           initialize PIT callbacks, ISRs, and interrupt software
 *           flag getters and setters.
 */

#include "PIT.h"

/*
 * ******************************************************************
 * ******************************************************************
 * Variables:
 * ******************************************************************
 * ******************************************************************
 */

// Bit field where the software interrupt flags are recorded.
volatile static pit_interrupt_flags_t g_intr_status_flag = {0};

// Interrupt callback functions.
static void (*PIT0_callback)(void) = 0;
static void (*PIT1_callback)(void) = 0;
static void (*PIT2_callback)(void) = 0;
static void (*PIT3_callback)(void) = 0;

/*
 * ******************************************************************
 * ******************************************************************
 * Function code:
 * ******************************************************************
 * ******************************************************************
 */

/*!
 * brief Sets the callback function to be executed when the specified
 *       PIT channel generates an interruption.
 *
 * param pit_channel PIT channel for whose interruptions the callback
 *                   function will be executed.
 * param handler     Function that will serve the role of a callback
 */
void PIT_callback_init(pit_chnl_t pit_channel, void (*handler)(void))
{
	switch (pit_channel)
	{
	    case kPIT_Chnl_0:
	    	PIT0_callback = handler;
	    break;
	    case kPIT_Chnl_1:
	    	PIT1_callback = handler;
	    break;
	    case kPIT_Chnl_2:
	    	PIT2_callback = handler;
	    break;
	    case kPIT_Chnl_3:
	    	PIT3_callback = handler;
	    break;
	    default:
	    break;

	}
}


/*!
 * brief Returns the software interrupt flag of a PIT channel. Used
 *       when attending interrupts via getters and setters (when no
 *       interrupt callback has been initialized). Return value is
 *       either a 1 or a 0.
 *
 * param pit_channel Flag to be read (PIT channel from 0 to 3).
 */
uint8_t PIT_get_interrupt_status(pit_chnl_t pit_channel)
{
    uint8_t status = 0;
    switch (pit_channel)
    {
        case kPIT_Chnl_0:
        	status = g_intr_status_flag.flag_chnl_0;
        break;
        case kPIT_Chnl_1:
        	status = g_intr_status_flag.flag_chnl_1;
        break;
        case kPIT_Chnl_2:
        	status = g_intr_status_flag.flag_chnl_2;
        break;
        case kPIT_Chnl_3:
        	status = g_intr_status_flag.flag_chnl_3;
        break;
        default:
       	break;
   	}
    return status;
}


/*!
 * brief Clears the software interrupt flag of a PIT channel. Used
 *       when attending interrupts via getters and setters (when no
 *       interrupt callback has been initialized).
 *
 * param pit_channel Flag to be cleared (PIT channel from 0 to 3).
 */
void PIT_clear_interrupt_status(pit_chnl_t pit_channel)
{
	switch (pit_channel)
	{
	    case kPIT_Chnl_0:
	    	g_intr_status_flag.flag_chnl_0 = false;
	    break;
	    case kPIT_Chnl_1:
	    	g_intr_status_flag.flag_chnl_1 = false;
	    break;
	    case kPIT_Chnl_2:
	    	g_intr_status_flag.flag_chnl_2 = false;
	    break;
	    case kPIT_Chnl_3:
	       	g_intr_status_flag.flag_chnl_3 = false;
	    break;
	    default:
	   	break;

	}
}


/*
 * ******************************************************************
 * ******************************************************************
 * Interrupt handlers:
 * ******************************************************************
 * ******************************************************************
 */

/*
 * All handlers follow the same logic: if a callback functions has
 * been initialized, it is executed. Otherwise, the software interrupt
 * flags are set, and it's up to the user to read them.
 */

void PIT0_IRQHandler(void)
{
	PIT_ClearStatusFlags(PIT, kPIT_Chnl_0, kPIT_TimerFlag);
	if (PIT0_callback)
	{
	    PIT0_callback();
	}
	else
	{
		g_intr_status_flag.flag_chnl_0 = true;
	}
}

void PIT1_IRQHandler(void)
{
	PIT_ClearStatusFlags(PIT, kPIT_Chnl_1, kPIT_TimerFlag);
	if (PIT1_callback)
	{
	    PIT1_callback();
	}
	else
	{
		g_intr_status_flag.flag_chnl_1 = true;
	}
}

void PIT2_IRQHandler(void)
{
	PIT_ClearStatusFlags(PIT, kPIT_Chnl_2, kPIT_TimerFlag);
	if (PIT2_callback)
	{
		PIT2_callback();
	}
	else
	{
		g_intr_status_flag.flag_chnl_2 = true;
	}
}

void PIT3_IRQHandler(void)
{
	PIT_ClearStatusFlags(PIT, kPIT_Chnl_3, kPIT_TimerFlag);
	if (PIT3_callback)
	{
	    PIT3_callback();
	}
	else
	{
		g_intr_status_flag.flag_chnl_3 = true;
	}
}
