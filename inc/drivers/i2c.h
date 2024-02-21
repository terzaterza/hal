#ifndef DRIVERS_I2C_H
#define DRIVERS_I2C_H

#include <stdint.h>
#include <stdlib.h>
#include "common/types.h"
#include "drivers/sdev.h"

/**
 * I2C driver interface
 * 
 * @note Should only be used through API functions starting with i2c_*
*/
typedef struct i2c i2c_t;
typedef struct i2c_sdev_context i2c_sdev_context_t;

/**
 * Hardware (driver) specific implementation of I2C functions
*/
typedef struct i2c_ops {
    status_t (*write)(void* context, uint8_t addr, uint8_t* data, size_t nbyte);
    status_t (*read)(void* context, uint8_t addr, uint8_t* data, size_t nbyte);
    status_t (*dev_probe)(void* context, uint8_t addr); /** @note Could also be implemented as writing 0 bytes to a device and waiting for ACK */
    /** @todo add async_write and async_read with registrable callbacks */
    /** @todo add close and test if needed */
} i2c_ops_t;

/**
 * @todo Can also implement dynamic allocation I2C open if dyn alloc enabled
 * 
 * i2c_t* i2c_open(i2c_ops_t* ops, void* context);
 * return NULL if failed, else return pointer to allocated structure
 * 
 * then also add i2c_close to free the memory
*/

/**
 * Create and initialize an I2C structure
 * 
 * @note Requires static (persistent) allocation
*/
status_t i2c_open(i2c_t* i2c, i2c_ops_t* ops, void* context);

/**
 * Write a sequence of bytes to an I2C slave
 * 
 * @note Blocking function, exits once the bus transaction is complete.
 * 
 * @return Return value indicates if the transaction was successful.
*/
status_t i2c_write(i2c_t* i2c, uint8_t addr, uint8_t* data, size_t nbyte);

/**
 * Read a sequence of bytes from an I2C slave
 * 
 * @note Blocking function, exits once the bus transaction is complete.
 * 
 * @return Return value indicates if the transaction was successful.
*/
status_t i2c_read(i2c_t* i2c, uint8_t addr, uint8_t* data, size_t nbyte);

/**
 * Call the device with the given address and expect acknowledge signal,
 * for a given number of tries. Stop after first success.
 * 
 * @return Return values indicates a successful acknowledge.
*/
status_t i2c_dev_probe(i2c_t* i2c, uint8_t addr);

/**
 * Open a serial device connected to this I2C
 * 
 * @note Require static (persistent) allocation
*/
status_t i2c_sdev_open(i2c_t* i2c, sdev_t* device, i2c_sdev_context_t* context, uint8_t addr);

#endif