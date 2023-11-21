/*
 * @file     main.c
 *
 * @Authors  Juan Pablo Villanueva
 *           Jose Angel Gonzalez
 *
 * @brief    Application entry point.
 */

// TODO: Add application drivers

#include "ftm_freq.h"
#include "fsl_ftm.h"
#include "fsl_clock.h"
#include "fsl_port.h"
#include "fsl_pit.h"
#include "fsl_gpio.h"
#include "NVIC.h"
#include "PIT.h"

void square_signal(void)
{
	GPIO_PortToggle(GPIOD, 1);
}

volatile uint32_t frecuencia = 0;
int main(void)
{
	NVIC_global_enable_interrupts;


	gpio_pin_config_t gpio_output_config = {
	        kGPIO_DigitalOutput,
	        1
	    };
	CLOCK_EnableClock(kCLOCK_PortD);
	PORT_SetPinMux(PORTD, 0, kPORT_MuxAsGpio);
	GPIO_PinInit(GPIOD, 0, &gpio_output_config);


	pit_config_t pit_config;
	PIT_GetDefaultConfig(&pit_config);
	PIT_Init(PIT, &pit_config);
	PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, USEC_TO_COUNT(5000,21000000));

	// PIT interrupt config:
	PIT_EnableInterrupts(PIT, kPIT_Chnl_0, kPIT_TimerInterruptEnable);
	PIT_callback_init(kPIT_Chnl_0, square_signal);
	NVIC_enable_interrupt_and_priotity(PIT_CH0_IRQ, PRIORITY_1);

	PIT_StartTimer(PIT, kPIT_Chnl_0);

	ftm_freq_init();

	// TODO: execute initialization functions

	while (1)
	{
		// TODO: execute periodic functions
		frecuencia = ftm_freq_get_freq();
	}
}
