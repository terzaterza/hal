#ifndef HAL_VCP_H
#define HAL_VCP_H

#include <stdint.h>
#include "hal_conf.h"
#include "hal_core.h"

#ifndef HAL_VCP_TYPEDEF
    #error "HAL_VCP_TYPEDEF not defined"
#else
    typedef HAL_VCP_TYPEDEF vcp_t;
#endif

/**
 * Get current status of the VCP interface
 * @retval `HAL_STATUS_OK` if interface is ready for next operation
 * @retval `HAL_STATUS_BUSY` if interface is currently processing an operation
 * @note User can also implement to return `HAL_STATUS_ERROR` if needed
*/
inline hal_status_t vcp_get_status(vcp_t vcp);

/**
 * Send <size> bytes via VCP
 * @retval `HAL_STATUS_OK` if sending completed succesfully
 * @retval `HAL_STATUS_ERROR` if previous sending operations is ongoing
 * @note Blocking function
 * @note Implement in hal_vcp.c
 */
inline hal_status_t vcp_send(vcp_t vcp, uint8_t* data, uint16_t size);

/**
 * Receive <size> bytes via VCP
 * @retval `HAL_STATUS_OK` if receiving completed succesfully
 * @retval `HAL_STATUS_ERROR` if previous receive operations is ongoing
 * @note Blocking function
 * @note Implement in hal_vcp.c
 */
inline hal_status_t vcp_recv(vcp_t vcp, uint8_t* buff, uint16_t size);

/**
 * Send <size> bytes via VCP
 * @retval `HAL_STATUS_OK` if sending process was started correctly
 * @retval `HAL_STATUS_ERROR` if not
 * @note `data` should not be modified until operation is complete
 * @note On complete `vcp_send_isr` is called
 * @note Implement in hal_vcp.c
 */
inline hal_status_t vcp_send_it(vcp_t vcp, uint8_t* data, uint16_t size);

/**
 * Enable receiving <size> bytes via VCP
 * @retval `HAL_STATUS_OK` if receiving process was started correctly
 * @retval `HAL_STATUS_ERROR` if not
 * @note On complete `vcp_recv_isr` is called
 * @note Implement in hal_vcp.c
 */
inline hal_status_t vcp_recv_it(vcp_t vcp, uint8_t* buff, uint16_t size);

/**
 * VCP send ISR
 * @note This function should be called from lower level (driver's) ISR in the hal_vcp.c
 */
inline void vcp_send_isr(vcp_t vcp, hal_status_t status);

/**
 * VCP receive ISR
 * @note This function should be called from lower level (driver's) ISR in the hal_vcp.c
 */
inline void vcp_recv_isr(vcp_t vcp, hal_status_t status);


#endif