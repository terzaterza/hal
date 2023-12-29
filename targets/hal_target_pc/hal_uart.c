#include "hal_target_pc.h"
#include "hal_uart.h"
#include <string.h>

#define UART_START_BYTE     0x5a
#define UART_STOP_BYTE      0xa5

#define UART_MSG_BAUD_RATE  0
#define UART_MSG_PARITY     1
#define UART_MSG_STOP_BITS  2
#define UART_MSG_START_BYTE 3

#define UART_MSG_HEADER_LEN (UART_MSG_START_BYTE + 1)
#define UART_MSG_DATA_LEN   (4096 - UART_MSG_HEADER_LEN - 1)

#define MIN(a, b) ((a) <= (b) ? (a) : (b))

/**
 * Called on receive `SOCKET_UART_ID` from the socket stream
 * 
 * This function handles the UART receving process
 * Sending is handled in hal functions
 * 
 * Socket UART message
 * <BAUD_RATE - 1> <PARITY - 1> <STOB_BITS - 1> <UART_START_BYTE - 1> <DATA - len-5> <UART_STOP_BYTE - 1>
*/
void peripheral_socket_handle_uart(hal_target_pc_uart_t* uart, uint8_t* msg, uint16_t len)
{
    /* Only receiving messages if user requested it - no dedicated UART buffer is used by hal */
    if (uart->status != SP_RECEIVING) {
        return;
    }

    /* Every message should start with a header */
    /* Check if UART settings match - in physical UART, message would not be received if mismatch */
    /** @todo Add another byte for data length checking once added to uart_t */
    if ((msg[UART_MSG_BAUD_RATE] != uart->baud_rate) ||
        (msg[UART_MSG_PARITY] != uart->parity) ||
        (msg[UART_MSG_STOP_BITS] != uart->stop_bits)) {
        return;
    }

    /* Check for starting byte */
    if (msg[UART_MSG_START_BYTE] != UART_START_BYTE) {
        return; // error
    }

    /* Find UART_STOP_BYTE and calculate data length*/
    uint16_t data_len = 0;
    for (uint16_t i = UART_MSG_START_BYTE + 1; i < len; i++) {
        if (msg[i] == UART_STOP_BYTE) {
            data_len = i - UART_MSG_START_BYTE - 1;
            break;
        }
    }
    /* If no stop byte found or if no data bytes in message, return */
    if (data_len == 0) {
        return;
    }

    /* Copy data to user's buffer - if there is no space for all of data, excess is lost */
    size_t copy_len = MIN(data_len, uart->rx_count);    
    memcpy(uart->rx_buf, msg + UART_MSG_START_BYTE + 1, copy_len);

    uart->rx_buf += copy_len;
    uart->rx_count -= copy_len;

    if (uart->rx_count == 0) {
        uart->status = SP_FINISHED_OK;
        /* If in interrupt mode, auto-reset UART status to not */
        /* be busy when starting next receive from isr */
        if (uart->int_mode == 1) {
            uart->status = SP_READY;
            uart_recv_isr(uart, HAL_STATUS_OK);
        }
    }
    
}

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
    if (size > UART_MSG_DATA_LEN) {
        return HAL_STATUS_ERROR;
    }

    /* UART should never be busy since only using socket_write, so no need to check */
    uart->status = SP_READY;

    /* Buffer = Header + Data + Stop byte */
    uint8_t tx_buf[UART_MSG_HEADER_LEN + UART_MSG_DATA_LEN + 1];

    /* Initialize UART message header */
    tx_buf[UART_MSG_BAUD_RATE] = uart->baud_rate;
    tx_buf[UART_MSG_PARITY] = uart->parity;
    tx_buf[UART_MSG_STOP_BITS] = uart->stop_bits;
    tx_buf[UART_MSG_START_BYTE] = UART_START_BYTE;
    /** @todo Add data length in bits here once implemented */

    memcpy(tx_buf + UART_MSG_HEADER_LEN, data, size);
    tx_buf[UART_MSG_HEADER_LEN + size] = UART_STOP_BYTE;

    uart->int_mode = 0;

    /* Sending successful only if all bytes sent successfully */
    uint16_t send_size = UART_MSG_HEADER_LEN + size + 1;
    if (socket_write(SOCKET_PERIPH_UART, uart->id, tx_buf, send_size) != send_size) {
        return HAL_STATUS_ERROR;
    }

    return HAL_STATUS_OK;
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
    if (uart->status != SP_READY) {
        return HAL_STATUS_BUSY;
    }

    uart->status = SP_RECEIVING;
    uart->rx_buf = buff;
    uart->rx_count = size;
    uart->int_mode = 0;

    /* Wait until received specified number of bytes which happens in another async thread */
    while (uart->status == SP_RECEIVING) {}

    hal_status_t ret_status =
        uart->status == SP_FINISHED_OK ? HAL_STATUS_OK : HAL_STATUS_ERROR;

    /* Reset UART for next receive process */
    uart->status = SP_READY;
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
    
    if (size > UART_MSG_DATA_LEN) {
        return HAL_STATUS_ERROR;
    }

    /* UART should never be busy since only using socket_write, so no need to check */
    uart->status = SP_READY;

    /* Buffer = Header + Data + Stop byte */
    uint8_t tx_buf[UART_MSG_HEADER_LEN + UART_MSG_DATA_LEN + 1];

    /* Initialize UART message header */
    tx_buf[UART_MSG_BAUD_RATE] = uart->baud_rate;
    tx_buf[UART_MSG_PARITY] = uart->parity;
    tx_buf[UART_MSG_STOP_BITS] = uart->stop_bits;
    tx_buf[UART_MSG_START_BYTE] = UART_START_BYTE;
    /** @todo Add data length in bits here once implemented */

    memcpy(tx_buf + UART_MSG_HEADER_LEN, data, size);
    tx_buf[UART_MSG_HEADER_LEN + size] = UART_STOP_BYTE;

    /* Not used but set to 1 for consistency */
    uart->int_mode = 1;

    /* Sending successful only if all bytes sent successfully */
    uint16_t send_size = UART_MSG_HEADER_LEN + size + 1;
    if (socket_write(SOCKET_PERIPH_UART, uart->id, tx_buf, send_size) != send_size) {
        return HAL_STATUS_ERROR;
    }

    /* Call interrupt when sending complete */
    uart_send_isr(uart, HAL_STATUS_OK);
    return HAL_STATUS_OK;
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
    
    if (uart->status != SP_READY) {
        return HAL_STATUS_BUSY;
    }

    uart->status = SP_RECEIVING;
    uart->rx_buf = buff;
    uart->rx_count = size;
    uart->int_mode = 1;

    /* Reset UART for next receive process */
    uart->status = SP_READY;
    return HAL_STATUS_OK;
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
#endif