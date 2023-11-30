/*
 * @file     rtc_mod.h
 *
 * @Authors  Juan Pablo Villanueva
 *           Jose Angel Gonzalez
 *
 * @brief    Header file for the RTC module manipulation driver.
 *           Includes definitions of important register addresses,
 *           configuration types and values, and function declarations.
 */

#ifndef RTC_MOD_H_
#define RTC_MOD_H_

#include "fsl_clock.h"
#include "fsl_port.h"
#include "fsl_i2c.h"

/*
 * ******************************************************************
 * Definitions:
 * ******************************************************************
 */

#define I2C_CLK_GATING_RTC    kCLOCK_PortB
#define I2C_PORT_RTC          PORTB
#define I2C_PIN_MUX_RTC       kPORT_MuxAlt2
#define MODULE_I2C            I2C0

#define SCL_PIN_RTC           2U
#define SDA_PIN_RTC           3U

#define MOD_24_HRS            ~0x40
#define I2C_MASTER_CLK_FREQ   CLOCK_GetFreq(kCLOCK_BusClk)
#define I2C_DATA_LENGTH       3U
#define RTC_MASTER_SLAVE_ADDR 0x68U
#define MEM_MASTER_SLAVE_ADDR 0x50U
#define TIME_REG_BASE_ADDR    0x00U
#define DATE_REG_BASE_ADDR    0x04U
#define CONTROL_REG_ADDR      0x07U
#define SQW_FREQ              0x10U

#define TENS_SHIFT            4U
#define UNITS_MASK            0xFU


/*
 * ******************************************************************
 * Structures and enums:
 * ******************************************************************
 */

/* Structure that stores a time, and an I2C transmission status */
typedef struct{
	uint8_t *speed;
	uint8_t *angle;
	uint8_t *distance;
} data_t;

/*
 * Structure that includes a data array of 1 Byte elements, its size and
 * its memory address, for EEPROM reading and writing
 */
typedef struct{
	uint8_t * data_array;
	uint32_t size;
	uint16_t address;     // EEPROM addresses are 12-bit.
} mem_data_t;


/*
 * ******************************************************************
 * Function prototypes:
 * ******************************************************************
 */

/*
 * @brief: Configuration for I2C transmissions with the RTC module:
 *         Includes I2C pin mux configuration, I2C module init and
 *         clock gating enabling.
 */
void RTC_mod_init(void);


/*
 * @brief: Function to write data to the RTC module's EEPROM.
 *
 * @param: data Pointer to a memory data structure where the data to be
 *         written, its length and its address are stored.
 *
 * @retval: Returns the I2C transmission status flag. On a successful data
            transmission, returns kStatus_Success (0).
 */
uint32_t RTC_mod_write_mem(mem_data_t * data);


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
uint32_t RTC_mod_read_mem(mem_data_t * data);


#endif /* RTC_MOD_H_ */
