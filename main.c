/*
 * @file     main.c
 *
 * @Authors  Juan Pablo Villanueva
 *           Jose Angel Gonzalez
 *
 * @brief    Application entry point.
 */

#include "bicycle.h"

int main(void)
{
	bicyclye_init_modules();

	while (true)
	{
		bicycle_update_FSM();
	}
    return 0 ;
}
