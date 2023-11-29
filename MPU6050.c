/*
 * @file     MPU6050.c
 *
 * @Authors  Juan Pablo Villanueva
 *           Jose Angel Gonzalez
 *
 * @brief    This source file contains the functions necessary to
 *           initialize, read values, and calculate angles using
 *           MPUT6050 module
 */

#include "MPU6050.h"

uint32_t g_actual_time = 0;
uint32_t g_prior_time = 0;

float g_angle = 0.0f;
float g_prior_angle = 0.0f;

/*
 * @brief: Callback function for the PIT to increase a timer
 */
void update_counter(void)
{
	g_actual_time++;
}

/*
 * @brief: Raise the number to the desired power
 */
float pot(float x,uint32_t expo)
{
	uint32_t j = 0;
	float pot = 1;
	for(j=0;j<expo;j++)
	{
		pot *= x;
	}
	return pot;
}

/*
 * @brief: Arctangent function, based on a series of approximations
 */

float arctan(float x)
{
	float result = 0.0f;
    float sign = 1.0;
    uint32_t i = 0;

	if(x > 1)
	{
		x = 1/x;
		return PI2_ADJUST_VAL - arctan(x);
	}

    for (i = 1; i <= 1000; i += 2)
    {
        result += (sign * (pot(x,i) / (float)i));
        sign = -sign;
    }
    return result;
}

/*
 * @brief: Gets the square root of the number
 */
float raiz(float num)
{
	uint32_t i = 0;

    float raiz = num / 2.0;

    for (i = 0; i < 100; ++i) {
        raiz = 0.5 * (raiz + (num / raiz));
    }
    return raiz;
}

/*
 * @brief: Initialization function for waking up MPU6050 module, as well
 *         as the configuration for I2C communication and a PIT channel
 */
void MPU6050_init(void)
{
	i2c_master_config_t masterConfig;
	uint32_t sourceClock = 0;
	i2c_master_transfer_t masterXfer;
	uint8_t master_txBuff[2] = {0};
	uint32_t deviceAddress = 0x00U;


	CLOCK_EnableClock(I2C_CLK_GATING);

	PORT_SetPinMux(I2C_PORT, SCL_PIN, I2C_PIN_MUX);
	PORT_SetPinMux(I2C_PORT, SDA_PIN, I2C_PIN_MUX);

	I2C_MasterGetDefaultConfig(&masterConfig);
	sourceClock = I2C_MASTER_CLK_FREQ;

	I2C_MasterInit(MODULE_I2C, &masterConfig, sourceClock);

	master_txBuff[0] = 0x6B;
	master_txBuff[1] = 0x00;

	masterXfer.slaveAddress   = MPU_ADDRESS;
	masterXfer.direction      = kI2C_Write;
	masterXfer.subaddress     = deviceAddress;
	masterXfer.subaddressSize = 0;
	masterXfer.data           = master_txBuff;
	masterXfer.dataSize       = 2;
	masterXfer.flags          = kI2C_TransferDefaultFlag;

	I2C_MasterTransferBlocking(I2C0, &masterXfer);

	// PIT config:
	pit_config_t pit_config;
	PIT_GetDefaultConfig(&pit_config);
	PIT_Init(PIT, &pit_config);
	PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, USEC_TO_COUNT(1000, 21000000));

	// PIT interrupt config:
	PIT_EnableInterrupts(PIT, kPIT_Chnl_0, kPIT_TimerInterruptEnable);
	PIT_callback_init(kPIT_Chnl_0, update_counter);
	NVIC_enable_interrupt_and_priotity(PIT_CH0_IRQ, PRIORITY_1);

	PIT_StartTimer(PIT, kPIT_Chnl_0);
}

/*
 * @brief: Reads the accelerometer raw values
 */
Acc_t MPU6050_read_acc(void)
{
	i2c_master_transfer_t masterXfer;
	uint8_t master_rxBuff[6] = {0};
	Acc_t values;
	uint32_t deviceAddress = 0x3BU;

	masterXfer.slaveAddress   = MPU_ADDRESS;
	masterXfer.direction      = kI2C_Read;
	masterXfer.subaddress     = deviceAddress;
	masterXfer.subaddressSize = 1;
	masterXfer.data           = master_rxBuff;
	masterXfer.dataSize       = 6;
	masterXfer.flags          = kI2C_TransferDefaultFlag;

	I2C_MasterTransferBlocking(I2C0, &masterXfer);

	values.AcX = (master_rxBuff[0] << 8) | (master_rxBuff[1]);
	values.AcY = (master_rxBuff[2] << 8) | (master_rxBuff[3]);
	values.AcZ = (master_rxBuff[4] << 8) | (master_rxBuff[5]);

	return values;
}

/*
 * @brief: Reads the gyroscope raw values
 */
Gyro_t MPU6050_read_gyro(void)
{
	i2c_master_transfer_t masterXfer;
	uint8_t master_rxBuff[6] = {0};
	Gyro_t values;
	uint32_t deviceAddress = 0x43U;

	masterXfer.slaveAddress   = MPU_ADDRESS;
	masterXfer.direction      = kI2C_Read;
	masterXfer.subaddress     = deviceAddress;
	masterXfer.subaddressSize = 1;
	masterXfer.data           = master_rxBuff;
	masterXfer.dataSize       = 6;
	masterXfer.flags          = kI2C_TransferDefaultFlag;

	I2C_MasterTransferBlocking(I2C0, &masterXfer);

	values.GyX = (master_rxBuff[0] << 8) | (master_rxBuff[1]);
	values.GyY = (master_rxBuff[2] << 8) | (master_rxBuff[3]);
	values.GyZ = (master_rxBuff[4] << 8) | (master_rxBuff[5]);

	return values;
}

/*
 * @brief: Gets the degrees of inclination of the sensor
 *         on the Y-axis.
 */
float MPU6050_get_angle(void)
{
	float Acc_tot = 0.0f;
	float Acc[3] = {0};
	float Gyr[3] = {0};
	float dt = 0.0f;

	Acc_t aceleracion = {0};
	Gyro_t giroscopio = {0};

	//Get values from the module:
	aceleracion = MPU6050_read_acc();
	giroscopio  = MPU6050_read_gyro();

	Acc[0] = (aceleracion.AcX + 350) / Acc_R;
	Acc[1] = (aceleracion.AcY + 350) / Acc_R;
	Acc[2] = (aceleracion.AcZ + 1350) / Acc_R;

	Acc_tot = arctan((Acc[1]) / (raiz(pot(Acc[0], 2) + pot(Acc[2], 2))));
	Acc_tot *= RAD_2_DEG;

	Gyr[0] = (giroscopio.GyX) / Gyr_R;

	dt = (g_actual_time - g_prior_time) / MILLIS_TO_SEC;
	g_prior_time = g_actual_time;

	g_angle = HPF * (Acc_tot + Gyr[0] * dt) + (LPF * Acc_tot);

	return g_angle;
}

