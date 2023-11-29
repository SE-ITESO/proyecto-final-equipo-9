/*
 * bicycle.h
 *
 *  Created on: 29 nov 2023
 *      Author: JVillanueva
 */

#include "ftm_freq.h"
#include "graphic_interface.h"
#include "MPU6050.h"

#define WHEEL       1.95f

typedef enum{
	DataState,
	RecordState,
}state_t;

void display_data(void);

void data_refresh(void);

void bicycle_main_screen(void);

void bicyclye_init_modules(void);

void bicycle_update_FSM(void);

float bicycle_calculate_speed(uint32_t freq);

uint32_t bicycle_calculate_distance(float speed);

