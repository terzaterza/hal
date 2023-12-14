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

typedef enum {
    I2C_CB_SRC_MASTER_SEND_COMPLETE,
    I2C_CB_SRC_MASTER_RECV_COMPLETE,
    I2C_CB_SRC_MASTER_SEND_ERROR,
    I2C_CB_SRC_MASTER_RECV_ERROR,
    I2C_CB_SRC_SLAVE_SEND_COMPLETE,
    I2C_CB_SRC_SLAVE_RECV_COMPLETE,
    I2C_CB_SRC_SLAVE_SEND_ERROR,
    I2C_CB_SRC_SLAVE_RECV_ERROR
} i2c_callback_src_t;

/**
 * Send <size> bytes via I2C to slave at address <addr>
 * @retval `HAL_STATUS_OK` if sending completed succesfully
 * @retval `HAL_STATUS_BUSY` if previous sending operation is ongoing
 * @retval `HAL_STATUS_ERROR` if sending could not be started or was interrupted
 * @note Blocking function
 * @note Implement in hal_i2c.c
 */
inline hal_status_t i2c_master_send(i2c_t i2c, uint16_t addr, uint8_t* data, uint16_t size, uint16_t timeout);

/**
 * Receive <size> bytes via I2C from slave at address <addr>
 * @retval `HAL_STATUS_OK` if receiving completed succesfully
 * @retval `HAL_STATUS_BUSY` if previous receive operation is ongoing
 * @retval `HAL_STATUS_ERROR` if receiving could not be started or was interrupted
 * @note Blocking function
 * @note Implement in hal_i2c.c
 */
inline hal_status_t i2c_master_recv(i2c_t i2c, uint16_t addr, uint8_t* buff, uint16_t size, uint16_t timeout);

/**
 * Send <size> bytes via I2C to slave at address <addr>
 * @retval `HAL_STATUS_OK` if sending process was started correctly
 * @retval `HAL_STATUS_BUSY` if previous sending operation is ongoing
 * @retval `HAL_STATUS_ERROR` if sending could not be started
 * @note `data` should not be modified until operation is complete
 * @note On complete or error `i2c_master_send_isr` is called
 * @note Implement in hal_i2c.c
 */
inline hal_status_t i2c_master_send_it(i2c_t i2c, uint16_t addr, uint8_t* data, uint16_t size, uint16_t timeout);

/**
 * Enable receiving <size> bytes via I2C from slave at address <addr>
 * @retval `HAL_STATUS_OK` if receiving process was started correctly
 * @retval `HAL_STATUS_BUSY` if previous receive operation is ongoing
 * @retval `HAL_STATUS_ERROR` if receiving could not be started
 * @note On complete or error `i2c_master_recv_isr` is called
 * @note Implement in hal_i2c.c
 */
inline hal_status_t i2c_master_recv_it(i2c_t i2c, uint16_t addr, uint8_t* buff, uint16_t size, uint16_t timeout);

#ifdef HAL_I2C_USE_REGISTER_CALLBACKS
/**
 * Register a callback for I2C event
 * @retval `HAL_STATUS_OK` callback registered successfully
 * @retval `HAL_STATUS_ERROR` callback registration failed
 * @note Implement in hal_i2c.c
 * @note Multiple registrations should override the last one
*/
inline hal_status_t i2c_register_callback(i2c_t i2c, callback_t callback, i2c_callback_src_t src);
#else
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
#endif

#endif /* HAL_I2C_H */
