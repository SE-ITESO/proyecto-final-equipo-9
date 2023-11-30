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
#include "ftm_speed.h"
#include "freq.h"

/*
 * ******************************************************************
 * Definitions:
 * ******************************************************************
 */

#define WHEEL 2.075f

#define UPDATE_PIT_CHNL kPIT_Chnl_2
#define UPDATE_PIT_IRQ  PIT_CH2_IRQ

/*
 * ******************************************************************
 * Structs and enums:
 * ******************************************************************
 */

/* Enum used to determine FSM state: real-time or recorded measures */
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
 * @brief: Configuration of all modules used, related to the screen (display
 *         and touch module), external devices (gyroscope, memory and Hall
 *         effect sensor), and a PIT used to update measures.
 */
void bicyclye_init_modules(void);


/*
 * @brief: Writes on screen the text that doesn't change in the real-time
 *         measuring screen: title and description text.
 */
void bicycle_main_screen(void);


/*
 * @brief: Writes on screen the text that doesn't change in the registered
 *         measures screen: title and description text.
 */
void bicycle_record_screen(void);


/*
 * @brief: checks if the touch screen has been pressed in order to
 *         change between states, and which information to display.
 */
void bicycle_update_FSM(void);


/*
 * @brief: Displays the current measures in the display, decoding int and
 *         float values into characters, and using each measure's corresponding
 *         coordinates.
 */
void display_data(void);


/*
 * @brief: Displays the recorded historic measures of average speed and total
 *         distance traveled, decoding numbers into ASCII and displaying
 *         each value in its corresponding screen position.
 */
void display_record(uint32_t distance, uint32_t speed);


/*
 * @brief: Calculates bicycle speed based on the frequency at which the wheel
 *         is turning.
 */
float bicycle_calculate_speed(float freq);


#endif /* BICYCLE_H_ */
