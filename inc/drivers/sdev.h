#ifndef DRIVERS_SDEV_H
#define DRIVERS_SDEV_H

#include <stdint.h>
#include <stdlib.h>
#include "common/types.h"

/**
 * Serial device interface
*/
typedef struct sdev sdev_t;

/**
 * Serial device operation handlers
*/
typedef struct sdev_ops {
    status_t (*test)(void* context);
    status_t (*write)(void* context, uint8_t* data, size_t nbyte);
    status_t (*read)(void* context, uint8_t* data, size_t nbyte);
    status_t (*ioctl)(void* context, int ctl_type, void* arg);
    status_t (*close)(void* context);
    /** @todo add async */
} sdev_ops_t;

/**
 * Create and initialize a serial device structure
 * 
 * @note Requires static (persistent) allocation
 * 
 * @note Can be used to reset device configuration even if the sdev was opened dynamically
*/
status_t sdev_open(sdev_t* sdev, sdev_ops_t* ops, void* context);

/**
 * Test if the device is responding
 * 
 * @note Blocking function, exits once the bus transaction is complete.
 * 
 * @return Return value indicates if the transaction was successful.
*/
status_t sdev_test(sdev_t* sdev);

/**
 * Write a sequence of bytes to a serial device
 * 
 * @note Blocking function, exits once the bus transaction is complete.
 * 
 * @return Return value indicates if the transaction was successful.
*/
status_t sdev_write(sdev_t* sdev, uint8_t* data, size_t nbyte);

/**
 * Read a sequence of bytes from a serial device
 * 
 * @note Blocking function, exits once the bus transaction is complete.
 * 
 * @return Return value indicates if the transaction was successful.
*/
status_t sdev_read(sdev_t* sdev, uint8_t* data, size_t nbyte);

/**
 * Serial device IO control
*/
status_t sdev_ioctl(sdev_t* sdev, int ctl_type, void* arg);

/**
 * Close the device and free all allocated resources
*/
status_t sdev_close(sdev_t* sdev);

#endif