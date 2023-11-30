/*
 * @file     XPT2046.h
 *
 * @Authors  Juan Pablo Villanueva
 *           Jose Angel Gonzalez
 *
 * @brief    Header file containing the prototype functions related to the use
 *           of the XPT2046 controller, which is in charge of detecting touch
 *           inputs on the display.
 */

#ifndef XPT2046_H_
#define XPT2046_H_

#include "MK64F12.h"
#include <stdint.h>
#include "fsl_dspi.h"
#include "fsl_port.h"
#include "fsl_gpio.h"
#include "fsl_pit.h"
#include "NVIC.h"
#include "gpio.h"
#include "PIT.h"
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
#define TOUCH_IRQ_GPIO   GPIOB

#define TOUCH_CE_PIN   3
#define SPI_CLK_PIN    1
#define SPI_MOSI_PIN   2
#define SPI_MISO_PIN   3
#define TOUCH_IRQ_PIN  18

#define SPI_MUX_ALT     kPORT_MuxAlt2

#define TOUCH_CTAR      kDSPI_Ctar1
#define TOUCH_BAUDRATE  2000000U
#define NANOSEC_DELAY   100U

#define TOUCH_CPOL  kDSPI_ClockPolarityActiveHigh
#define TOUCH_CPHA  kDSPI_ClockPhaseFirstEdge

#define TOUCH_PIT_CHNL kPIT_Chnl_1
#define TOUCH_DELAY    USEC_TO_COUNT(10000U, 21000000U)
#define TOUCH_PIT_IRQ  PIT_CH1_IRQ
#define TOUCH_PIT_PRIO PRIORITY_4

/*
 * ******************************************************************
 * Structs and enums:
 * ******************************************************************
 */

/* Structure that contains the x and y coordinates of a touched spot: */
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
 * @brief: Configures and initializes the peripherals used (SPI, PIT, GPIO)
 */
void Touch_config_peripherals(void);


/*
 * @brief: Indicates whether the screen has been touched.
 *
 * @retval: true if the screen has been touched, false otherwise.
 */
bool Touch_pressed(void);


/*
 * @brief: Sets the value of the flag that indicates whether the screen has
 *         been touched to false, avoiding multiple responses to the same
 *         touch. Should be used after Touch_pressed().
 */
void Touch_clear_irq_flag(void);


/*
 * @brief: When the screen has been touched, returns the screen coordinates
 *         where it was touched.
 *         TODO: process these coordinates, since currently the origin is
 *         at the lower right corner, and the highest point, at the higher left
 *
 * @retval: structure containing the x and y coordinates of the touched point.
 */
Coordinate_t Touch_get_coordinates(void);

#endif /* XPT2046_H_ */
