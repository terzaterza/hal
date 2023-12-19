#ifndef HAL_TARGET_PC_H
#define HAL_TARGET_PC_H

#include <stdint.h>
#include <stdlib.h>

#define SOCKET_PORT     8080
#define SOCKET_GPIO_ID  0
#define SOCKET_I2C_ID   1
#define SOCKET_UART_ID  2
#define SOCKET_TIMER_ID 3

#define HAL_GPIO_16BIT
#define HAL_GPIO_PORT_TYPEDEF   hal_target_pc_gpio_t*
#define HAL_GPIO_PORT_COUNT     8

#define SOCKET_I2C_START    0x5a
#define SOCKET_I2C_STOP     0xa5
#define SOCKET_I2C_ACK      0xaa
// #define SOCKET_I2C_NACK     0x55

int socket_read(void* buffer, size_t size);
int socket_write(uint8_t periph_id, const void* data, size_t size);

typedef struct {
    uint16_t in_reg;
    uint16_t out_reg;
    uint16_t dir; /* not used currently since hal doesn't have options to set pin dir */
    uint16_t intr; /* used to check which pin triggers an interrupt */
    uint16_t intr_edge; /* which edge for each pin triggers an interrupt (0 - falling, 1 - rising) */
} hal_target_pc_gpio_t;

#endif