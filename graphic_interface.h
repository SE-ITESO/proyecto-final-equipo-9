/*
 * @file     graphic_interface.h
 *
 * @Authors  Juan Pablo Villanueva
 *           Jose Angel Gonzalez
 *
 * @brief    Header file related to the actual letters and symbols the user
 *           will see on screen, as well as the position of touch points of
 *           interest, like buttons.
 */

#ifndef GRAPHIC_INTERFACE_H_
#define GRAPHIC_INTERFACE_H_

#include "ILI9341.h"
#include "XPT2046.h"

/*
 * ******************************************************************
 * Definitions:
 * ******************************************************************
 */

#define CHAR_HEIGHT  16
#define CHAR_PIXELS  288
#define CHAR_SIZE    96



/*
 * ******************************************************************
 * Definitions:
 * ******************************************************************
 */

/* Structure used when writing a message on the screen: */
typedef struct {
	uint8_t * message;
	uint32_t msg_size;
} screen_message_t;

/* */
typedef struct {
	screen_message_t btn_msg;
	uint16_t x;
	uint16_t y;
	uint16_t w;
	uint16_t h;
} button_t;

/*
 * ******************************************************************
 * Function prototypes:
 * ******************************************************************
 */

/*
 * @brief: Initialization of the modules required for the functioning of the
 *         display (ILI9341 and XPT2046).
 */
void GUI_init(void);


/*
 * @brief: Moves the cursor (which is merely conceptual to help the user move
 *         around the screen), marking the place where the next message will
 *         be written.
 */
void GUI_set_cursor(uint16_t x, uint16_t y);


/*
 *
 */
void GUI_write_char(uint8_t c);


/*
 *
 */
void GUI_write_string(screen_message_t * msg);


/*
 *
 */
void GUI_create_button(button_t * btn_info);

#endif /* GRAPHIC_INTERFACE_H_ */
