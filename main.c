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
	button_t boton1 = {
			{"SIMON", 5},
			120, 160,
			94, 32
	};
	Coordinate_t pressed_spot = {0};
	uint32_t counter = 0;

	GUI_init();
	bicycle_main_screen();


    while(1)
    {
    	// TODO: Execute periodic functions
    	if (Touch_pressed())
    	{
    		Touch_clear_irq_flag();
    		pressed_spot = Touch_get_coordinates();
    		counter++;
    	}

    }
    return 0 ;
}
