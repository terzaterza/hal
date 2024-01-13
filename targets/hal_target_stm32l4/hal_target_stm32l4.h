#ifndef HAL_TARGET_STM32L4_H
#define HAL_TARGET_STM32L4_H

#ifndef STM32L476xx
/* This block is used when writing the hal library */
/* In app development IDE will already have these defined globally */
#define STM32
#define STM32L476xx
#define USE_HAL_DRIVER
#endif

#include "stm32l4xx.h"
#include "stm32l4xx_hal_gpio.h"
#include "stm32l4xx_hal_i2c.h"
#include "stm32l4xx_hal_uart.h"
#include "stm32l4xx_hal_tim.h"
#include "stm32l4xx_hal_adc.h"
#include "stm32l4xx_hal.h"

#define HAL_GPIO_16BIT
#define HAL_TIMER_32BIT /* Not all timers are 32-bit so be careful */
#define HAL_GPIO_PORT_TYPEDEF   GPIO_TypeDef*
#define HAL_I2C_TYPEDEF         I2C_HandleTypeDef*
#define HAL_UART_TYPEDEF        UART_HandleTypeDef*
#define HAL_TIMER_TYPEDEF       TIM_HandleTypeDef*
#define HAL_ADC_TYPEDEF         ADC_HandleTypeDef*

#endif