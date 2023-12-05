#ifndef HAL_CONF_H
#define HAL_CONF_H

// #include "stm32f4xx_hal.h"

// #define HAL_GPIO_PORT_TYPEDEF   GPIO_HandleTypeDef
// #define HAL_I2C_TYPEDEF         I2C_HandleTypeDef
// #define HAL_UART_TYPEDEF        UART_HandleTypeDef

#ifdef HAL_GPIO_PORT_TYPEDEF
    #define GPIO_PORT_A             GPIOA
    #define GPIO_PORT_B             GPIOB
    #define GPIO_PORT_C             GPIOC
    #define GPIO_PORT_D             GPIOD
    #define GPIO_PORT_E             GPIOE
    #define GPIO_PORT_F             GPIOF
    #define GPIO_PORT_G             GPIOG
    #define GPIO_PORT_H             GPIOH
#endif

#ifdef HAL_I2C_TYPEDEF
    extern I2C_HandleTypeDef* hi2c1;
    extern I2C_HandleTypeDef* hi2c2;
    extern I2C_HandleTypeDef* hi2c3;
    extern I2C_HandleTypeDef* hi2c4;

    #define I2C_1    hi2c1
    #define I2C_2    hi2c2
    #define I2C_3    hi2c3
    #define I2C_4    hi2c4
#endif

#ifdef HAL_UART_TYPEDEF
    extern UART_HandleTypeDef* huart1;
    extern UART_HandleTypeDef* huart2;
    extern UART_HandleTypeDef* huart3;
    extern UART_HandleTypeDef* huart4;

    #define UART_1    huart1
    #define UART_2    huart2
    #define UART_3    huart3
    #define UART_4    huart4
#endif

#endif /* HAL_CONF_H */