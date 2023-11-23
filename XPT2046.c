/*
 *
 */

#include "XPT2046.h"

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

	// SPI pins:
	PORT_SetPinMux(TOUCH_CE_PORT,  TOUCH_CE_PIN, SPI_MUX_ALT);
	PORT_SetPinMux(TOUCH_SPI_PORT, SPI_CLK_PIN,  SPI_MUX_ALT);
	PORT_SetPinMux(TOUCH_SPI_PORT, SPI_MOSI_PIN, SPI_MUX_ALT);

	// IRQ pin:
	PORT_SetPinMux(TOUCH_CE_PORT, TOUCH_IRQ_PIN, kPORT_MuxAsGpio);
	GPIO_PinInit(TOUCH_IRQ_GPIO, TOUCH_IRQ_PIN, &input_config);
	PORT_SetPinInterruptConfig(TOUCH_CE_PORT, TOUCH_IRQ_PIN, kPORT_InterruptFallingEdge);


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
 *
 */
bool Touch_pressed(void)
{

}


/*
 *
 */
Coordinate_t Touch_get_coordinates(void)
{

}
