#ifndef HAL_TARGET_PC_H
#define HAL_TARGET_PC_H

#include <stdint.h>
#include <stdlib.h>

#define SOCKET_PORT     8080

typedef enum {
    SOCKET_PERIPH_GPIO,
    SOCKET_PERIPH_I2C,
    SOCKET_PERIPH_UART,
    SOCKET_PERIPH_TIMER
} socket_periph_t;

typedef struct {
    uint8_t id;
    uint16_t in_reg;
    uint16_t out_reg;
    uint16_t dir; /* not used currently since hal doesn't have options to set pin dir */
    uint16_t intr; /* used to check which pin triggers an interrupt */
    uint16_t intr_edge; /* which edge for each pin triggers an interrupt (0 - falling, 1 - rising) */
} hal_target_pc_gpio_t;

#define HAL_GPIO_PORT_TYPEDEF   hal_target_pc_gpio_t*
#define HAL_GPIO_16BIT

typedef enum {
    I2C_READY,
    I2C_SENDING,
    I2C_RECEIVING,
    I2C_FINISHED_OK,
    I2C_FINISHED_ERROR
} i2c_status_t;

/** @note volatile variables might be needed */
typedef struct {
    uint8_t id;
    /* Set before START byte is sent (or received if working as slave) */
    i2c_status_t status;
    /* Pointer to buffer from which data is transmited while i2c_tx_len > 0 (autoincremented) */
    uint8_t* tx_buf;
    /* Pointer to where the current received byter should be stored (autoincremented) */
    uint8_t* rx_buf;
    /* How many bytes are still to be sent or received */
    uint16_t byte_count;
    /* Wait for slave to acknowledge addressing when starting reading process */
    uint8_t wait_ack;
    /* Should ISR be called on complete or if error */
    uint8_t int_mode;
    /** @todo Add registrable callbacks here */
} hal_target_pc_i2c_t;
/** @todo Could use one buffer pointer for both tx and rx */

#define HAL_I2C_TYPEDEF     hal_target_pc_i2c_t*

typedef struct {
    uint8_t id;
    /* Parity mode - 0=None, 1=Odd, 2=Even */
    uint8_t parity;
    /* Number of stop bits */
    uint8_t stop_bits;
    // uart_baud_t baud_rate; - not used currently
    /* Pointers to send/receive buffers (autoincremented) */
    uint8_t* tx_buf;
    uint8_t* rx_buf;
    /* Number of bytes left to send/receive */
    uint16_t tx_count;
    uint16_t rx_count;
    /* Should ISR be called on complete or if error */
    uint8_t int_mode;
    /** @todo Add registrable callbacks here*/
} hal_target_pc_uart_t;

#define HAL_UART_TYPEDEF    hal_target_pc_uart_t*

/**
 * Wait for `size` bytes to be received from the peripheral
 * @note Blocking
*/
int socket_read(socket_periph_t periph_type, uint8_t periph_id, void* buffer, size_t size);
/**
 * Write `size` bytes from `data`
 * @note Blocking
 * @return Number of bytes sent successfully
*/
int socket_write(socket_periph_t periph_type, uint8_t periph_id, const void* data, size_t size);

static int socket_write_byte(socket_periph_t periph_type, uint8_t periph_id, uint8_t data)
{
    uint8_t temp = data; /* could replace with &data */
    return socket_write(periph_type, periph_id, &temp, 1);
}

void peripheral_socket_handle_gpio(hal_target_pc_gpio_t* port);
void peripheral_socket_handle_i2c(hal_target_pc_i2c_t* i2c);

#endif