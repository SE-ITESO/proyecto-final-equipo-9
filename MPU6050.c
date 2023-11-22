/*
 * MPU6050.c
 *
 *  Created on: 21 nov 2023
 *      Author: JVillanueva
 */

#include "MPU6050.h"



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
}

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
