/*
 * @file     main.c
 *
 * @Authors  Juan Pablo Villanueva
 *           Jose Angel Gonzalez
 *
 * @brief    Application entry point.
 */

//TODO: Include driver libraries

#include "graphic_interface.h"
#include "bicycle.h"

int main(void)
{
	// TODO: execute configuration functions

	Coordinate_t pressed_spot = {0};

	button_t boton1 = {
			{"SIMON", 5},
			160, 120,
			78, 32
	};
	uint32_t counter = 0;

	GUI_init();
	bicycle_main_screen();

    while(1)
    {
    	// TODO: Execute periodic functions
    	if (GUI_button_pressed(&boton1))
    	{
    		counter++;
    	}
    }
    return 0 ;
}
