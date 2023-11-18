/*
 * @file     PIT.h
 *
 * @Authors  Juan Pablo Villanueva
 *           Jose Angel Gonzalez
 *
 * @brief    This is the header file used for PIT callback
 *           initialization.
 */

#ifndef PIT_H_
#define PIT_H_

#include "fsl_pit.h"

// Definition of the struct used for software interrupt flags:
typedef struct
{
	uint8_t flag_chnl_0 : 1;
	uint8_t flag_chnl_1 : 1;
	uint8_t flag_chnl_2 : 1;
	uint8_t flag_chnl_3 : 1;
} pit_interrupt_flags_t;

/*
 * ******************************************************************
 * ******************************************************************
 * Function prototypes:
 * ******************************************************************
 * ******************************************************************
 */

/*!
 * @brief Sets the callback function to be executed when the specified
 *        PIT channel generates an interruption.
 *
 * @param pit_channel PIT channel for whose interruptions the callback
 *                    function will be executed.
 * @param handler     Function that will serve the role of a callback
 */
void PIT_callback_init(pit_chnl_t pit_channel, void (*handler)(void));


/*!
 * @brief Returns the software interrupt flag of a PIT channel. Used
 *        when attending interrupts via getters and setters (when no
 *        interrupt callback has been initialized). Return value is
 *        either a 1 or a 0.
 *
 * @param pit_channel Flag to be read (PIT channel from 0 to 3).
 */
uint8_t PIT_get_interrupt_status(pit_chnl_t pit_channel);


/*!
 * @brief Clears the software interrupt flag of a PIT channel. Used
 *        when attending interrupts via getters and setters (when no
 *        interrupt callback has been initialized).
 *
 * @param pit_channel Flag to be cleared (PIT channel from 0 to 3).
 */
void PIT_clear_interrupt_status(pit_chnl_t pit_channel);


#endif /* PIT_H_ */
