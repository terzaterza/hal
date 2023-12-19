#include "hal_target_pc.h"

#define I2C_WRITE_BIT       (0)
#define I2C_READ_BIT        (1)
#define I2C_START_BYTE      0x5a
#define I2C_STOP_BYTE       0xa5
#define I2C_ACK_BYTE        0xaa
// #define SOCKET_I2C_NACK     0x55

typedef enum {
    I2C_READY,
    I2C_SENDING,
    I2C_RECEIVING,
    I2C_FINISHED_OK,
    I2C_FINISHED_ERROR
} i2c_status_t;

/** @note volatile variables might be needed */
typedef struct {
    /* Set before START byte is sent (or received if working as slave) */
    i2c_status_t status;
    /* Pointer to buffer from which data is transmited while i2c_tx_len > 0 (autoincremented) */
    uint8_t* tx_buf;
    /* Pointer to where the current received byter should be stored (autoincremented) */
    uint8_t* rx_buf;
    /* How many bytes are still to be sent or received */
    uint16_t byte_count;
    /* Wait for slave to acknowledge addressing when starting reading process */
    uint8_t wait_ack;
    /* Should ISR be called on complete or if error */
    uint8_t int_mode;
    /** @todo Add registrable callbacks here */
} hal_target_pc_i2c_t;
/** @todo Could use one buffer pointer for both tx and rx */

#define HAL_I2C_TYPEDEF     hal_target_pc_i2c_t*
#include "hal_i2c.h"

/**
 * Virtual I2C peripherals
 * 
 * @note Could rename
*/
static hal_target_pc_i2c_t i2c_structs[TARGET_PC_I2C_COUNT];

