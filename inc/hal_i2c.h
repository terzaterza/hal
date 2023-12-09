#ifndef HAL_I2C_H
#define HAL_I2C_H

#include <stdint.h>
#include "hal_conf.h"
#include "hal_core.h"

#ifndef HAL_I2C_TYPEDEF
    #error "HAL_I2C_TYPEDEF not defined"
#else
    typedef HAL_I2C_TYPEDEF i2c_t;
#endif

/**
 * Get current status of the I2C interface
 * @retval `HAL_STATUS_OK` if interface is ready for next operation
 * @retval `HAL_STATUS_BUSY` if interface is currently processing an operation
 * @note User can also implement to return `HAL_STATUS_ERROR` if needed
*/
inline hal_status_t i2c_get_status(i2c_t i2c);

/**
 * Send <size> bytes via I2C to slave at address <addr>
 * @retval `HAL_STATUS_OK` if sending completed succesfully
 * @retval `HAL_STATUS_ERROR` if previous sending operations is ongoing
 * @note Blocking function
 * @note Implement in hal_i2c.c
 */
inline hal_status_t i2c_master_send(i2c_t i2c, uint16_t addr, uint8_t* data, uint16_t size, uint16_t timeout);

/**
 * Receive <size> bytes via I2C from slave at address <addr>
 * @retval `HAL_STATUS_OK` if receiving completed succesfully
 * @retval `HAL_STATUS_ERROR` if previous receive operations is ongoing
 * @note Blocking function
 * @note Implement in hal_i2c.c
 */
inline hal_status_t i2c_master_recv(i2c_t i2c, uint16_t addr, uint8_t* buff, uint16_t size, uint16_t timeout);

/**
 * Send <size> bytes via I2C to slave at address <addr>
 * @retval `HAL_STATUS_OK` if sending process was started correctly
 * @retval `HAL_STATUS_ERROR` if not
 * @note On complete `i2c_master_send_isr` is called
 * @note Implement in hal_i2c.c
 */
inline hal_status_t i2c_master_send_it(i2c_t i2c, uint16_t addr, uint8_t* data, uint16_t size, uint16_t timeout);

/**
 * Enable receiving <size> bytes via I2C from slave at address <addr>
 * @retval `HAL_STATUS_OK` if receiving process was started correctly
 * @retval `HAL_STATUS_ERROR` if not
 * @note On complete `i2c_master_recv_isr` is called
 * @note Implement in hal_i2c.c
 */
inline hal_status_t i2c_master_recv_it(i2c_t i2c, uint16_t addr, uint8_t* buff, uint16_t size, uint16_t timeout);

/**
 * Master send ISR
 * @note This function should be called from lower level (driver's) ISR in the hal_i2c.c
 */
inline void i2c_master_send_isr(i2c_t i2c, hal_status_t status);

/**
 * Master receive ISR
 * @note This function should be called from lower level (driver's) ISR in the hal_i2c.c
 */
inline void i2c_master_recv_isr(i2c_t i2c, hal_status_t status);

#endif /* HAL_I2C_H */
