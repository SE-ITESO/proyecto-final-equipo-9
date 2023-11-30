/*
 * @file     bicycle.h
 *
 * @Authors  Juan Pablo Villanueva
 *           Jose Angel Gonzalez
 *
 * @brief    Header file for the top-level module, containing the function
 *           prototypes necessary to operate the bicycle monitoring system.
 */
#ifndef BICYCLE_H_
#define BICYCLE_H_

#include "graphic_interface.h"
#include "MPU6050.h"
#include "rtc_mod.h"
#include "freq.h"

/*
 * ******************************************************************
 * Definitions:
 * ******************************************************************
 */

#define WHEEL       2.075f

/*
 * ******************************************************************
 * Structs and enums:
 * ******************************************************************
 */

typedef enum {
	DataState,
	RecordState,
} state_t;

/*
 * ******************************************************************
 * Function prototypes:
 * ******************************************************************
 */

/*
 *
 */
void display_data(void);


/*
 *
 */
void display_record(uint32_t distance, uint32_t speed);


/*
 *
 */
void bicycle_main_screen(void);


/*
 *
 */
void bicycle_record_screen(void);


/*
 *
 */
void bicyclye_init_modules(void);


/*
 *
 */
void bicycle_update_FSM(void);


/*
 * @brief: Calculates bicycle speed based on the frequency at which the wheel
 *         is turning.
 */
float bicycle_calculate_speed(float freq);

#endif /* BICYCLE_H_ */
