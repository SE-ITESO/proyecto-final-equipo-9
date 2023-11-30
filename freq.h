/*
 * freq.h
 *
 *  Created on: 29 nov 2023
 *      Author: JVillanueva
 */

#ifndef FREQ_H_
#define FREQ_H_

#include "fsl_port.h"
#include "fsl_gpio.h"
#include "fsl_pit.h"
#include "NVIC.h"
#include "PIT.h"
#include "gpio.h"

void capture_values(uint32_t flags);

void init_freq(void);

float freq_get_freq(void);

#endif /* FREQ_H_ */
