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
#include "MPU6050.h"
#include <stdint.h>
#include <math.h>



Acc_t aceleracion = {0};
Gyro_t giroscopio = {0};

uint32_t time_elapsed = 0;
uint32_t time = 1000;
uint32_t counter = 0;

float angle = 0.0f;

int main(void)
{

	MPU6050_init();

	angle = MPU6050_get_angle();

	while(1)
	{

	}

}
