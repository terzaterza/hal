#include "hal_target_stm32l4.h"
#include "hal_uart.h"

typedef enum {
    UART_SEND,
    UART_RECV
} uart_op_t;

static uart_op_t uart_it_started_op;

/**
 * Send <size> bytes via UART
 * @retval `HAL_STATUS_OK` if sending completed succesfully
 * @retval `HAL_STATUS_BUSY` if previous sending operation is ongoing
 * @retval `HAL_STATUS_ERROR` if sending could not be started or was interrupted
 * @note Blocking function
 * @note Implement in hal_uart.c
 */
inline hal_status_t uart_send(uart_t uart, uint8_t* data, uint16_t size, uint16_t timeout)
{
    hal_status_t ret_status = HAL_STATUS_OK;

    /** @todo Check if uart is busy and return HAL_STATUS_BUSY if true */

    if (HAL_UART_Transmit(uart, data, size, timeout) != HAL_OK) {
        ret_status = HAL_STATUS_ERROR;
    }
    while (HAL_UART_GetState(uart)) {}

    return ret_status;
}

/**
 * Receive <size> bytes via UART
 * @retval `HAL_STATUS_OK` if receiving completed succesfully
 * @retval `HAL_STATUS_BUSY` if previous receive operation is ongoing
 * @retval `HAL_STATUS_ERROR` if receiving could not be started or was interrupted
 * @note Blocking function
 * @note Implement in hal_uart.c
 */
inline hal_status_t uart_recv(uart_t uart, uint8_t* buff, uint16_t size, uint16_t timeout)
{
    hal_status_t ret_status = HAL_STATUS_OK;

    /** @todo Check if uart is busy and return HAL_STATUS_BUSY if true */

    if (HAL_UART_Receive(uart, buff, size, timeout) != HAL_OK) {
        ret_status = HAL_STATUS_ERROR;
    }
    while (HAL_UART_GetState(uart)) {}

    return ret_status;
}

/**
 * Send <size> bytes via UART
 * @retval `HAL_STATUS_OK` if sending process was started correctly
 * @retval `HAL_STATUS_BUSY` if previous sending operation is ongoing
 * @retval `HAL_STATUS_ERROR` if sending could not be started
 * @note `data` should not be modified until operation is complete
 * @note On complete or error `uart_send_isr` is called
 * @note Implement in hal_uart.c
 */
inline hal_status_t uart_send_it(uart_t uart, uint8_t* data, uint16_t size, uint16_t timeout)
{
    UNUSED(timeout);
    
    hal_status_t ret_status = HAL_STATUS_OK;

    /** @todo Check if uart is busy and return HAL_STATUS_BUSY if true */

    if (HAL_UART_Transmit_IT(uart, data, size) != HAL_OK) {
        ret_status = HAL_STATUS_ERROR;
    }

    if (ret_status == HAL_STATUS_OK) {
        uart_it_started_op = UART_SEND;
    }

    return ret_status;
}

/**
 * Enable receiving <size> bytes via UART
 * @retval `HAL_STATUS_OK` if receiving process was started correctly
 * @retval `HAL_STATUS_BUSY` if previous receive operation is ongoing
 * @retval `HAL_STATUS_ERROR` if receiving could not be started
 * @note On complete or error `uart_recv_isr` is called
 * @note Implement in hal_uart.c
 */
inline hal_status_t uart_recv_it(uart_t uart, uint8_t* buff, uint16_t size, uint16_t timeout)
{
    UNUSED(timeout);
    
    hal_status_t ret_status = HAL_STATUS_OK;

    /** @todo Check if uart is busy and return HAL_STATUS_BUSY if true */

    if (HAL_UART_Receive_IT(uart, buff, size) != HAL_OK) {
        ret_status = HAL_STATUS_ERROR;
    }

    if (ret_status == HAL_STATUS_OK) {
        uart_it_started_op = UART_RECV;
    }

    return ret_status;
}

/**
 * Returns whether UART receive buffer is empty
 */
inline uint8_t uart_rx_not_empty(uart_t uart)
{
    return __HAL_UART_GET_FLAG(uart, UART_FLAG_RXNE);
}

/**
 * Returns contents of UART receive data register
 */
inline uint8_t uart_get_rx_data(uart_t uart)
{
    return (uint8_t)(uart->Instance->RDR & 0xff);
}

#ifdef HAL_UART_USE_REGISTER_CALLBACKS
/**
 * Register a callback for UART event
 * @retval `HAL_STATUS_OK` callback registered successfully
 * @retval `HAL_STATUS_ERROR` callback registration failed
 * @note Implement in hal_uart.c
 * @note Multiple registrations should override the last one
*/
inline hal_status_t uart_register_callback(uart_t uart, callback_t callback, uart_callback_src_t src)
{
    #error "UART register callbacks not implemented"
}
#else

__weak void uart_send_isr(uart_t uart, hal_status_t status)
{

}

__weak void uart_recv_isr(uart_t uart, hal_status_t status)
{

}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef* huart)
{
    uart_send_isr(huart, HAL_STATUS_OK);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart)
{
    uart_recv_isr(huart, HAL_STATUS_OK);
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef* huart)
{
    if (uart_it_started_op == UART_SEND) {
        uart_send_isr(huart, HAL_STATUS_ERROR);
    } else {
        uart_recv_isr(huart, HAL_STATUS_ERROR);
    }
}
#endif