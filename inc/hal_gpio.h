#ifndef HAL_GPIO_H
#define HAL_GPIO_H

#include "hal_core.h"

#ifndef HAL_GPIO_PORT_TYPEDEF
	typedef enum {
		GPIO_PORT_A,
		GPIO_PORT_B,
		GPIO_PORT_C,
		GPIO_PORT_D,
		GPIO_PORT_E,
		GPIO_PORT_F,
		GPIO_PORT_G,
		GPIO_PORT_H
	} gpio_port_t;
#else
	typedef HAL_GPIO_PORT_TYPEDEF gpio_port_t;
#endif

typedef enum {
	GPIO_PIN_0 = 0x1,
	GPIO_PIN_1 = 0x2,
	GPIO_PIN_2 = 0x4,
	GPIO_PIN_3 = 0x8,
	GPIO_PIN_4 = 0x10,
	GPIO_PIN_5 = 0x20,
	GPIO_PIN_6 = 0x40,
	GPIO_PIN_7 = 0x80,
	GPIO_PIN_8 = 0x100,
	GPIO_PIN_9 = 0x200,
	GPIO_PIN_10 = 0x400,
	GPIO_PIN_11 = 0x800,
	GPIO_PIN_12 = 0x1000,
	GPIO_PIN_13 = 0x2000,
	GPIO_PIN_14 = 0x4000,
	GPIO_PIN_15 = 0x8000,
	GPIO_PIN_ALL = 0xffff
} gpio_pin_t;

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

/**
 * GPIO external interrupt ISR
 * @note This function should be called from lower level (driver's) ISR in the hal_gpio.c
 */
inline void gpio_exti_isr(gpio_port_t port, gpio_pin_t pin);

#endif /* HAL_GPIO_H */
