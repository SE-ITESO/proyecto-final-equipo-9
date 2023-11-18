/*
 *
 */

#ifndef ILI9341_H_
#define ILI9341_H_

#include "MK64F12.h"
#include <stdint.h>
#include "fsl_dspi.h"
#include "fsl_port.h"
#include "fsl_gpio.h"

/*
 * ******************************************************************
 * Definitions:
 * ******************************************************************
 */

#define EXIT_SLEEP 0x11
#define DISPLAY_ON 0x29

#define ILI9341_PWCTR1   0xC0 // Power Control 1
#define ILI9341_PWCTR2   0xC1 // Power Control 2
#define ILI9341_VMCTR1   0xC5 // VCOM Control 1
#define ILI9341_VMCTR2   0xC7 // VCOM Control 2
#define ILI9341_MADCTL   0x36 // Memory Access Control
#define ILI9341_VSCRSADD 0x37 // Vertical Scrolling Start Address
#define ILI9341_PIXFMT   0x3A // COLMOD: Pixel Format Set
#define ILI9341_FRMCTR1  0xB1 // Frame Rate Control (In Normal Mode/Full Colors)
#define ILI9341_DFUNCTR  0xB6 // Display Function Control
#define ILI9341_GAMMASET 0x26 // Gamma Set
#define ILI9341_GMCTRP1  0xE0 // Positive Gamma Correction
#define ILI9341_GMCTRN1  0xE1 // Negative Gamma Correction

#define ILI9341_RDMODE 0x0A     ///< Read Display Power Mode

#define SCREEN_SPI_CLOCK kCLOCK_PortD
#define CTRL_PINS_CLOCK  kCLOCK_PortC

#define SCREEN_SPI_PORT  PORTD
#define CTRL_PINS_PORT   PORTC
#define CTRL_PINS_GPIO   GPIOC

#define DISPLAY_CE_PIN  0
#define SPI_CLK_PIN     1
#define SPI_MOSI_PIN    2
#define DATA_OR_CMD_PIN 5
#define RESET_PIN       7

#define SPI_MUX_ALT     kPORT_MuxAlt2

#define DISPLAY_CTAR      kDSPI_Ctar0
#define DISPLAY_BAUDRATE  2000000U
#define LCD_NANOSEC_DELAY 1000U

#define DISPLAY_CPOL kDSPI_ClockPolarityActiveHigh
#define DISPLAY_CPHA kDSPI_ClockPhaseFirstEdge

#define INIT_SEQ_SIZE 106

#define SCREEN_WIDTH  320
#define SCREEN_HEIGHT 240

#define ILI9341_CASET 0x2A // Column Address Set
#define ILI9341_PASET 0x2B // Page Address Set
#define ILI9341_RAMWR 0x2C // Memory Write

/*
 * ******************************************************************
 * Structs and enums:
 * ******************************************************************
 */

typedef struct {
	uint8_t red   : 5;
	uint8_t green : 6;
	uint8_t blue  : 5;
} RGB_pixel_t;


/*
 * ******************************************************************
 * Function prototypes:
 * ******************************************************************
 */

/*
 *
 */
void Display_config_peripherals(void);


/*
 *
 */
void Display_send_command(uint8_t command, uint8_t * args, uint8_t arg_num);


/*
 *
 */
void Display_init(void);


/*
 *
 */
void Display_fill_screen(RGB_pixel_t color);

#endif /* ILI9341_H_ */