/**
 * Called on receive `SOCKET_I2C_ID` from the socket stream
 * 
 * I2C process is always started from the hal function by sending start and addressing bytes
 * This is then called on slave acknowledge and every sequential byte
 * 
 * Socket I2C message - <VALUE - size in bytes>
 * <I2C_STRUCT_INDEX - 1> <DATA BYTE - 1>
 * 
 * @note Always receiving one byte at a time
 * @todo Check for errors every time when using socket_write/read
*/
void peripheral_socket_handle_i2c()
{
    uint8_t message[2];
    socket_read(message, 2);

    /* Refer to I2C selected by the message */
    if (message[0] >= TARGET_PC_I2C_COUNT) {
        /** @todo Could return error here */
        return;
    }
    i2c_t i2c = i2c_structs + message[0];

    /* Receive a byte which was requested (since currently implementing only for master on I2C) */
    /** @todo When implementing slave I2C add a way to notify other functions on addressing */
    uint8_t recv = message[1];

    if (i2c->status == I2C_SENDING) {
        /* If we are sending then the received byte can (should) only be the acknowledge */
        /* Else there is an error, so end transfer */
        if (recv == I2C_ACK_BYTE) {
            if (i2c->byte_count > 0) {
                socket_write(SOCKET_I2C_ID, i2c->tx_buf, 1);
                i2c->tx_buf++;
                i2c->byte_count--;
            } else {
                socket_write_byte(SOCKET_I2C_ID, I2C_STOP_BYTE);
                i2c->status = I2C_FINISHED_OK;
            }
        } else {
            /* Received byte was not an acknowledge so end transfer */
            socket_write_byte(SOCKET_I2C_ID, I2C_STOP_BYTE);
            i2c->status = I2C_FINISHED_ERROR;
        }

        /* If in interrupt mode, auto-reset I2C to be ready for next send */
        if (i2c->status != I2C_SENDING && i2c->int_mode == 1) {
            i2c->status = I2C_READY;
            i2c_master_send_isr(i2c, i2c->status == I2C_FINISHED_OK ? HAL_STATUS_OK : HAL_STATUS_ERROR);
        }
    } else if (i2c->status == I2C_RECEIVING) {
        if (i2c->wait_ack > 0) {
            if (recv == I2C_ACK_BYTE) {
                /* Now waiting for the first byte of data from the slave so just wait for next packet */
                return;
            } else {
                /* Received byte was not an acknowledge so end transfer */
                socket_write_byte(SOCKET_I2C_ID, I2C_STOP_BYTE);
                i2c->status = I2C_FINISHED_ERROR;
            }
        } else {
            /* Store received byte to rx buffer */
            *(i2c->rx_buf) = recv;

            if (i2c->byte_count > 0) {
                /* There is more bytes to receive */
                i2c->byte_count--;
                i2c->rx_buf++;
                socket_write_byte(SOCKET_I2C_ID, I2C_ACK_BYTE);
            } else {
                /* Reading process completed successfully */
                socket_write_byte(SOCKET_I2C_ID, I2C_STOP_BYTE);
                i2c->status = I2C_FINISHED_OK;
            }
        }
        
        /* If in interrupt mode, auto-reset I2C to be ready for next send */
        if (i2c->status != I2C_RECEIVING && i2c->int_mode == 1) {
            i2c->status = I2C_READY;
            i2c_master_recv_isr(i2c, i2c->status == I2C_FINISHED_OK ? HAL_STATUS_OK : HAL_STATUS_ERROR);
        }
    }
}

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
    if (i2c->status != I2C_READY) {
        return HAL_STATUS_BUSY;
    }

    i2c->status = I2C_SENDING;
    i2c->tx_buf = data;
    i2c->byte_count = size;
    i2c->int_mode = 0;

    uint8_t addressing_byte = (addr < 1) | I2C_WRITE_BIT;
    uint8_t send_data[2] = {I2C_START_BYTE, addressing_byte};

    /* Send the first two bytes and if the amount actually sent is not 2 then return error */
    if (socket_write(SOCKET_I2C_ID, send_data, 2) != 2) {
        return HAL_STATUS_ERROR;
    }

    /* Process should be going on in the background so wait until finished since this function is blocking */
    /** @todo Implement timeout here */
    while (i2c->status == I2C_SENDING) {}

    hal_status_t ret_status =
        i2c->status == I2C_FINISHED_OK ? HAL_STATUS_OK : HAL_STATUS_ERROR;
    
    /* Reset I2C for next transfer */
    i2c->status = I2C_READY;    
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
    if (i2c->status != I2C_READY) {
        return HAL_STATUS_BUSY;
    }

    i2c->status = I2C_RECEIVING;
    i2c->rx_buf = buff;
    i2c->byte_count = size;
    i2c->int_mode = 0;

    uint8_t addressing_byte = (addr < 1) | I2C_READ_BIT;
    uint8_t send_data[2] = {I2C_START_BYTE, addressing_byte};

    /* Send the first two bytes and if the amount actually sent is not 2 then return error */
    if (socket_write(SOCKET_I2C_ID, send_data, 2) != 2) {
        return HAL_STATUS_ERROR;
    }

    /* Process should be going on in the background so wait until finished since this function is blocking */
    /** @todo Implement timeout here */
    while (i2c->status == I2C_RECEIVING) {}

    hal_status_t ret_status =
        i2c->status == I2C_FINISHED_OK ? HAL_STATUS_OK : HAL_STATUS_ERROR;
    
    /* Reset I2C for next transfer */
    i2c->status = I2C_READY;    
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
    
    if (i2c->status != I2C_READY) {
        return HAL_STATUS_BUSY;
    }

    i2c->status = I2C_SENDING;
    i2c->tx_buf = data;
    i2c->byte_count = size;
    i2c->int_mode = 1;

    uint8_t addressing_byte = (addr < 1) | I2C_WRITE_BIT;
    uint8_t send_data[2] = {I2C_START_BYTE, addressing_byte};

    /* Send the first two bytes and if the amount actually sent is not 2 then return error */
    if (socket_write(SOCKET_I2C_ID, send_data, 2) != 2) {
        return HAL_STATUS_ERROR;
    }

    return HAL_STATUS_OK;
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
    
    if (i2c->status != I2C_READY) {
        return HAL_STATUS_BUSY;
    }

    i2c->status = I2C_RECEIVING;
    i2c->rx_buf = buff;
    i2c->byte_count = size;
    i2c->int_mode = 1;

    uint8_t addressing_byte = (addr < 1) | I2C_READ_BIT;
    uint8_t send_data[2] = {I2C_START_BYTE, addressing_byte};

    /* Send the first two bytes and if the amount actually sent is not 2 then return error */
    if (socket_write(SOCKET_I2C_ID, send_data, 2) != 2) {
        return HAL_STATUS_ERROR;
    }

    return HAL_STATUS_OK;
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

#endif