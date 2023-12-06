#ifndef HAL_UART_H
#define HAL_UART_H

#include <stdint.h>
#include "hal_conf.h"
#include "hal_core.h"

#ifndef HAL_UART_TYPEDEF
    #error "HAL_UART_TYPEDEF not defined"
#else
    typedef HAL_UART_TYPEDEF uart_t;
#endif

/**
 * Blocking function
 * Send <size> bytes via UART
 * @note Implement in hal_uart.c
 */
inline hal_status_t uart_send(uart_t uart, uint8_t* data, uint16_t size, uint16_t timeout);

/**
 * Blocking function
 * Wait for <size> bytes via UART
 * @note Implement in hal_uart.c
 */
inline hal_status_t uart_recv(uart_t uart, uint8_t* buff, uint16_t size, uint16_t timeout);

/**
 * Send <size> bytes via UART
 * Returns true if sending process was started correctly
 * On complete uart_send_isr is called
 * @note Implement in hal_uart.c
 */
inline hal_status_t uart_send_it(uart_t uart, uint8_t* data, uint16_t size, uint16_t timeout);

/**
 * Receive <size> bytes via UART
 * Returns true if UART is ready to receive data
 * On complete uart_recv_isr is called
 * @note Implement in hal_uart.c
 */
inline hal_status_t uart_recv_it(uart_t uart, uint8_t* buff, uint16_t size, uint16_t timeout);

/**
 * UART send ISR
 * @note This function should be called from lower level (driver's) ISR in the hal_uart.c
 */
inline void uart_send_isr(uart_t uart, hal_status_t status);

/**
 * UART receive ISR
 * @note This function should be called from lower level (driver's) ISR in the hal_uart.c
 */
inline void uart_recv_isr(uart_t uart, hal_status_t status);

#endif /* HAL_UART_H */
