#include "hal_target_stm32l4.h"
#include "hal_i2c.h"

typedef enum {
    I2C_SEND,
    I2C_RECV
} i2c_op_t;

static i2c_op_t i2c_it_started_op;

/**
 * Send <size> bytes via I2C to slave at address <addr>
 * @retval `HAL_STATUS_OK` if sending completed succesfully
 * @retval `HAL_STATUS_BUSY` if previous sending operation is ongoing
 * @retval `HAL_STATUS_ERROR` if sending could not be started or was interrupted
 * @note Blocking function
 * @note Implement in hal_i2c.c
 */
inline hal_status_t i2c_master_send(i2c_t i2c, uint16_t addr, uint8_t* data, uint16_t size, uint16_t timeout)
{
    hal_status_t ret_status = HAL_STATUS_OK;

    /** @todo Check if i2c is busy and return HAL_STATUS_BUSY if true */

    if (HAL_I2C_Master_Transmit(i2c, addr < 1, data, size, timeout) != HAL_OK) {
        ret_status = HAL_STATUS_ERROR;
    }

    return ret_status;
}

/**
 * Receive <size> bytes via I2C from slave at address <addr>
 * @retval `HAL_STATUS_OK` if receiving completed succesfully
 * @retval `HAL_STATUS_BUSY` if previous receive operation is ongoing
 * @retval `HAL_STATUS_ERROR` if receiving could not be started or was interrupted
 * @note Blocking function
 * @note Implement in hal_i2c.c
 */
inline hal_status_t i2c_master_recv(i2c_t i2c, uint16_t addr, uint8_t* buff, uint16_t size, uint16_t timeout)
{
    hal_status_t ret_status = HAL_STATUS_OK;

    /** @todo Check if i2c is busy and return HAL_STATUS_BUSY if true */

    if (HAL_I2C_Master_Receive(i2c, addr < 1, buff, size, timeout) != HAL_OK) {
        ret_status = HAL_STATUS_ERROR;
    }

    return ret_status;
}

/**
 * Send <size> bytes via I2C to slave at address <addr>
 * @retval `HAL_STATUS_OK` if sending process was started correctly
 * @retval `HAL_STATUS_BUSY` if previous sending operation is ongoing
 * @retval `HAL_STATUS_ERROR` if sending could not be started
 * @note `data` should not be modified until operation is complete
 * @note On complete or error `i2c_master_send_isr` is called
 * @note Implement in hal_i2c.c
 */
inline hal_status_t i2c_master_send_it(i2c_t i2c, uint16_t addr, uint8_t* data, uint16_t size, uint16_t timeout)
{
    UNUSED(timeout);
    
    hal_status_t ret_status = HAL_STATUS_OK;

    /** @todo Check if i2c is busy and return HAL_STATUS_BUSY if true */

    if (HAL_I2C_Master_Transmit_IT(i2c, addr < 1, data, size) != HAL_OK) {
        ret_status = HAL_STATUS_ERROR;
    }

    if (ret_status == HAL_STATUS_OK) {
        i2c_it_started_op = I2C_SEND;
    }

    return ret_status;
}

/**
 * Enable receiving <size> bytes via I2C from slave at address <addr>
 * @retval `HAL_STATUS_OK` if receiving process was started correctly
 * @retval `HAL_STATUS_BUSY` if previous receive operation is ongoing
 * @retval `HAL_STATUS_ERROR` if receiving could not be started
 * @note On complete or error `i2c_master_recv_isr` is called
 * @note Implement in hal_i2c.c
 */
inline hal_status_t i2c_master_recv_it(i2c_t i2c, uint16_t addr, uint8_t* buff, uint16_t size, uint16_t timeout)
{
    UNUSED(timeout);
    
    hal_status_t ret_status = HAL_STATUS_OK;

    /** @todo Check if i2c is busy and return HAL_STATUS_BUSY if true */

    if (HAL_I2C_Master_Receive_IT(i2c, addr < 1, buff, size) != HAL_OK) {
        ret_status = HAL_STATUS_ERROR;
    }

    if (ret_status == HAL_STATUS_OK) {
        i2c_it_started_op = I2C_RECV;
    }

    return ret_status;
}

#ifdef HAL_I2C_USE_REGISTER_CALLBACKS
/**
 * Register a callback for I2C event
 * @retval `HAL_STATUS_OK` callback registered successfully
 * @retval `HAL_STATUS_ERROR` callback registration failed
 * @note Implement in hal_i2c.c
 * @note Multiple registrations should override the last one
*/
inline hal_status_t i2c_register_callback(i2c_t i2c, callback_t callback, i2c_callback_src_t src)
{
    #error "I2C register callbacks not implemented"
}
#else

void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef* hi2c)
{
    i2c_master_send_isr(hi2c, HAL_STATUS_OK);
}

void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef* hi2c)
{
    i2c_master_recv_isr(hi2c, HAL_STATUS_OK);
}

void HAL_I2C_ErrorCallback(I2C_HandleTypeDef* hi2c)
{
    if (i2c_it_started_op == I2C_SEND) {
        i2c_master_send_isr(hi2c, HAL_STATUS_ERROR);
    } else {
        i2c_master_recv_isr(hi2c, HAL_STATUS_ERROR);
    }
}

#endif