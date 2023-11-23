/*
 *
 */

#ifndef XPT2046_H_
#define XPT2046_H_

#include "MK64F12.h"
#include <stdint.h>
#include "fsl_dspi.h"
#include "fsl_port.h"
#include "fsl_gpio.h"
#include <stdbool.h>

/*
 * ******************************************************************
 * Definitions:
 * ******************************************************************
 */

#define TOUCH_SPI_CLOCK  kCLOCK_PortD
#define TOUCH_CE_CLOCK   kCLOCK_PortC

#define TOUCH_SPI_PORT   PORTD
#define TOUCH_CE_PORT    PORTC
#define TOUCH_IRQ_GPIO   GPIOC

#define TOUCH_CE_PIN   3
#define SPI_CLK_PIN    1
#define SPI_MOSI_PIN   2
#define TOUCH_IRQ_PIN  0

#define SPI_MUX_ALT     kPORT_MuxAlt2

#define TOUCH_CTAR      kDSPI_Ctar1
#define TOUCH_BAUDRATE  5000000U
#define NANOSEC_DELAY   100U

#define TOUCH_CPOL  kDSPI_ClockPolarityActiveHigh
#define TOUCH_CPHA  kDSPI_ClockPhaseFirstEdge

/*
 * ******************************************************************
 * Structs and enums:
 * ******************************************************************
 */

typedef struct {
	uint16_t x_position;
	uint16_t y_position;
} Coordinate_t;

/*
 * ******************************************************************
 * Function prototypes:
 * ******************************************************************
 */

/*
 *
 */
void Touch_config_peripherals(void);


/*
 *
 */
bool Touch_pressed(void);


/*
 *
 */
Coordinate_t Touch_get_coordinates(void);

#endif /* XPT2046_H_ */
