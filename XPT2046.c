/*
 *
 */

#include "XPT2046.h"

/*
 * ******************************************************************
 * Private function prototypes:
 * ******************************************************************
 */

static void Touch_gpio_irq(uint32_t port_flags);
static uint16_t send_halfduplex_command(uint16_t command);
static uint16_t smart_average(uint16_t a, uint16_t b, uint16_t c);

/*
 * ******************************************************************
 * Global variables:
 * ******************************************************************
 */

bool g_touch_irq = false;

/*
 * ******************************************************************
 * Function code:
 * ******************************************************************
 */

/*
 *
 */
void Touch_config_peripherals(void)
{
	dspi_master_config_t masterConfig;
	gpio_pin_config_t input_config = {
			kGPIO_DigitalInput,
			0
	};
	uint32_t srcClock_Hz;

	CLOCK_SetSimSafeDivs();

	CLOCK_EnableClock(TOUCH_SPI_CLOCK);
	CLOCK_EnableClock(TOUCH_CE_CLOCK);
	CLOCK_EnableClock(kCLOCK_PortB);

	// SPI pins:
	PORT_SetPinMux(TOUCH_CE_PORT,  TOUCH_CE_PIN, SPI_MUX_ALT);
	PORT_SetPinMux(TOUCH_SPI_PORT, SPI_CLK_PIN,  SPI_MUX_ALT);
	PORT_SetPinMux(TOUCH_SPI_PORT, SPI_MOSI_PIN, SPI_MUX_ALT);
	PORT_SetPinMux(TOUCH_SPI_PORT, SPI_MISO_PIN, SPI_MUX_ALT);

	// IRQ pin:
	PORT_SetPinMux(PORTB, TOUCH_IRQ_PIN, kPORT_MuxAsGpio);
	GPIO_PinInit(TOUCH_IRQ_GPIO, TOUCH_IRQ_PIN, &input_config);
	PORT_SetPinInterruptConfig(PORTB, TOUCH_IRQ_PIN, kPORT_InterruptFallingEdge);
	NVIC_enable_interrupt_and_priotity(PORTB_IRQ, PRIORITY_3);
	GPIO_callback_init(GPIO_B, Touch_gpio_irq);

	// SPI config:
	masterConfig.whichCtar 								  = TOUCH_CTAR;
	masterConfig.ctarConfig.baudRate 					  = TOUCH_BAUDRATE;
	masterConfig.ctarConfig.bitsPerFrame 				  = 16U;
	masterConfig.ctarConfig.cpol 						  = TOUCH_CPOL;
	masterConfig.ctarConfig.cpha 						  = TOUCH_CPHA;
	masterConfig.ctarConfig.direction 					  = kDSPI_MsbFirst;
	masterConfig.ctarConfig.pcsToSckDelayInNanoSec 		  = NANOSEC_DELAY;
	masterConfig.ctarConfig.lastSckToPcsDelayInNanoSec 	  = NANOSEC_DELAY;
	masterConfig.ctarConfig.betweenTransferDelayInNanoSec = NANOSEC_DELAY;

	masterConfig.whichPcs 			= kDSPI_Pcs1;
	masterConfig.pcsActiveHighOrLow = kDSPI_PcsActiveLow;

	masterConfig.enableContinuousSCK 		= false;
	masterConfig.enableRxFifoOverWrite 		= false;
	masterConfig.enableModifiedTimingFormat = false;
	masterConfig.samplePoint 				= kDSPI_SckToSin0Clock;

	srcClock_Hz = CLOCK_GetFreq(DSPI0_CLK_SRC);

	DSPI_MasterInit(SPI0, &masterConfig, srcClock_Hz);
}


/*
 *
 */
bool Touch_pressed(void)
{
	return g_touch_irq;
}


/*
 *
 */
void Touch_clear_irq_flag(void)
{
	g_touch_irq = false;
}


/*
 *
 */
Coordinate_t Touch_get_coordinates(void)
{
	Coordinate_t touch_spot = {0};
	uint16_t x_coord[3] = {0};
	uint16_t y_coord[3] = {0};

	if (Touch_event_threshold())
	{
		y_coord[0] = send_halfduplex_command(0x91);
		x_coord[0] = send_halfduplex_command(0xD1);
		y_coord[0] = send_halfduplex_command(0x91);
		x_coord[1] = send_halfduplex_command(0xD1);
		y_coord[1] = send_halfduplex_command(0x91);
		x_coord[2] = send_halfduplex_command(0xD0);
		y_coord[2] = send_halfduplex_command(0x00);

		touch_spot.x_position = smart_average(x_coord[0], x_coord[1], x_coord[2]);
		touch_spot.y_position = smart_average(y_coord[0], y_coord[1], y_coord[2]);
	}
	// TODO: tie the re-enabling of the interrupt to a PIT
	PORT_SetPinInterruptConfig(PORTB, TOUCH_IRQ_PIN, kPORT_InterruptFallingEdge);
	return touch_spot;
}


