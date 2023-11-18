/*
 *
 */

#include "ILI9341.h"

/*
 * ******************************************************************
 * Global variables:
 * ******************************************************************
 */

static const uint8_t g_init_sequence[INIT_SEQ_SIZE] = {
    0xEF, 3, 0x03, 0x80, 0x02,
    0xCF, 3, 0x00, 0xC1, 0x30,
    0xED, 4, 0x64, 0x03, 0x12, 0x81,
    0xE8, 3, 0x85, 0x00, 0x78,
    0xCB, 5, 0x39, 0x2C, 0x00, 0x34, 0x02,
    0xF7, 1, 0x20,
    0xEA, 2, 0x00, 0x00,
    ILI9341_PWCTR1  , 1, 0x23,              // Power control VRH[5:0]
    ILI9341_PWCTR2  , 1, 0x10,              // Power control SAP[2:0];BT[3:0]
    ILI9341_VMCTR1  , 2, 0x3e, 0x28,        // VCM control
    ILI9341_VMCTR2  , 1, 0x86,              // VCM control2
    ILI9341_MADCTL  , 1, 0x48,              // Memory Access Control
    ILI9341_VSCRSADD, 1, 0x00,              // Vertical scroll zero
    ILI9341_PIXFMT  , 1, 0x55,
    ILI9341_FRMCTR1 , 2, 0x00, 0x18,
    ILI9341_DFUNCTR , 3, 0x08, 0x82, 0x27,  // Display Function Control
    0xF2, 1, 0x00,                          // 3Gamma Function Disable
    ILI9341_GAMMASET , 1, 0x01,             // Gamma curve selected
    ILI9341_GMCTRP1 , 15, 0x0F, 0x31, 0x2B, 0x0C, 0x0E, 0x08, // Set Gamma
      0x4E, 0xF1, 0x37, 0x07, 0x10, 0x03, 0x0E, 0x09, 0x00,
    ILI9341_GMCTRN1 , 15, 0x00, 0x0E, 0x14, 0x03, 0x11, 0x07, // Set Gamma
      0x31, 0xC1, 0x48, 0x08, 0x0F, 0x0C, 0x31, 0x36, 0x0F,
};


/*
 * ******************************************************************
 * Function code:
 * ******************************************************************
 */

void Display_config_peripherals(void)
{
	dspi_master_config_t masterConfig;
	uint32_t srcClock_Hz;
	gpio_pin_config_t gpio_output_config = {
				        kGPIO_DigitalOutput,
				        0,
				    };

	CLOCK_SetSimSafeDivs();

	CLOCK_EnableClock(SCREEN_SPI_CLOCK);
	CLOCK_EnableClock(CTRL_PINS_CLOCK);

	// SPI pins:
	PORT_SetPinMux(SCREEN_SPI_PORT, DISPLAY_CE_PIN, SPI_MUX_ALT);
	PORT_SetPinMux(SCREEN_SPI_PORT, SPI_CLK_PIN,    SPI_MUX_ALT);
	PORT_SetPinMux(SCREEN_SPI_PORT, SPI_MOSI_PIN,   SPI_MUX_ALT);

    // Reset and data/command pin config:
	GPIO_PinInit(CTRL_PINS_GPIO, DATA_OR_CMD_PIN, &gpio_output_config);
	GPIO_PinInit(CTRL_PINS_GPIO, RESET_PIN,       &gpio_output_config);
	PORT_SetPinMux(CTRL_PINS_PORT, DATA_OR_CMD_PIN, kPORT_MuxAsGpio);
	PORT_SetPinMux(CTRL_PINS_PORT, RESET_PIN,       kPORT_MuxAsGpio);

	// SPI config:
	masterConfig.whichCtar 								  = DISPLAY_CTAR;
	masterConfig.ctarConfig.baudRate 					  = DISPLAY_BAUDRATE;
	masterConfig.ctarConfig.bitsPerFrame 				  = 8U;
	masterConfig.ctarConfig.cpol 						  = DISPLAY_CPOL;
	masterConfig.ctarConfig.cpha 						  = DISPLAY_CPHA;
	masterConfig.ctarConfig.direction 					  = kDSPI_MsbFirst;
	masterConfig.ctarConfig.pcsToSckDelayInNanoSec 		  = LCD_NANOSEC_DELAY;
	masterConfig.ctarConfig.lastSckToPcsDelayInNanoSec 	  = LCD_NANOSEC_DELAY;
	masterConfig.ctarConfig.betweenTransferDelayInNanoSec = LCD_NANOSEC_DELAY;

	masterConfig.whichPcs 			= kDSPI_Pcs0;
	masterConfig.pcsActiveHighOrLow = kDSPI_PcsActiveLow;

	masterConfig.enableContinuousSCK 		= false;
	masterConfig.enableRxFifoOverWrite 		= false;
	masterConfig.enableModifiedTimingFormat = false;
	masterConfig.samplePoint 				= kDSPI_SckToSin0Clock;

	srcClock_Hz = CLOCK_GetFreq(DSPI0_CLK_SRC);

	DSPI_MasterInit(SPI0, &masterConfig, srcClock_Hz);

	// Reset the display:
	GPIO_PortClear(CTRL_PINS_GPIO, 1u << RESET_PIN);
	SDK_DelayAtLeastUs(20, 21000000U);
	GPIO_PortSet(CTRL_PINS_GPIO,   1u << RESET_PIN);
	SDK_DelayAtLeastUs(5000, 21000000U);

}


