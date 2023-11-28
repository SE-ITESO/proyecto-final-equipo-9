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

/* Structure used when writing a message on the screen: */
typedef struct {
	uint8_t * message;
	uint32_t msg_size;
	uint8_t font_size;
} screen_message_t;

/* Enum used to limit the possible font sizes: */
typedef enum {
	SmallFont,
	MediumFont,
	BigFont
} font_size_t;

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
void GUI_set_cursor(uint16_t x, uint16_t y, font_size_t font_size);

#endif /* GRAPHIC_INTERFACE_H_ */
