/*
 * ftm_speed.c
 *
 *  Created on: 28 nov 2023
 *      Author: JVillanueva
 */

#include "ftm_speed.h"

short int g_dutyCycle = 0;

void ftm_speed_init(void)
{
	/** Clock gating for the FlexTimer 0*/
		SIM->SCGC6 |= FLEX_TIMER_0_CLOCK_GATING;
		/**When write protection is enabled (WPDIS = 0), write protected bits cannot be written.
		* When write protection is disabled (WPDIS = 1), write protected bits can be written.*/
		FTM0->MODE |= FLEX_TIMER_WPDIS;
		/**Enables the writing over all registers*/
		FTM0->MODE &= ~FLEX_TIMER_FTMEN;
		/**Assigning a default value for modulo register*/
		FTM0->MOD = 0x00FF;
		/**Selects the Edge-Aligned PWM mode mode*/
		FTM0->CONTROLS[0].CnSC = FLEX_TIMER_MSB | FLEX_TIMER_ELSB;
		/**Assign a duty cycle of 50%*/
		FTM0->CONTROLS[0].CnV = FTM0->MOD/2;
		/**Configure the times*/
		FTM0->SC = FLEX_TIMER_CLKS_1|FLEX_TIMER_PS_128;

		SIM->CLKDIV1 = 0x01240000U;

		CLOCK_EnableClock(kCLOCK_PortC);
		PORT_SetPinMux(PORTC, 1u, kPORT_MuxAlt4);
}

void FTM0_ISR()
{
	FTM0->SC &= ~FLEX_TIMER_TOF;
	GPIOD->PDOR ^= 0xFF;
}

void ftm_speed_chnnlVal(short int channelValue)
{

	/**Assigns a new value for the duty cycle*/
	FTM0->CONTROLS[0].CnV = channelValue;
}

void ftm_speed_update(short int speed)
{
	switch (speed)
	{
		case 0:
			g_dutyCycle = 260;
		break;
		case 2:
			g_dutyCycle = 250;
		break;
		case 4:
			g_dutyCycle = 240;
		break;
		case 6:
			g_dutyCycle = 230;
		break;
		case 8:
			g_dutyCycle = 220;
		break;
		case 10:
			g_dutyCycle = 210;
		break;
		case 12:
			g_dutyCycle = 200;
		break;
		case 14:
			g_dutyCycle = 190;
		break;
		case 16:
			g_dutyCycle = 180;
		break;
		case 18:
			g_dutyCycle = 170;
		break;
		case 20:
			g_dutyCycle = 160;
		break;
		case 22:
			g_dutyCycle = 150;
		break;
		case 24:
			g_dutyCycle = 140;
		break;
		case 26:
			g_dutyCycle = 130;
		break;
		case 28:
			g_dutyCycle = 120;
		break;
		case 30:
			g_dutyCycle = 110;
		break;
		case 32:
			g_dutyCycle = 100;
		break;
		case 34:
			g_dutyCycle = 90;
		break;
		case 36:
			g_dutyCycle = 80;
		break;
		case 38:
			g_dutyCycle = 70;
		break;
		case 40:
			g_dutyCycle = 60;
		break;
		case 42:
			g_dutyCycle = 50;
		break;
		case 44:
			g_dutyCycle = 40;
		break;
		default:
			g_dutyCycle = 260;
		break;
	}
	ftm_speed_chnnlVal(g_dutyCycle);
}
