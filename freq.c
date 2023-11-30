/*
 * freq.c
 *
 *  Created on: 29 nov 2023
 *      Author: JVillanueva
 */

#include "freq.h"

static void no_gpio_pit_callback(void);

uint8_t g_flag = 0;
uint32_t g_pit_period = 0;

void capture_values(uint32_t flags)
{
	if(g_flag == 0)
	{
		PIT_StartTimer(PIT, kPIT_Chnl_3);
		g_flag = 1;
	}
	else
	{
		g_pit_period = USEC_TO_COUNT(5000000U,21000000U) - PIT_GetCurrentTimerCount(PIT, kPIT_Chnl_3);
		PIT_StopTimer(PIT, kPIT_Chnl_3);
		PIT_StartTimer(PIT, kPIT_Chnl_3);
	}
}

void init_freq(void)
{
	// PIT config:
	PIT_SetTimerPeriod(PIT, kPIT_Chnl_3, USEC_TO_COUNT(5000000U,21000000U));
	PIT_EnableInterrupts(PIT, kPIT_Chnl_3, kPIT_TimerInterruptEnable);
	PIT_callback_init(kPIT_Chnl_3, no_gpio_pit_callback);
	NVIC_enable_interrupt_and_priotity(PIT_CH3_IRQ, PRIORITY_4);

	CLOCK_EnableClock(kCLOCK_PortC);
	const port_pin_config_t input_config = {
			kPORT_PullUp,
			kPORT_SlowSlewRate,
			kPORT_PassiveFilterEnable,
			kPORT_OpenDrainDisable,
			kPORT_LowDriveStrength,
			kPORT_MuxAsGpio,
			kPORT_UnlockRegister
		  };

	gpio_pin_config_t gpio_input_config = {
			        kGPIO_DigitalInput,
			        0
			    };
	PORT_SetPinConfig(PORTC, 2u, &input_config);
	GPIO_PinInit(GPIOC, 2u, &gpio_input_config);
	PORT_SetPinInterruptConfig(PORTC, 2u, kPORT_InterruptFallingEdge);
	GPIO_callback_init(GPIO_C, capture_values);

	NVIC_enable_interrupt_and_priotity(PORTC_IRQ, PRIORITY_2);

}

float freq_get_freq(void)
{
	float frequency = 0.0f;
	if (g_pit_period)
	{
		frequency = 10500000.0f / g_pit_period;
	}

	return frequency;
}

static void no_gpio_pit_callback(void)
{
	PIT_StopTimer(PIT, kPIT_Chnl_3);
	g_pit_period = 0;
	g_flag = 0;
}
