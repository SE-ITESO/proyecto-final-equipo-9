/*
 * bicycle.c
 *
 *  Created on: 29 nov 2023
 *      Author: JVillanueva
 */


#include "bicycle.h"

static state_t g_current_state = DataState;
uint8_t g_title_msg[]    = "CURRENT TRIP";
uint8_t g_speed_msg[]    = "SPEED:";
uint8_t g_angle_msg[]    = "INCLINATION:";
uint8_t g_distance_msg[] = "DISTANCE:";

uint8_t g_speed_data[]    = "00.0 KM/H";
uint8_t g_angle_data[]    = "00.0^";
uint8_t g_distance_data[] = "0000 M";

bool g_data_refresh = 0;

screen_message_t g_title_str    = {g_title_msg,    12};
screen_message_t g_speed_str    = {g_speed_msg,     6};
screen_message_t g_angle_str    = {g_angle_msg,    12};
screen_message_t g_distance_str = {g_distance_msg,  9};

mem_data_t g_record = {0};

float g_inclination   = 0.0;
float g_current_speed = 0.0;
float g_prev_speed    = 0.0;
uint32_t g_distance   = 0.0;
float g_freq          = 0;

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

void data_refresh(void)
{
	g_data_refresh = true;
}

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
	PIT_callback_init(kPIT_Chnl_2, data_refresh);
	NVIC_enable_interrupt_and_priotity(PIT_CH2_IRQ, PRIORITY_1);


	PIT_StartTimer(PIT, kPIT_Chnl_2);



}

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



void bicycle_update_FSM(void)
{
	RGB_pixel_t bg_color = {0x1F, 0x3F, 0x1F};

	switch (g_current_state)
	{
		case DataState:
			if(GUI_button_pressed(&g_record_btn))
			{
				g_current_state = RecordState;
				Display_fill_screen(bg_color);
				GUI_create_button(&g_sesion_btn);
			}
			else if (g_data_refresh)
			{
				g_freq = freq_get_freq();
				g_prev_speed = g_current_speed;
				g_current_speed = bicycle_calculate_speed(g_freq);
				g_inclination = MPU6050_get_angle();
				//g_distance = bicycle_calculate_distance(g_current_speed);

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
	g_distance_data[0] = (g_distance / 10) + 0x30;
	g_distance_data[1] = (g_distance % 10)  + 0x30;
	g_distance_data[3] = ((g_distance * 10) % 10) + 0x30;

	distance_data.message = g_distance_data;
	distance_data.msg_size = 9;

	GUI_set_cursor(162,172);
	GUI_write_string(&distance_data);
}

float bicycle_calculate_speed(float freq)
{
	float speed = 0.0f;

	speed = freq * WHEEL;

	speed *= 3.6f;

	return speed;
}
/*
uint32_t bicycle_calculate_distance(float speed)
{

}
*/
