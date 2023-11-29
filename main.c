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

	uint32_t counter = 0;

	GUI_init();

	GUI_set_cursor(80, 60);
	GUI_write_char('U');
	GUI_write_char('V');
	GUI_write_char(':');
	GUI_write_char('1');
	GUI_write_char('.');
	GUI_write_char('2');
	GUI_write_char('/');
	GUI_write_char(' ');
	GUI_write_char('E');
	GUI_write_char('X');
	GUI_write_char('E');

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
