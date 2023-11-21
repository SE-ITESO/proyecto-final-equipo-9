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

typedef struct
{
	uint8_t flag_chnl_0 : 1;
	uint8_t flag_chnl_1 : 1;
	uint8_t flag_chnl_2 : 1;
	uint8_t flag_chnl_3 : 1;
} pit_interrupt_flags_t;

//Initialization function for PIT callback
void PIT_callback_init(pit_chnl_t pit_channel, void (*handler)(void));

void PIT_clear_interrupt_status(pit_chnl_t pit_channel);

uint8_t PIT_get_interrupt_status(pit_chnl_t pit_channel);

#endif /* PIT_H_ */
