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

int main(void)
{
	// TODO: execute configuration functions

	RGB_pixel_t red = {0x0F, 0x1F, 0x0F};

	Display_config_peripherals();
	Display_init();

	Display_fill_screen(red);

    while(1)
    {
    	// TODO: Execute periodic functions

    }
    return 0 ;
}
