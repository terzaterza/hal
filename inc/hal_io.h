#ifndef HAL_IO_H
#define HAL_IO_H

#include "hal_core.h"
#include "stdint.h"

/**
 * Send `len` bytes of data starting from `buffer`
 * 
 * @note Blocking function
 * @note Never returns busy since all IO functions should be blocking
 * @retval `HAL_STATUS_OK` - message sent successfully
 * @retval `HAL_STATUS_ERROR` - error during communication
*/
hal_status_t io_putdata(const uint8_t* buffer, uint16_t len);

/**
 * Receive `len` bytes of data and store starting at `buffer`
 * 
 * @note Blocking function
 * @note Never returns busy since all IO functions should be blocking
 * @retval `HAL_STATUS_OK` - message sent successfully
 * @retval `HAL_STATUS_ERROR` - error during communication
*/
hal_status_t io_getdata(uint8_t* buffer, uint16_t len);

/**
 * Replacement for stdio printf
 * 
 * @note Should be implemented as vsnprintf to a buffer than a call to io_putdata
 * @note Blocking function
 * @retval `HAL_STATUS_OK` - message sent successfully
 * @retval `HAL_STATUS_ERROR` - message larger than transmit buffer or error during communication
*/
hal_status_t io_printf(const char* format, ...);

/**
 * Replacement for stdio scanf
*/
hal_status_t io_scanf(const char* format, ...);

/**
 * Replacement for putchar
*/
hal_status_t io_putchar(const char c);

/**
 * Replacement for getchar
*/
hal_status_t io_getchar(char* c);

#endif