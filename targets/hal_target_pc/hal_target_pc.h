#ifndef HAL_TARGET_PC_H
#define HAL_TARGET_PC_H

#include <stdint.h>
#include <stdlib.h>

#define SOCKET_PORT     8080
#define SOCKET_GPIO_ID  0
#define SOCKET_I2C_ID   1
#define SOCKET_UART_ID  2
#define SOCKET_TIMER_ID 3
/** @todo Change ids to enum */

#define TARGET_PC_GPIO_PORT_COUNT   8
#define TARGET_PC_I2C_COUNT         4
#define TARGET_PC_UART_COUNT        4

#define HAL_GPIO_16BIT

int socket_read(void* buffer, size_t size);
int socket_write(uint8_t periph_id, const void* data, size_t size);

static int socket_write_byte(uint8_t periph_id, uint8_t data)
{
    uint8_t temp = data; /* could replace with &data */
    return socket_write(periph_id, &temp, 1);
}

#endif