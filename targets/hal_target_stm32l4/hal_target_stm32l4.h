#ifndef HAL_TARGET_STM32L4_H
#define HAL_TARGET_STM32L4_H

#define DEBUG
#define STM32
#define USE_HAL_DRIVER
#define STM32L476xx

#include "stm32l4xx.h"
#include "stm32l4xx_hal_gpio.h"
#include "stm32l4xx_hal_i2c.h"
#include "stm32l4xx_hal_uart.h"
#include "stm32l4xx_hal_tim.h"

#define HAL_GPIO_16BIT
#define HAL_GPIO_PORT_TYPDEF    GPIO_TypeDef*
#define HAL_I2C_TYPEDEF         I2C_HandleTypeDef*
#define HAL_UART_TYPEDEF        UART_HandleTypeDef*
#define HAL_TIMER_TYPEDEF       TIM_HandleTypeDef*
#define HAL_VCP_USE_USB

#endif