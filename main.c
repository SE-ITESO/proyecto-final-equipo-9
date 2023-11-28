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

	RGB_pixel_t gray = {0x0F, 0x1F, 0x0F};
	RGB_pixel_t black = {0x00, 0x00, 0x00};

	Coordinate_t pressed_spot = {0};

	uint32_t counter = 0;

	GUI_init();

	Display_fill_screen(black);

	GUI_set_cursor(80, 60, MediumFont);
	Display_paint_color(gray, 162);
	Display_paint_color(black, 162);
	Display_paint_color(gray, 162);

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
