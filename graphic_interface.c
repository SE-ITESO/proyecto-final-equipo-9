/*
 * @file     graphic_interface.c
 *
 * @Authors  Juan Pablo Villanueva
 *           Jose Angel Gonzalez
 *
 * @brief    Source file related to the displaying of characters and symbols
 *           on screen, as well as determining if a touch event should trigger
 *           a system function.
 */

#include "graphic_interface.h"

/*
 * ******************************************************************
 * Global variables:
 * ******************************************************************
 */

uint16_t g_current_cursor_size = 8;

/*
 * ******************************************************************
 * Function code:
 * ******************************************************************
 */

/*
 * @brief: Initialization of the modules required for the functioning of the
 *         display (ILI9341 and XPT2046).
 */
void GUI_init(void)
{
	Display_config_peripherals();
	Touch_config_peripherals();
	Display_init();
}


/*
 * @brief: Moves the cursor (which is merely conceptual to help the user move
 *         around the screen), marking the place where the next message will
 *         be written.
 */
void GUI_set_cursor(uint16_t x, uint16_t y, font_size_t font_size)
{
	uint16_t window_w = SCREEN_WIDTH  - x;

	switch (font_size)
	{
	    case SmallFont:
	    	g_current_cursor_size = 8;
	    break;

	    case MediumFont:
	    	g_current_cursor_size = 12;
	    break;

	    case BigFont:
	    	g_current_cursor_size = 16;
	    break;

	    default:
	    	g_current_cursor_size = 12;
	}

	Display_set_window(x, y, window_w, g_current_cursor_size);
}
