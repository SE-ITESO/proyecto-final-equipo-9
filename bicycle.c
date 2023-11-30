/*
 * @file     bicycle.c
 *
 * @Authors  Juan Pablo Villanueva
 *           Jose Angel Gonzalez
 *
 * @brief    Source file for the top-level module that gathers speed and
 *           inclination information and displays it on screen, along with the
 *           system's touch buttons.
 */

#include "bicycle.h"

/*
 * ******************************************************************
 * Private function prototypes:
 * ******************************************************************
 */

/* PIT callback that indicates new measures must be taken: */
static void data_refresh_callback(void);

/*
 * ******************************************************************
 * Global variables:
 * ******************************************************************
 */

static state_t g_current_state = DataState;

static uint8_t g_speed_data[]    = "00.0 KM/H";
static uint8_t g_angle_data[]    = "00.0^";
static uint8_t g_distance_data[] = "0000 M";

static bool g_data_refresh = 0;

static screen_message_t g_title_str    = {"CURRENT TRIP", 12};
static screen_message_t g_speed_str    = {"SPEED:",        6};
static screen_message_t g_angle_str    = {"INCLINATION:", 12};
static screen_message_t g_distance_str = {"DISTANCE:",     9};

static screen_message_t g_record_title_str = {"HISTORIC RECORDS", 16};
static screen_message_t g_dist_record_str  = {"TOTAL DISTANCE:",  15};
static screen_message_t g_speed_record_str = {"AVERAGE SPEED:",   14};

float g_inclination   = 0.0f;
float g_current_speed = 0.0f;
float g_prev_speed    = 0.0f;
uint32_t g_distance   = 0;
float g_freq          = 0.0f;

uint32_t g_avg_speed   = 0;
uint32_t g_avg_samples = 0;

button_t g_record_btn = {
		{"RECORD", 6},
		180, 200,
		96, 32
};

button_t g_sesion_btn = {
		{"SESION", 6},
		180, 200,
		96, 32
};

/*
 * ******************************************************************
 * Function code:
 * ******************************************************************
 */

/*
 *
 */
void bicyclye_init_modules(void)
{
	GUI_init();
	init_freq();
	MPU6050_init();

	bicycle_main_screen();

	GUI_create_button(&g_record_btn);

	// PIT config:
	PIT_SetTimerPeriod(PIT, kPIT_Chnl_2, USEC_TO_COUNT(500000U, 21000000));
	// PIT interrupt config:
	PIT_EnableInterrupts(PIT, kPIT_Chnl_2, kPIT_TimerInterruptEnable);
	PIT_callback_init(kPIT_Chnl_2, data_refresh_callback);
	NVIC_enable_interrupt_and_priotity(PIT_CH2_IRQ, PRIORITY_1);

	PIT_StartTimer(PIT, kPIT_Chnl_2);
}


/*
 *
 */
void bicycle_main_screen(void)
{
	GUI_set_cursor(94,1);
	GUI_write_string(&g_title_str);
	GUI_set_cursor(10,40);
	GUI_write_string(&g_speed_str);
	GUI_set_cursor(10,106);
	GUI_write_string(&g_angle_str);
	GUI_set_cursor(10,172);
	GUI_write_string(&g_distance_str);
}


/*
 *
 */
void bicycle_record_screen(void)
{
	GUI_set_cursor(80,1);
	GUI_write_string(&g_record_title_str);
	GUI_set_cursor(10,40);
	GUI_write_string(&g_dist_record_str);
	GUI_set_cursor(10,106);
	GUI_write_string(&g_speed_record_str);
}


/*
 *
 */