void Display_send_command(uint8_t command, uint8_t * args, uint8_t arg_num)
{
	dspi_transfer_t masterXfer;
	uint8_t tx_command[1] = {0};

	tx_command[0] = command;

	GPIO_PortClear(CTRL_PINS_GPIO, 1u << DATA_OR_CMD_PIN);

	/* Start master transfer, send data to slave */
	masterXfer.txData      = tx_command;
	masterXfer.rxData      = NULL;
	masterXfer.dataSize    = 1;
	masterXfer.configFlags = kDSPI_MasterCtar0 | kDSPI_MasterPcs0 | kDSPI_MasterPcsContinuous;
	DSPI_MasterTransferBlocking(SPI0, &masterXfer);

	GPIO_PortSet(CTRL_PINS_GPIO, 1u << DATA_OR_CMD_PIN);

	if (arg_num)
	{
		/* Start master transfer, send data to slave */
		masterXfer.txData      = args;
		masterXfer.rxData      = NULL;
		masterXfer.dataSize    = arg_num;
		masterXfer.configFlags = kDSPI_MasterCtar0 | kDSPI_MasterPcs0 | kDSPI_MasterPcsContinuous;
		DSPI_MasterTransferBlocking(SPI0, &masterXfer);
	}
}


void Display_init(void)
{
	dspi_transfer_t masterXfer;
	uint8_t wake_turn_on[2] = {EXIT_SLEEP, DISPLAY_ON};
	uint8_t i = 0;
	uint8_t command = 0;
	uint8_t cmd_size = 0;

	for (i=0; i<INIT_SEQ_SIZE; i++)
	{
		command = g_init_sequence[i];
		i++;
		cmd_size = g_init_sequence[i];
		i++;
		Display_send_command(command, &g_init_sequence[i], cmd_size);
		i += cmd_size;
	}

	GPIO_PortClear(CTRL_PINS_GPIO, 1u << DATA_OR_CMD_PIN);
	masterXfer.txData      = &wake_turn_on[0];
	masterXfer.rxData      = NULL;
	masterXfer.dataSize    = 1;
	masterXfer.configFlags = kDSPI_MasterCtar0 | kDSPI_MasterPcs0 | kDSPI_MasterPcsContinuous;
	DSPI_MasterTransferBlocking(SPI0, &masterXfer);
	GPIO_PortSet(CTRL_PINS_GPIO, 1u << DATA_OR_CMD_PIN);

	SDK_DelayAtLeastUs(150000U, 21000000U);

	GPIO_PortClear(CTRL_PINS_GPIO, 1u << DATA_OR_CMD_PIN);
	masterXfer.txData      = &wake_turn_on[1];
	masterXfer.rxData      = NULL;
	masterXfer.dataSize    = 1;
	masterXfer.configFlags = kDSPI_MasterCtar0 | kDSPI_MasterPcs0 | kDSPI_MasterPcsContinuous;
	DSPI_MasterTransferBlocking(SPI0, &masterXfer);
	GPIO_PortSet(CTRL_PINS_GPIO, 1u << DATA_OR_CMD_PIN);

	SDK_DelayAtLeastUs(150000U, 21000000U);
}


void Display_fill_screen(RGB_pixel_t color)
{
	dspi_transfer_t masterXfer;
	uint8_t x_limits[] = {0x00, 0x50, 0x00, 0xF0};
	uint8_t y_limits[] = {0x00, 0x3C, 0x00, 0xB4};
	uint8_t pixels[2] = {(color.red << 3) | (color.green >> 3), ((color.green & 0x3) << 5) | (color.blue)};
	uint32_t i = 0;

	// Set AddrWindow from 0,0 to 319,239 (whole screen):
	Display_send_command(ILI9341_CASET, y_limits, 4);
	Display_send_command(ILI9341_PASET, x_limits, 4);
	Display_send_command(ILI9341_RAMWR, 0, 0);

	// Send 320*240 times the color info in intervals of 50000:
	for (i=0; i<50000U; i++)
	{
		masterXfer.txData      = pixels;
		masterXfer.rxData      = NULL;
		masterXfer.dataSize    = 2;
		masterXfer.configFlags = kDSPI_MasterCtar0 | kDSPI_MasterPcs0 | kDSPI_MasterPcsContinuous;
		DSPI_MasterTransferBlocking(SPI0, &masterXfer);
	}

	SDK_DelayAtLeastUs(1000, 21000000U);

	pixels[0] = 0x07;
	pixels[1] = 0xFF;

	for (i=0; i<19200U; i++)
	{
		masterXfer.txData      = pixels;
		masterXfer.rxData      = NULL;
		masterXfer.dataSize    = 2;
		masterXfer.configFlags = kDSPI_MasterCtar0 | kDSPI_MasterPcs0 | kDSPI_MasterPcsContinuous;
		DSPI_MasterTransferBlocking(SPI0, &masterXfer);
	}

}
