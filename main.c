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


Acc_t aceleracion = {0};
Gyro_t giroscopio = {0};

int main(void)
{
	MPU6050_init();
	aceleracion = MPU6050_read_acc();
	giroscopio = MPU6050_read_gyro();

	while(1)
	{

	}

}
