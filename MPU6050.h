/*
 * MPU6050.h
 *
 *  Created on: 21 nov 2023
 *      Author: JVillanueva
 */

#ifndef MPU6050_H_
#define MPU6050_H_

#include "fsl_i2c.h"
#include "fsl_port.h"
#include "fsl_clock.h"

#define MPU_ADDRESS           0x68

#define I2C_CLK_GATING        kCLOCK_PortE
#define I2C_PORT              PORTE
#define I2C_PIN_MUX           kPORT_MuxAlt5
#define MODULE_I2C            I2C0
#define I2C_MASTER_CLK_FREQ   CLOCK_GetFreq(kCLOCK_BusClk)

#define SCL_PIN               24U
#define SDA_PIN               25U


typedef struct{
	uint16_t AcX;
	uint16_t AcY;
	uint16_t AcZ;
}Acc_t;

typedef struct{
	uint16_t GyX;
	uint16_t GyY;
	uint16_t GyZ;
}Gyro_t;



void MPU6050_init(void);

Acc_t MPU6050_read_acc(void);

Gyro_t MPU6050_read_gyro(void);



#endif /* MPU6050_H_ */
