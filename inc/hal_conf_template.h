/**
 * TEMPLATE FILE FOR CONFIG
 * 
 * Rename to hal_conf.h when adding to project
*/

#ifndef HAL_CONF_H
#define HAL_CONF_H

#include "stm32f4xx_hal.h"

#define HAL_GPIO_PORT_TYPEDEF   GPIO_HandleTypeDef*
#define HAL_I2C_TYPEDEF         I2C_HandleTypeDef*
#define HAL_UART_TYPEDEF        UART_HandleTypeDef*

// #define HAL_I2C_USE_REGISTER_CALLBACKS
// #define HAL_UART_USE_REGISTER_CALLBACKS

#define GPIO_PORT_INT_1         GPIOA
#define GPIO_PORT_LED_R         GPIOB
#define GPIO_PORT_LED_G         GPIOB
#define GPIO_PORT_BUTTON        GPIOD


extern HAL_I2C_TYPEDEF hi2c1;
extern HAL_I2C_TYPEDEF hi2c2;
extern HAL_I2C_TYPEDEF hi2c3;

#define I2C_ENCODER     hi2c1
#define I2C_LED_DRIVER  hi2c2
#define I2C_POT         hi2c3


extern HAL_UART_TYPEDEF huart1;
extern HAL_UART_TYPEDEF huart2;
extern HAL_UART_TYPEDEF huart3;
extern HAL_UART_TYPEDEF huart4;

#define UART_1    huart1
#define UART_2    huart2
#define UART_3    huart3
#define UART_4    huart4

#endif /* HAL_CONF_H */