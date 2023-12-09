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
 * Get current status of the UART interface
 * @retval `HAL_STATUS_OK` if interface is ready for next operation
 * @retval `HAL_STATUS_BUSY` if interface is currently processing an operation
 * @note User can also implement to return `HAL_STATUS_ERROR` if needed
*/
inline hal_status_t uart_get_status(uart_t uart);

/**
 * Send <size> bytes via UART
 * @retval `HAL_STATUS_OK` if sending completed succesfully
 * @retval `HAL_STATUS_ERROR` if previous sending operations is ongoing
 * @note Blocking function
 * @note Implement in hal_uart.c
 */
inline hal_status_t uart_send(uart_t uart, uint8_t* data, uint16_t size, uint16_t timeout);

/**
 * Receive <size> bytes via UART
 * @retval `HAL_STATUS_OK` if receiving completed succesfully
 * @retval `HAL_STATUS_ERROR` if previous receive operations is ongoing
 * @note Blocking function
 * @note Implement in hal_uart.c
 */
inline hal_status_t uart_recv(uart_t uart, uint8_t* buff, uint16_t size, uint16_t timeout);

/**
 * Send <size> bytes via UART
 * @retval `HAL_STATUS_OK` if sending process was started correctly
 * @retval `HAL_STATUS_ERROR` if not
 * @note On complete `uart_send_isr` is called
 * @note Implement in hal_uart.c
 */
inline hal_status_t uart_send_it(uart_t uart, uint8_t* data, uint16_t size, uint16_t timeout);

/**
 * Enable receiving <size> bytes via UART
 * @retval `HAL_STATUS_OK` if receiving process was started correctly
 * @retval `HAL_STATUS_ERROR` if not
 * @note On complete `uart_recv_isr` is called
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
