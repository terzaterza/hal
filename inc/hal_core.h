#ifndef HAL_CORE_H
#define HAL_CORE_H

#if defined(HAL_TARGET_PC)
#include "hal_target_pc.h"
#elif defined(HAL_TARGET_STM32F4)
#include "hal_target_stm32f4.h"
#elif defined(HAL_TARGET_STM32L4)
#include "hal_target_stm32l4.h"
#endif

/**
 * Get HAL resource (gpio, i2c, uart,...) by name
 * Requires user to define what this resource is by using `HAL_SET_RESOURCE`
 * 
 * @example HAL_GET_RESOURCE(uart_t, TELEMETRY_UART)
 * @example HAL_GET_RESOURCE(gpio_port_t, ENCODER_INTERRUPT_PORT)
 * 
 * @todo Check if can make this look for defines first before declaring as extern
*/
#define HAL_GET_RESOURCE(type, name)	extern const type name

/**
 * Set the resource reference for a specific target
 * 
 * @example HAL_SET_RESOURCE(uart_t, TELEMETRY_UART, &handle_uart1)
 * 
 * @todo Check if can infer resource type from HAL_GET_RESOURCE
*/
#define HAL_SET_RESOURCE(type, name, res)	const type name = (type)res

/**
 * Remove compiler warnings for unused function arguments
*/
#ifndef UNUSED
#define UNUSED(x)				(void)x
#endif

/**
 * Define hal types as `void*` so apps can be written while target not specified
*/
#define __HAL_TEMPLATE_TYPEDEF(x)		typedef void* x

typedef enum {
	HAL_STATUS_OK,
	HAL_STATUS_ERROR,
	HAL_STATUS_BUSY
} hal_status_t;

typedef void (*callback_t)(hal_status_t);

#endif /* HAL_CORE_H */
