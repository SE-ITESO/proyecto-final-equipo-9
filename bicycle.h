/*
 * bicycle.h
 *
 *  Created on: 29 nov 2023
 *      Author: JVillanueva
 */

#include "graphic_interface.h"
#include "MPU6050.h"
#include "rtc_mod.h"
#include "freq.h"

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

float bicycle_calculate_speed(float freq);

uint32_t bicycle_calculate_distance(float speed);

