#ifndef HAL_GPIO_H
#define HAL_GPIO_H

#include <stdint.h>
#include "hal_core.h"

#ifndef HAL_GPIO_PORT_TYPEDEF
	#error "HAL_GPIO_PORT_TYPEDEF not defined"
	__HAL_TEMPLATE_TYPEDEF(gpio_port_t);
#else
	typedef HAL_GPIO_PORT_TYPEDEF gpio_port_t;
#endif

#ifdef HAL_GPIO_16BIT
	typedef uint16_t gpio_pin_t;
#else
	typedef uint8_t gpio_pin_t;
#endif

/**
 * Read gpio port
 * @note Implement in hal_gpio.c
 */
gpio_pin_t gpio_port_read(gpio_port_t port);

/**
 * Set GPIO pins to 1
 * @note Implement in hal_gpio.c
 */
void gpio_port_set(gpio_port_t port, gpio_pin_t pins);

/**
 * Set GPIO pins to 0
 * @note Implement in hal_gpio.c
 */
void gpio_port_clear(gpio_port_t port, gpio_pin_t pins);

/**
 * Toggle GPIO pins
 * @note Implement in hal_gpio.c
 */
void gpio_port_toggle(gpio_port_t port, gpio_pin_t pins);

#ifdef HAL_GPIO_USE_REGISTER_CALLBACKS
/**
 * Register a callback for GPIO external interrupt
 * @retval `HAL_STATUS_OK` callback registered successfully
 * @retval `HAL_STATUS_ERROR` callback registration failed
 * @note Implement in hal_gpio.c
 * @note Multiple registrations should override the last one
*/
hal_status_t gpio_register_callback(gpio_t gpio, gpio_pin_t pin, callback_t callback);
#else
/**
 * GPIO external interrupt ISR
 * @note This function should be called from lower level (driver's) ISR in the hal_gpio.c
 */
void gpio_exti_isr(gpio_port_t port, gpio_pin_t pin);
#endif

#endif /* HAL_GPIO_H */
