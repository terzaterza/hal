#include "hal_target_pc.h"

#define UART_START_BYTE     0x5a
#define UART_STOP_BYTE      0xa5

typedef struct {
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
#include "hal_uart.h"

/**
 * Virtual UART peripherals
*/
static hal_target_pc_uart_t uart_structs[TARGET_PC_UART_COUNT];

/**
 * Called on receive `SOCKET_UART_ID` from the socket stream
 * 
 * This function handles the UART receving process
 * Sending is handled through hal functions
 * 
 * Socket UART message
 * <UART_STRUCT_INDEX - 1> <UART_START_BYTE - 1> <DATA - 1> [PARITY - 1] <UART_STOP_BYTE - 1 (2)>
*/
void peripheral_socket_handle_uart()
{
    /* Max message size if using > 1 stop bits and parity */
    uint8_t message[1 + 1 + 1 + 1 + 2];
    /* Read uart index to get settings */
    socket_read(message, 1);

    /* Get UART peripheral */
    if (message[0] >= TARGET_PC_UART_COUNT) {
        return;
    }
    uart_t uart = uart_structs + message[0];

    /* Compute how many bytes to receive based on settings */
    uint8_t recv_len = 2; /* START and DATA byte */
    /* Add parity bits (byte) */
    recv_len += uart->parity > 0;
    /* Add 1 byte for each stop bit */
    recv_len += uart->stop_bits;

    /* Get the rest of the message */
    socket_read(message + 1, recv_len);

    /* Check for starting byte */
    if (message[1] != UART_START_BYTE) {
        return; // error
    }

    /* Store received data */
    *(uart->rx_buf++) = message[2];
    uart->rx_count--;

    if (uart->parity > 0) {
        // check parity
    }

    /* Check stop bits */
    for (uint8_t i = 0; i < uart->stop_bits; i++) {
        if (message[i + 3 + (uart->parity > 0)] != UART_STOP_BYTE) {
            return; // error
        }
    }

    /** @todo Add status for uart_t */
    
    /** @todo Here add for interrupt calling */
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
    hal_status_t ret_status = HAL_STATUS_OK;

    /** @todo Check if uart is busy and return HAL_STATUS_BUSY if true */

    if (HAL_UART_Transmit(uart, data, size, timeout) != HAL_OK) {
        ret_status = HAL_STATUS_ERROR;
    }

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