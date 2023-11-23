/*
 * @file     main.c
 *
 * @Authors  Juan Pablo Villanueva
 *           Jose Angel Gonzalez
 *
 * @brief    Application entry point.
 */

//TODO: Include driver libraries

#include "ILI9341.h"
#include "XPT2046.h"

int main(void)
{
	// TODO: execute configuration functions

	RGB_pixel_t gray = {0x0F, 0x1F, 0x0F};
	RGB_pixel_t black = {0x00, 0x00, 0x00};

	Coordinate_t pressed_spot = {0};

	uint32_t counter = 0;

	Display_config_peripherals();
	Touch_config_peripherals();
	Display_init();

	Display_fill_screen(black);

	Display_set_window(80, 120, 80, 120);
	Display_send_pixels(gray, 14400U);

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
