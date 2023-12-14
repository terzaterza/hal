#ifndef HAL_GPIO_H
#define HAL_GPIO_H

#include <stdint.h>
#include "hal_conf.h"
#include "hal_core.h"

#ifndef HAL_GPIO_PORT_TYPEDEF
	#error "HAL_GPIO_PORT_TYPEDEF not defined"
#else
	typedef HAL_GPIO_PORT_TYPEDEF gpio_port_t;
#endif

#ifdef HAL_GPIO_16BIT
	typedef uint16_t gpio_pin_t;
#else
	typedef uint8_t gpio_pin_t;
#endif

#define GPIO_PIN_0	(gpio_pin_t)(0x1)
#define GPIO_PIN_1	(gpio_pin_t)(0x2)
#define GPIO_PIN_2	(gpio_pin_t)(0x4)
#define GPIO_PIN_3	(gpio_pin_t)(0x8)
#define GPIO_PIN_4	(gpio_pin_t)(0x10)
#define GPIO_PIN_5	(gpio_pin_t)(0x20)
#define GPIO_PIN_6	(gpio_pin_t)(0x40)
#define GPIO_PIN_7	(gpio_pin_t)(0x80)

#ifdef HAL_GPIO_16BIT
#define GPIO_PIN_8	(gpio_pin_t)(0x100)
#define GPIO_PIN_9	(gpio_pin_t)(0x200)
#define GPIO_PIN_10	(gpio_pin_t)(0x400)
#define GPIO_PIN_11	(gpio_pin_t)(0x800)
#define GPIO_PIN_12	(gpio_pin_t)(0x1000)
#define GPIO_PIN_13	(gpio_pin_t)(0x2000)
#define GPIO_PIN_14	(gpio_pin_t)(0x4000)
#define GPIO_PIN_15	(gpio_pin_t)(0x8000)
#endif

/**
 * Read gpio port
 * @note Implement in hal_gpio.c
 */
inline gpio_pin_t gpio_port_read(gpio_port_t port);

/**
 * Set GPIO pins to 1
 * @note Implement in hal_gpio.c
 */
inline void gpio_port_set(gpio_port_t port, gpio_pin_t pins);

/**
 * Set GPIO pins to 0
 * @note Implement in hal_gpio.c
 */
inline void gpio_port_clear(gpio_port_t port, gpio_pin_t pins);

/**
 * Toggle GPIO pins
 * @note Implement in hal_gpio.c
 */
inline void gpio_port_toggle(gpio_port_t port, gpio_pin_t pins);

#ifdef HAL_GPIO_USE_REGISTER_CALLBACKS
/**
 * Register a callback for GPIO external interrupt
 * @retval `HAL_STATUS_OK` callback registered successfully
 * @retval `HAL_STATUS_ERROR` callback registration failed
 * @note Implement in hal_gpio.c
 * @note Multiple registrations should override the last one
*/
inline hal_status_t gpio_register_callback(gpio_t gpio, gpio_pin_t pin, callback_t callback);
#else
/**
 * GPIO external interrupt ISR
 * @note This function should be called from lower level (driver's) ISR in the hal_gpio.c
 */
inline void gpio_exti_isr(gpio_port_t port, gpio_pin_t pin);
#endif

#endif /* HAL_GPIO_H */