void bicycle_update_FSM(void)
{
	RGB_pixel_t bg_color = {0x1F, 0x3F, 0x1F};
	uint32_t saved_dist  = 0;
	uint32_t saved_speed = 0;
	mem_data_t mem_data_dist  = {
			(uint8_t *)(&saved_dist),
			4, 0x00
	};
	mem_data_t mem_data_speed = {
			(uint8_t *)(&saved_speed),
			4, 0x10
	};

	switch (g_current_state)
	{
		case DataState:
			if(GUI_button_pressed(&g_record_btn))
			{
				g_avg_speed = g_distance / (g_avg_samples);

				g_current_state = RecordState;
				Display_fill_screen(bg_color);
				GUI_create_button(&g_sesion_btn);

				RTC_mod_read_mem(&mem_data_dist);
				RTC_mod_read_mem(&mem_data_speed);

				saved_dist  += g_distance;
				saved_speed =  (saved_speed + g_avg_speed) / 2;

				RTC_mod_write_mem(&mem_data_dist);
				RTC_mod_write_mem(&mem_data_speed);

				bicycle_record_screen();
				display_record(saved_dist, saved_speed);

			}
			else if (g_data_refresh)
			{
				g_freq = freq_get_freq();
				g_prev_speed = g_current_speed;
				g_current_speed = bicycle_calculate_speed(g_freq);

				g_avg_samples++;

				g_inclination = MPU6050_get_angle();

				g_distance += (g_prev_speed / 3.6f);

				display_data();
				g_data_refresh = false;
			}
		break;

		case RecordState:
			if(GUI_button_pressed(&g_sesion_btn))
			{
				g_current_state = DataState;
				Display_fill_screen(bg_color);
				bicycle_main_screen();
				GUI_create_button(&g_record_btn);
			}
		break;
	}
}


/*
 *
 */
void display_data(void)
{
	screen_message_t speed_data = {0};
	screen_message_t angle_data = {0};
	screen_message_t distance_data = {0};

	uint32_t spd_val = (uint32_t)(g_current_speed * 10);
	uint32_t inc_val = (uint32_t)(g_inclination * 10);


	//Speed values:
	g_speed_data[0] = (spd_val / 100) + 0x30;
	g_speed_data[1] = ((spd_val / 10) % 10)  + 0x30;
	g_speed_data[3] = (spd_val % 10) + 0x30;

	speed_data.message = g_speed_data;
	speed_data.msg_size = 9;

	GUI_set_cursor(162,40);
	GUI_write_string(&speed_data);

	//inclination values:
	g_angle_data[0] = (inc_val / 100) + 0x30;
	g_angle_data[1] = ((inc_val / 10) % 10)  + 0x30;
	g_angle_data[3] = (inc_val % 10) + 0x30;

	angle_data.message = g_angle_data;
	angle_data.msg_size = 5;

	GUI_set_cursor(162,106);
	GUI_write_string(&angle_data);

	//distance values:
	g_distance_data[0] = ((g_distance / 1000) % 10) + 0x30;
	g_distance_data[1] = ((g_distance / 100)  % 10) + 0x30;
	g_distance_data[2] = ((g_distance / 10)   % 10) + 0x30;
	g_distance_data[3] = (g_distance % 10) + 0x30;

	distance_data.message = g_distance_data;
	distance_data.msg_size = 6;

	GUI_set_cursor(162,172);
	GUI_write_string(&distance_data);
}


/*
 *
 */
void display_record(uint32_t distance, uint32_t speed)
{
	screen_message_t speed_data = {0};
	screen_message_t distance_data = {0};

	// Speed values:
	g_speed_data[0] = (speed / 10) + 0x30;
	g_speed_data[1] = (speed % 10) + 0x30;
	g_speed_data[3] = '0';

	speed_data.message = g_speed_data;
	speed_data.msg_size = 9;

	GUI_set_cursor(10,126);
	GUI_write_string(&speed_data);

	//distance values:
	g_distance_data[0] = ((distance / 1000) % 10) + 0x30;
	g_distance_data[1] = ((distance / 100)  % 10) + 0x30;
	g_distance_data[1] = ((distance / 10)   % 10) + 0x30;
	g_distance_data[3] = (distance % 10) + 0x30;

	distance_data.message = g_distance_data;
	distance_data.msg_size = 6;

	GUI_set_cursor(10,60);
	GUI_write_string(&distance_data);
}


/*
 * @brief: Calculates bicycle speed based on the frequency at which the wheel
 *         is turning.
 */
float bicycle_calculate_speed(float freq)
{
	float speed = 0.0f;
	// Go from freq (turns per second) to meters per second to km per hour:
	speed = freq * WHEEL;
	speed *= 3.6f;

	return speed;
}


/*
 * @brief: This PIT callback turns on a flag that indicates new speed and
 *         inclination measures must be taken.
 */
static void data_refresh_callback(void)
{
	g_data_refresh = true;
}
