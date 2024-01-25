#include <stdio.h>
#include <stdarg.h>
#include "hal_target_stm32l4.h"
#include "hal_io.h"

#define HAL_IO_PRINTF_BUFFER_LEN    (512)
#define UART_RECV_BUFF_LEN          (512)

#if defined(HAL_IO_USE_USB)
    #include "usbd_cdc_if.h"
#elif defined(HAL_IO_USE_UART)
    #include "hal_uart.h"
    HAL_GET_RESOURCE(uart_t, IO_UART);
#else
    #warning "IO port not defined"
    #warning "Please set HAL_IO_USE_USB or HAL_IO_USE_UART to use IO"
#endif

/**
 * Send `len` bytes of data starting from `buffer`
 * 
 * @note Blocking function
 * @note Never returns busy since all IO functions should be blocking
 * @retval `HAL_STATUS_OK` - message sent successfully
 * @retval `HAL_STATUS_ERROR` - error during communication
*/
inline hal_status_t io_putdata(const uint8_t* buffer, uint16_t len)
{
    hal_status_t ret_status = HAL_STATUS_OK;

    #if defined(HAL_IO_USE_USB)
        /** @todo Fix this to return status */
        CDC_Transmit_FS(buffer, len);
    #elif defined(HAL_IO_USE_UART)
        if (HAL_UART_Transmit(IO_UART, buffer, len, 1000) != HAL_OK) {
            ret_status = HAL_STATUS_ERROR;
        }
    #endif

    return ret_status;
}

/**
 * Receive `len` bytes of data and store starting at `buffer`
 * 
 * @note Blocking function
 * @note Never returns busy since all IO functions should be blocking
 * @retval `HAL_STATUS_OK` - message sent successfully
 * @retval `HAL_STATUS_ERROR` - error during communication
*/
inline hal_status_t io_getdata(uint8_t* buffer, uint16_t len)
{
    hal_status_t ret_status = HAL_STATUS_OK;

    #if defined(HAL_IO_USE_USB)

    #elif defined(HAL_IO_USE_UART)
        if (HAL_UART_Receive(IO_UART, buffer, len, 1000) != HAL_OK) {
            ret_status = HAL_STATUS_ERROR;
        }
    #endif

    return ret_status;
}

/**
 * Replacement for stdio printf
 * 
 * @note Should be implemented as vsnprintf to a buffer than a call to io_putdata
 * @note Blocking function
 * @retval `HAL_STATUS_OK` - message sent successfully
 * @retval `HAL_STATUS_ERROR` - message larger than transmit buffer or error during communication
*/
inline hal_status_t io_printf(const char* format, ...)
{
    static char buffer[HAL_IO_PRINTF_BUFFER_LEN] = {0};

    va_list args;
    va_start(args, format);
    int len = vsnprintf(buffer, HAL_IO_PRINTF_BUFFER_LEN, format, args);
    va_end(args);
    
    if (len >= HAL_IO_PRINTF_BUFFER_LEN) {
        len = HAL_IO_PRINTF_BUFFER_LEN - 1;
    }
    return io_putdata(buffer, len);
}

/**
 * Replacement for stdio scanf
*/
inline hal_status_t io_scanf(const char* format, ...)
{
    /* Not implemented */
    return HAL_STATUS_ERROR;
}

/**
 * Replacement for putchar
*/
inline hal_status_t io_putchar(const char c)
{
    return io_putdata(&c, 1);
}

/**
 * Replacement for getchar
*/
inline hal_status_t io_getchar(char* c)
{
    return io_getdata(c, 1);
}


/**
 * Callback for IO receive event
 */
static void (*callback_assigned)(uint8_t*, uint16_t) = NULL;

#if HAL_IO_USE_UART
static uint8_t uart_recv_buffer[UART_RECV_BUFF_LEN];
/* If STM32 register callbacks are enabled, use a custom function for ISR */
#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)

#else
void HAL_UARTEx_RxEventCallback(uart_t uart, uint16_t len)
{
    if (callback_assigned != NULL)
        callback_assigned(uart_recv_buffer, len);
    HAL_UARTEx_ReceiveToIdle_IT(IO_UART, uart_recv_buffer, UART_RECV_BUFF_LEN);
}
#endif
#endif

/**
 * Register a callback for when data is received over IO port
 * @note Callback function is called from ISR context
 */
inline hal_status_t io_register_receive_callback(void (*callback)(uint8_t* data, uint16_t len))
{
#ifdef HAL_IO_USE_UART
    /* If STM32 register callbacks are enabled, use a custom function for ISR */
    #if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
    /** @todo Register a callback to the function above */
    #endif
    if (callback_assigned == NULL) {
        if (HAL_UARTEx_ReceiveToIdle_IT(IO_UART, uart_recv_buffer, UART_RECV_BUFF_LEN) != HAL_OK)
            return HAL_STATUS_ERROR;
    }
#endif
    callback_assigned = callback;
    return HAL_STATUS_OK;
}