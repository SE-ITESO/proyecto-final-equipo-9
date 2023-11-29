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
	GUI_create_button(&boton1);

    while(1)
    {
    	// TODO: Execute periodic functions
    	if (GUI_button_pressed(&boton1))
    	{
    		counter++;
    	}
    	else if (Touch_pressed())
    	{
    		Touch_clear_irq_flag();
    		pressed_spot = Touch_get_coordinates();
    		counter++;
    	}
    }
    return 0 ;
}
