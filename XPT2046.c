/*
 * @file     XPT2046.c
 *
 * @Authors  Juan Pablo Villanueva
 *           Jose Angel Gonzalez
 *
 * @brief    Source file containing the function code related to the use
 *           of the XPT2046 controller, including initialization, detecting a
 *           touch input, and calculating said input's coordinates.
 */

#include "XPT2046.h"

/*
 * ******************************************************************
 * Private function prototypes:
 * ******************************************************************
 */

static void Touch_gpio_irq(uint32_t port_flags);
static void Touch_debounce(void);
static uint16_t send_halfduplex_command(uint8_t command);

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
 * @brief: Configures and initializes the peripherals used (SPI, PIT, GPIO)
 */
void Touch_config_peripherals(void)
{
	dspi_master_config_t masterConfig;
	gpio_pin_config_t input_config = {
			kGPIO_DigitalInput,
			0
	};
	pit_config_t pit_config;
	uint32_t srcClock_Hz;

	CLOCK_SetSimSafeDivs();

	CLOCK_EnableClock(TOUCH_SPI_CLOCK);
	CLOCK_EnableClock(TOUCH_CE_CLOCK);
	CLOCK_EnableClock(kCLOCK_PortB);

	// PIT config:
	PIT_GetDefaultConfig(&pit_config);
	PIT_Init(PIT, &pit_config);
	PIT_SetTimerPeriod(PIT, TOUCH_PIT_CHNL, TOUCH_DELAY);
	PIT_EnableInterrupts(PIT, TOUCH_PIT_CHNL, kPIT_TimerInterruptEnable);
	PIT_callback_init(TOUCH_PIT_CHNL, Touch_debounce);
	NVIC_enable_interrupt_and_priotity(TOUCH_PIT_IRQ, TOUCH_PIT_PRIO);

	// SPI pins:
	PORT_SetPinMux(TOUCH_CE_PORT,  TOUCH_CE_PIN, SPI_MUX_ALT);
	PORT_SetPinMux(TOUCH_SPI_PORT, SPI_CLK_PIN,  SPI_MUX_ALT);
	PORT_SetPinMux(TOUCH_SPI_PORT, SPI_MOSI_PIN, SPI_MUX_ALT);
	PORT_SetPinMux(TOUCH_SPI_PORT, SPI_MISO_PIN, SPI_MUX_ALT);

	// Touch IRQ pin:
	PORT_SetPinMux(PORTB, TOUCH_IRQ_PIN, kPORT_MuxAsGpio);
	GPIO_PinInit(TOUCH_IRQ_GPIO, TOUCH_IRQ_PIN, &input_config);
	PORT_SetPinInterruptConfig(PORTB, TOUCH_IRQ_PIN, kPORT_InterruptFallingEdge);
	NVIC_enable_interrupt_and_priotity(PORTB_IRQ, PRIORITY_3);
	GPIO_callback_init(GPIO_B, Touch_gpio_irq);

	// SPI config:
	masterConfig.whichCtar 								  = TOUCH_CTAR;
	masterConfig.ctarConfig.baudRate 					  = TOUCH_BAUDRATE;
	masterConfig.ctarConfig.bitsPerFrame 				  = 8U;
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
 * @brief: Indicates whether the screen has been touched.
 *
 * @retval: true if the screen has been touched, false otherwise.
 */
bool Touch_pressed(void)
{
	return g_touch_irq;
}


/*
 * @brief: Sets the value of the flag that indicates whether the screen has
 *         been touched to false, avoiding multiple responses to the same
 *         touch. Should be used after Touch_pressed().
 */
void Touch_clear_irq_flag(void)
{
	g_touch_irq = false;
}


/*
 * @brief: When the screen has been touched, returns the screen coordinates
 *         where it was touched.
 *
 * @retval: structure containing the x and y coordinates of the touched point.
 */
Coordinate_t Touch_get_coordinates(void)
{
	Coordinate_t touch_spot = {0};
	uint16_t x_coord[3] = {0};
	uint16_t y_coord[3] = {0};
	uint16_t x_rectified = 0;
	uint16_t y_rectified = 0;

	// The command for getting an x-value is 0x91, and 0xD1 for y-value.
	x_coord[0] = send_halfduplex_command(0x91);
	x_coord[0] = send_halfduplex_command(0x91);
	y_coord[0] = send_halfduplex_command(0xD1);
	x_coord[1] = send_halfduplex_command(0x91);
	y_coord[1] = send_halfduplex_command(0xD1);
	x_coord[2] = send_halfduplex_command(0x91);
	// The last command is different in order to enter low-power mode.
	y_coord[2] = send_halfduplex_command(0xD0);

	// Gets an average of the 3 points obtained.
	x_rectified = (x_coord[0] + x_coord[1] + x_coord[2]) / 3;
	y_rectified = (y_coord[0] + y_coord[1] + y_coord[2]) / 3;

	// Calculate coordinates considering the origin at the top left corner:
	if (x_rectified > 2000) x_rectified = 2000;
	if (y_rectified > 1600) y_rectified = 1600;
	x_rectified = (2000 - x_rectified) / 5;
	y_rectified = (1600 - y_rectified) / 5;

	touch_spot.x_position = x_rectified;
	touch_spot.y_position = y_rectified;

	return touch_spot;
}


/*
 * The following function code corresponds to private (static) functions:
 */

/*
 * @brief: Used as a callback function for the touch IRQ port. Sets the IRQ
 *         flag and disables the port's interrupt for debouncing.
 *
 * @param: port_flags Determines which pin in the port triggered the interrupt
 */
static void Touch_gpio_irq(uint32_t port_flags)
{
	// Only acts if the touch IRQ pin triggered the interrupt:
	if (port_flags & (1 << TOUCH_IRQ_PIN))
	{
		// Set the global IRQ flag:
		g_touch_irq = true;
		// Disabled the port's interrupt for 5 ms as debouncing.
		PORT_SetPinInterruptConfig(PORTB, TOUCH_IRQ_PIN, kPORT_InterruptOrDMADisabled);
		PIT_StartTimer(PIT, TOUCH_PIT_CHNL);
	}
}


/*
 * @brief: Function used as a PIT callback function. Enables the touch IRQ
 *         port's interrupt again.
 */
static void Touch_debounce(void)
{
	PIT_StopTimer(PIT, TOUCH_PIT_CHNL);
	PORT_SetPinInterruptConfig(PORTB, TOUCH_IRQ_PIN, kPORT_InterruptFallingEdge);
}


/*
 * @brief: Function with internal purposes for communicating with the XPT2046
 *         controller. Sends a 1-byte command and receives a 12-bit data.
 *
 * @param: command 1-byte command to be sent to the touch controller.
 *
 * @retval: returns the 12-bit response from the controller.
 */
static uint16_t send_halfduplex_command(uint8_t command)
{
    dspi_half_duplex_transfer_t masterXfer;
    uint8_t command_buffer[] = {command};
    uint8_t rx_buffer[2] = {0};

	masterXfer.txData      = command_buffer;
	masterXfer.rxData      = rx_buffer;
	masterXfer.txDataSize  = 1;
	masterXfer.rxDataSize  = 2;
	masterXfer.configFlags = kDSPI_MasterCtar1 | kDSPI_MasterPcs1 | kDSPI_MasterPcsContinuous;
	masterXfer.isPcsAssertInTransfer = true;
	masterXfer.isTransmitFirst       = true;
	DSPI_MasterHalfDuplexTransferBlocking(SPI0, &masterXfer);

	// Concatenates the two bytes into their corresponding 12-bit value:
	return (uint16_t)((rx_buffer[0] << 4) | (rx_buffer[1] >> 4));
}


