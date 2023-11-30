/*
 * @file     MPU6050.h
 *
 * @Authors  Juan Pablo Villanueva
 *           Jose Angel Gonzalez
 *
 * @brief    This header file contains the prototype functions for
 * 			 manipulating MPU6050 module
 */

#ifndef MPU6050_H_
#define MPU6050_H_

#include "fsl_i2c.h"
#include "fsl_port.h"
#include "fsl_clock.h"
#include "fsl_pit.h"
#include "NVIC.h"
#include "PIT.h"

/*
 * ******************************************************************
 * Definitions:
 * ******************************************************************
 */

#define MPU_ADDRESS           0x68

#define I2C_CLK_GATING        kCLOCK_PortE
#define I2C_PORT              PORTE
#define I2C_PIN_MUX           kPORT_MuxAlt5
#define MODULE_I2C            I2C0
#define I2C_MASTER_CLK_FREQ   CLOCK_GetFreq(kCLOCK_BusClk)

#define SCL_PIN               24U
#define SDA_PIN               25U

#define MILLIS_TO_SEC         1000.0
#define Acc_R                 16384.0f
#define Gyr_R                 131.0f
#define RAD_2_DEG             57.295779f
#define PI2_ADJUST_VAL        3.14159265f / 2
#define HPF                   0.98
#define LPF                   0.02



typedef struct{
	int16_t AcX;
	int16_t AcY;
	int16_t AcZ;
}Acc_t;

typedef struct{
	int16_t GyX;
	int16_t GyY;
	int16_t GyZ;
}Gyro_t;

/*
 * ******************************************************************
 * Function prototypes:
 * ******************************************************************
 */


/*
 * @brief: Initialization function for waking up MPU6050 module, as well
 *         as the configuration for I2C communication and a PIT channel
 */
void MPU6050_init(void);

/*
 * @brief: Reads the accelerometer raw values
 */
Acc_t MPU6050_read_acc(void);

/*
 * @brief: Reads the gyroscope raw values
 */
Gyro_t MPU6050_read_gyro(void);

/*
 * @brief: Gets the degrees of inclination of the sensor
 *         on the Y-axis.
 */
float MPU6050_get_angle(void);

/*
 * @brief: Callback function for the PIT to increase a timer
 */
void update_counter(void);

/*Math functions*/

/*
 * @brief: Gets the square root of the number
 */
float raiz(float num);

/*
 * @brief: Arctangent function, based on a series of approximations
 */
float arctan(float x);

/*
 * @brief: Raise the number to the desired power
 */
float pot(float x,uint32_t expo);


#endif /* MPU6050_H_ */