/*
 *
 */
bool Touch_event_threshold(void)
{
    dspi_half_duplex_transfer_t masterXfer;
    uint16_t command[3] = {0xB1, 0xC1, 0x91};
    uint16_t z_coord[3] = {0};
    uint16_t z_value = 0;

	masterXfer.txData      = (uint8_t *)&command[0];
	masterXfer.rxData      = (uint8_t *)&z_coord[0];
	masterXfer.txDataSize  = 1;
	masterXfer.rxDataSize  = 1;
	masterXfer.configFlags = kDSPI_MasterCtar1 | kDSPI_MasterPcs1 | kDSPI_MasterPcsContinuous;
	masterXfer.isPcsAssertInTransfer = true;
	masterXfer.isTransmitFirst       = true;
	DSPI_MasterHalfDuplexTransferBlocking(SPI0, &masterXfer);

	masterXfer.txData      = (uint8_t *)&command[1];
	masterXfer.rxData      = (uint8_t *)&z_coord[1];
	masterXfer.txDataSize  = 1;
	masterXfer.rxDataSize  = 1;
	masterXfer.configFlags = kDSPI_MasterCtar1 | kDSPI_MasterPcs1 | kDSPI_MasterPcsContinuous;
	masterXfer.isPcsAssertInTransfer = true;
	masterXfer.isTransmitFirst       = true;
	DSPI_MasterHalfDuplexTransferBlocking(SPI0, &masterXfer);

	masterXfer.txData      = (uint8_t *)&command[2];
	masterXfer.rxData      = (uint8_t *)&z_coord[2];
	masterXfer.txDataSize  = 1;
	masterXfer.rxDataSize  = 1;
	masterXfer.configFlags = kDSPI_MasterCtar1 | kDSPI_MasterPcs1 | kDSPI_MasterPcsContinuous;
	masterXfer.isPcsAssertInTransfer = true;
	masterXfer.isTransmitFirst       = true;
	DSPI_MasterHalfDuplexTransferBlocking(SPI0, &masterXfer);

	z_value = (z_coord[1] >> 3) - (z_coord[2] >> 3) + 4095U;

	return (z_value > 300);
}


/*
 * The following function code corresponds to private (static) functions:
 */

/*
 *
 */
static void Touch_gpio_irq(uint32_t port_flags)
{
	if (port_flags & (1 << TOUCH_IRQ_PIN))
	{
		g_touch_irq = true;
		PORT_SetPinInterruptConfig(PORTB, TOUCH_IRQ_PIN, kPORT_InterruptOrDMADisabled);
		SDK_DelayAtLeastUs(10000, 21000000U);
	}
}


/*
 *
 */
static uint16_t send_halfduplex_command(uint16_t command)
{
    dspi_half_duplex_transfer_t masterXfer;
    uint16_t command_buffer[] = {command};
    uint16_t rx_buffer[1] = {0};

	masterXfer.txData      = (uint8_t *)command_buffer;
	masterXfer.rxData      = (uint8_t *)rx_buffer;
	masterXfer.txDataSize  = 1;
	masterXfer.rxDataSize  = 1;
	masterXfer.configFlags = kDSPI_MasterCtar1 | kDSPI_MasterPcs1 | kDSPI_MasterPcsContinuous;
	masterXfer.isPcsAssertInTransfer = true;
	masterXfer.isTransmitFirst       = true;
	DSPI_MasterHalfDuplexTransferBlocking(SPI0, &masterXfer);

	return rx_buffer[0];
}


/*
 *
 */
static uint16_t smart_average(uint16_t a, uint16_t b, uint16_t c)
{
	uint16_t d_ab = 0;
	uint16_t d_ac = 0;
	uint16_t d_bc = 0;
	uint16_t avg  = 0;

	if (a > b)
	{
		d_ab = a - b;
	}
	else
	{
		d_ab = b - a;
	}
	if (a > c)
	{
		d_ac = a - c;
	}
	else
	{
		d_ac = c - a;
	}
	if (c > b)
	{
		d_bc = c - b;
	}
	else
	{
		d_bc = b - c;
	}

	if (d_ab < d_ac)
	{
		if (d_ab < d_bc)
		{
			avg = (a + b) / 2;
		}
		else
		{
			avg = (c + b) / 2;
		}
	}
	else
	{
		if (d_ac < d_bc)
		{
			avg = (a + c) / 2;
		}
		else
		{
			avg = (c + b) / 2;
		}
	}

	return avg;
}

