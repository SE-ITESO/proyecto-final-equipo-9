/*
 * @file     gpio.h
 *
 * @Authors  Juan Pablo Villanueva
 *           Jose Angel Gonzalez
 *
 * @brief    This file includes the developed functions related to
 *           manipulating GPIO registers. Some of the included
 *           functions are setting, clearing and toggling individual
 *           bits, writing a word onto a port, pin configuration
 *           and callback related functions, etc.
 */

#include "gpio.h"

/*
 * ******************************************************************
 * Global variables:
 * ******************************************************************
 */

// Bit field where the software interrupt flags are recorded.
volatile static gpio_interrupt_flags_t g_intr_status_flag = {0};

volatile static uint32_t g_intr_portd_flag = 0;

// Interrupt callback functions.
static void (*gpio_A_callback)(uint32_t flags) = 0;
static void (*gpio_B_callback)(uint32_t flags) = 0;
static void (*gpio_C_callback)(uint32_t flags) = 0;
static void (*gpio_D_callback)(uint32_t flags) = 0;
static void (*gpio_E_callback)(uint32_t flags) = 0;


/*
 * ******************************************************************
 * Function code:
 * ******************************************************************
 */

/*!
 * brief  Returns the software interrupt flag of a GPIO. Used when
 *        attending interrupts via getters and setters (when no
 *        interrupt callback has been initialized). Return value is
 *        either a 1 or a 0.
 *
 * param gpio Flag to be read (GPIO A, B, C, D or E).
 */
uint8_t GPIO_get_irq_status(gpio_name_t gpio)
{
	uint8_t status = 0;


	switch (gpio)
	{
	    case GPIO_A:
		    status = g_intr_status_flag.flag_port_a;
		    break;
	    case GPIO_B:
		    status = g_intr_status_flag.flag_port_b;
		    break;
	    case GPIO_C:
		    status = g_intr_status_flag.flag_port_c;
		    break;
	    case GPIO_D:
		    status = g_intr_status_flag.flag_port_d;
		    break;
	    case GPIO_E:
		    status = g_intr_status_flag.flag_port_e;
		    break;
	    default:
	    	status = 0;
	}
	return(status);

}


/*!
 * brief Clears the software interrupt flag of a GPIO. Used when
 *       attending interrupts via getters and setters (when no
 *       interrupt callback has been initialized).
 *
 * param gpio Flag to be cleared (GPIO A, B, C, D or E).
 */
void GPIO_clear_irq_status(gpio_name_t gpio)
{
	switch (gpio)
	{
	    case GPIO_A:
	    	g_intr_status_flag.flag_port_a = false;
		    break;
	    case GPIO_B:
	    	g_intr_status_flag.flag_port_b = false;
		    break;
	    case GPIO_C:
	    	g_intr_status_flag.flag_port_c = false;
		    break;
	    case GPIO_D:
	    	g_intr_status_flag.flag_port_d = false;
		    break;
	    case GPIO_E:
	    	g_intr_status_flag.flag_port_e = false;
		    break;
	    default:
	    	break;

	}
}


/*
 * @brief  Tells the user which pins triggered an interrupt in the indicated
 *         GPIO. Used as a "getter" function.
 *
 * @param  gpio Flag to be read (GPIO A, B, C, D or E).
 *
 * @retval Returns the value of the hardware register that keeps track of the
 *         GPIO's interrupt.
 */
uint32_t GPIO_get_irq_flag(gpio_name_t gpio)
{
	uint32_t status = 0;

	switch (gpio)
	{
	    case GPIO_D:
		    status = g_intr_portd_flag;
		    break;
	    default:
	    	status = 0;
	}
	return(status);
}


/*
 * @brief  Sets the interrupt flag that indicates which pin in the port
 *         triggered an interrupt. Used as a "setter" function.
 *
 * @param gpio Flag to be cleared (GPIO A, B, C, D or E).
 */
void GPIO_clear_irq_flag(gpio_name_t gpio)
{
	switch (gpio)
	{
	    case GPIO_D:
		    g_intr_portd_flag = 0;
		break;
	    default:
	    break;
	}
}


/*!
 * brief Sets the callback function to be executed when the specified
 *        GPIO generates an interruption. The "flags" argument of said
 *        function is always the corresponding interrupt flag register
 *        to help determine which pin triggered the interruption.
 *
 * param gpio    GPIO for whose interruptions the callback function will
 *                be executed.
 * param handler Function that will serve the role of a callback.
 */
void GPIO_callback_init(gpio_name_t gpio, void (*handler)(uint32_t flags))
{
	if(GPIO_A == gpio)
	{
		gpio_A_callback = handler;
	}
	if(GPIO_B == gpio)
	{
		gpio_B_callback = handler;
	}
	if(GPIO_C == gpio)
	{
		gpio_C_callback = handler;
	}
	if(GPIO_D == gpio)
	{
		gpio_D_callback = handler;
	}
	if(GPIO_E == gpio)
	{
		gpio_E_callback = handler;
	}
}


/*
 * ******************************************************************
 * ******************************************************************
 * Interrupt handlers:
 * ******************************************************************
 * ******************************************************************
 */

/*
 * All handlers follow the same logic: if a callback functions has
 * been initialized, it is executed. Otherwise, the software interrupt
 * flags are set, and it's up to the user to read them.
 */

void PORTA_IRQHandler(void)
{
	uint32_t irq_status = 0;

	if(gpio_A_callback)
	{
		irq_status = GPIO_PortGetInterruptFlags(GPIOA);
		gpio_A_callback(irq_status);
	}
	else
	{
		g_intr_status_flag.flag_port_a = true;
	}
	GPIO_PortClearInterruptFlags(GPIOA, 0xFFFF);
}

void PORTB_IRQHandler(void)
{
	uint32_t irq_status = 0;

	if(gpio_B_callback)
	{
		irq_status = GPIO_PortGetInterruptFlags(GPIOB);
		gpio_B_callback(irq_status);
	}
	else
	{
		g_intr_status_flag.flag_port_b = true;
	}
	GPIO_PortClearInterruptFlags(GPIOB, 0xFFFFFF);
}

void PORTC_IRQHandler(void)
{
	uint32_t irq_status = 0;

	if(gpio_C_callback)
	{
		irq_status = GPIO_PortGetInterruptFlags(GPIOC);
		gpio_C_callback(irq_status);
	}
	else
	{
		g_intr_status_flag.flag_port_c = true;
	}
	GPIO_PortClearInterruptFlags(GPIOC, 0xFFFFFF);
}

void PORTD_IRQHandler(void)
{
	uint32_t irq_status = 0;

	if(gpio_D_callback)
	{
		irq_status = GPIO_PortGetInterruptFlags(GPIOD);
		gpio_D_callback(irq_status);
	}
	else
	{
		g_intr_status_flag.flag_port_d = true;
		g_intr_portd_flag = GPIO_PortGetInterruptFlags(GPIOD);
	}
	GPIO_PortClearInterruptFlags(GPIOD, 0xFFFF);
}

void PORTE_IRQHandler(void)
{
	uint32_t irq_status = 0;

	if(gpio_E_callback)
	{
		irq_status = GPIO_PortGetInterruptFlags(GPIOE);
		gpio_E_callback(irq_status);
	}
	else
	{
		g_intr_status_flag.flag_port_e = true;
	}
	GPIO_PortClearInterruptFlags(GPIOE, 0xFFFF);
}
