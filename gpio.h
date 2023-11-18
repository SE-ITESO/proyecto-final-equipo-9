/*
 * @file     gpio.h
 *
 * @Authors  Juan Pablo Villanueva
 *           Jose Angel Gonzalez
 *
 * @brief    Header file for the GPIO manipulation driver.
 *           Includes definitions of important register addresses,
 *           configuration types and values, interrupt callback
 *           support, and function declarations.
 */

#ifndef GPIO_H_
#define GPIO_H_


#include <stdint.h>
#include <stdbool.h>
#include "MK64F12.h"
#include "fsl_gpio.h"

/*
 * ******************************************************************
 * Structs and enums:
 * ******************************************************************
 */

/* Enum for GPIO names */
typedef enum{
	GPIO_A,
	GPIO_B,
	GPIO_C,
	GPIO_D,
	GPIO_E
}gpio_name_t;

// Definition of the struct used for interrupt flags:
typedef struct
{
	uint8_t flag_port_a : 1;
	uint8_t flag_port_b : 1;
	uint8_t flag_port_c : 1;
	uint8_t flag_port_d : 1;
	uint8_t flag_port_e : 1;
} gpio_interrupt_flags_t;


/*
 * ******************************************************************
 * ******************************************************************
 * Function prototypes:
 * ******************************************************************
 * ******************************************************************
 */


/*!
 * @brief Returns the software interrupt flag of a GPIO. Used when
 *        attending interrupts via getters and setters (when no
 *        interrupt callback has been initialized). Return value is
 *        either a 1 or a 0.
 *
 * @param gpio Flag to be read (GPIO A, B, C, D or E).
 */
uint8_t GPIO_get_irq_status(gpio_name_t gpio);


/*!
 * @brief Clears the software interrupt flag of a GPIO. Used when
 *        attending interrupts via getters and setters (when no
 *        interrupt callback has been initialized).
 *
 * @param gpio Flag to be cleared (GPIO A, B, C, D or E).
 */
void GPIO_clear_irq_status(gpio_name_t gpio);


/*!
 * @brief Sets the callback function to be executed when the specified
 *        GPIO generates an interruption. The "flags" argument of said
 *        function is always the corresponding interrupt flag register
 *        to help determine which pin triggered the interruption.
 *
 * @param gpio    GPIO for whose interruptions the callback function will
 *                be executed.
 * @param handler Function that will serve the role of a callback.
 */
void GPIO_callback_init(gpio_name_t gpio, void (*handler)(uint32_t flags));


/*
 * @brief  Tells the user which pins triggered an interrupt in the indicated
 *         GPIO. Used as a "getter" function.
 *
 * @param  gpio Flag to be read (GPIO A, B, C, D or E).
 *
 * @retval Returns the value of the hardware register that keeps track of the
 *         GPIO's interrupt.
 */
uint32_t GPIO_get_irq_flag(gpio_name_t gpio);


/*
 * @brief  Sets the interrupt flag that indicates which pin in the port
 *         triggered an interrupt. Used as a "setter" function.
 *
 * @param gpio Flag to be cleared (GPIO A, B, C, D or E).
 */
void GPIO_clear_irq_flag(gpio_name_t gpio);


#endif /* GPIO_H_ */
