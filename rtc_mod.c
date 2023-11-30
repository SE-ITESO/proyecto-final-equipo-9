/*
 * @file     rtc_mod.c
 *
 * @Authors  Juan Pablo Villanueva
 *           Jose Angel Gonzalez
 *
 * @brief    This file includes the developed functions related to
 *           communicating with RTC module through I2C protocol, including
 *           initializing, transmitting and recieving functions
 */

#include <stdint.h>
#include "rtc_mod.h"

/*
 * ******************************************************************
 * Function code:
 * ******************************************************************
 */

/*
 * @brief: Configuration for I2C transmissions with the RTC module:
 *         Includes I2C pin mux configuration, I2C module init and
 *         clock gating enabling.
 */
void RTC_mod_init(void)
{
	i2c_master_config_t masterConfig;
	uint32_t sourceClock = 0;

	CLOCK_EnableClock(I2C_CLK_GATING_RTC);

	PORT_SetPinMux(I2C_PORT_RTC, SCL_PIN_RTC, I2C_PIN_MUX_RTC);
	PORT_SetPinMux(I2C_PORT_RTC, SDA_PIN_RTC, I2C_PIN_MUX_RTC);

	I2C_MasterGetDefaultConfig(&masterConfig);
	sourceClock = I2C_MASTER_CLK_FREQ;

	I2C_MasterInit(MODULE_I2C, &masterConfig, sourceClock);
}



/*
 * @brief: Function to write data to the RTC module's EEPROM.
 *
 * @param: data Pointer to a memory data structure where the data to be
 *         written, its length and its address are stored.
 *
 * @retval: Returns the I2C transmission status flag. On a successful data
            transmission, returns kStatus_Success (0).
 */
uint32_t RTC_mod_write_mem(mem_data_t * data)
{
	uint32_t status = 0;
	i2c_master_transfer_t masterXfer;

	masterXfer.slaveAddress   = MEM_MASTER_SLAVE_ADDR;
	masterXfer.direction      = kI2C_Write;
	// Memory addresses are 12 bit, so 2 bytes are used:
	masterXfer.subaddress     = data->address;
	masterXfer.subaddressSize = 2;
	// Data to write and the amount of bytes to write are decided by the user:
	masterXfer.data           = data->data_array;
	masterXfer.dataSize       = data->size;
	masterXfer.flags          = kI2C_TransferDefaultFlag;

	status = I2C_MasterTransferBlocking(I2C0, &masterXfer);
	// Returns I2C status to let the user know if the transmission was correct:
	return status;
}


/*
 * @brief: Function to read data from the RTC module's EEPROM.
 *
 * @param: data Pointer to a memory data structure where the data length
 *              and data address are stored. This structure also holds
 *              the array where the read data is to be written.
 *
 * @retval: Returns the I2C transmission status flag. On a successful data
            transmission, returns kStatus_Success (0).
 */
uint32_t RTC_mod_read_mem(mem_data_t * data)
{
	uint32_t status = 0;
	i2c_master_transfer_t masterXfer;

	masterXfer.slaveAddress   = MEM_MASTER_SLAVE_ADDR;
	masterXfer.direction      = kI2C_Read;
	// Memory addresses are 12 bit, so 2 bytes are used:
	masterXfer.subaddress     = data->address;
	masterXfer.subaddressSize = 2;
	// Location where the data will be stored:
	masterXfer.data           = data->data_array;
	// The amount of bytes to read are decided by the user:
	masterXfer.dataSize       = data->size;
	masterXfer.flags          = kI2C_TransferDefaultFlag;

	status = I2C_MasterTransferBlocking(I2C0, &masterXfer);
	// Returns I2C status to let the user know if the transmission was correct:
	return status;
}
