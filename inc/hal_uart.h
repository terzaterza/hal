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

typedef enum {
    UART_CB_SRC_SEND_COMPLETE,
    UART_CB_SRC_RECV_COMPLETE,
    UART_CB_SRC_SEND_ERROR,
    UART_CB_SRC_RECV_ERROR
} uart_callback_src_t;

/**
 * Send <size> bytes via UART
 * @retval `HAL_STATUS_OK` if sending completed succesfully
 * @retval `HAL_STATUS_BUSY` if previous sending operation is ongoing
 * @retval `HAL_STATUS_ERROR` if sending could not be started or was interrupted
 * @note Blocking function
 * @note Implement in hal_uart.c
 */
inline hal_status_t uart_send(uart_t uart, uint8_t* data, uint16_t size, uint16_t timeout);

/**
 * Receive <size> bytes via UART
 * @retval `HAL_STATUS_OK` if receiving completed succesfully
 * @retval `HAL_STATUS_BUSY` if previous receive operation is ongoing
 * @retval `HAL_STATUS_ERROR` if receiving could not be started or was interrupted
 * @note Blocking function
 * @note Implement in hal_uart.c
 */
inline hal_status_t uart_recv(uart_t uart, uint8_t* buff, uint16_t size, uint16_t timeout);

/**
 * Send <size> bytes via UART
 * @retval `HAL_STATUS_OK` if sending process was started correctly
 * @retval `HAL_STATUS_BUSY` if previous sending operation is ongoing
 * @retval `HAL_STATUS_ERROR` if sending could not be started
 * @note `data` should not be modified until operation is complete
 * @note On complete or error `uart_send_isr` is called
 * @note Implement in hal_uart.c
 */
inline hal_status_t uart_send_it(uart_t uart, uint8_t* data, uint16_t size, uint16_t timeout);

/**
 * Enable receiving <size> bytes via UART
 * @retval `HAL_STATUS_OK` if receiving process was started correctly
 * @retval `HAL_STATUS_BUSY` if previous receive operation is ongoing
 * @retval `HAL_STATUS_ERROR` if receiving could not be started
 * @note On complete or error `uart_recv_isr` is called
 * @note Implement in hal_uart.c
 */
inline hal_status_t uart_recv_it(uart_t uart, uint8_t* buff, uint16_t size, uint16_t timeout);

#ifdef HAL_UART_USE_REGISTER_CALLBACKS
/**
 * Register a callback for UART event
 * @retval `HAL_STATUS_OK` callback registered successfully
 * @retval `HAL_STATUS_ERROR` callback registration failed
 * @note Implement in hal_uart.c
 * @note Multiple registrations should override the last one
*/
inline hal_status_t uart_register_callback(uart_t uart, callback_t callback, uart_callback_src_t src);
#else
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
#endif

#endif /* HAL_UART_H */
