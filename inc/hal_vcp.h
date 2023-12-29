/**
 * Virtual Com Port implementation
*/

#ifndef HAL_VCP_H
#define HAL_VCP_H

#include <stdint.h>
#include "hal_core.h"

/**
 * Send `length` bytes from `data` over VCP (serial port)
 * @retval `HAL_STATUS_OK` if data was sent correctly
 * @retval `HAL_STATUS_ERROR` if there was an error during sending process
 * @note Blocking function
*/
inline hal_status_t vcp_send(uint8_t* data, uint16_t length);

/**
 * Read `read_len` unread bytes which were received over VCP (serial port)
 * @returns Number of bytes which was actually read (available to read)
 * @note This function should read from the buffer where `vcp_recv_isr` stores received packets
*/
inline uint16_t vcp_read(uint8_t* buffer, uint16_t read_len);

/**
 * VCP receive packet callback
 * @note This function should be called from lower level (driver's) ISR in the hal_vcp.c
*/
inline void vcp_recv_isr(uint8_t* data, uint16_t length);

#endif /* HAL_VCP_H */