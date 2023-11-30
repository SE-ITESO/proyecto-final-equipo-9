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
#include "bicycle.h"

int main(void)
{


	bicyclye_init_modules();

	while(1)
	{
		bicycle_update_FSM();
	}
    return 0 ;
}
