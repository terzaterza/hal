#include "drivers/i2c.h"
#include "drivers/sdev.h"

/**
 * I2C driver structure
*/
typedef struct i2c {
    i2c_ops_t* ops;
    void* context;
} i2c_t;

/**
 * Open handler
*/
status_t i2c_open(i2c_t* i2c, i2c_ops_t* ops, void* context)
{
    /** @todo Here init other I2C struct members when added */

    /* Can check here which ops are implemented and set flags accordingly if needed */
    i2c->ops = ops;
    i2c->context = context;

    return STATUS_OK;
}

/**
 * Call the implementation specific write handler
 * @todo Maybe can be implemented as `#define` or `static inline`
*/
status_t i2c_write(i2c_t* i2c, uint8_t addr, uint8_t* data, size_t nbyte)
{
    return i2c->ops->write(i2c->context, addr, data, nbyte);
}

/**
 * Call the implementation specific read handler
 * @todo Maybe can be implemented as `#define` or `static inline`
*/
status_t i2c_read(i2c_t* i2c, uint8_t addr, uint8_t* data, size_t nbyte)
{
    return i2c->ops->read(i2c->context, addr, data, nbyte);
}

/**
 * Call the implementation specific dev_probe handler
 * @todo Maybe can be implemented as `#define` or `static inline`
*/
status_t i2c_dev_probe(i2c_t* i2c, uint8_t addr)
{
    return i2c->ops->dev_probe(i2c->context, addr);
}


/**
 * Additional data for serial devices connected to an I2C
*/
typedef struct i2c_sdev_params {
    i2c_t* bus;
    uint8_t addr;
} i2c_sdev_params_t;

/**
 * Serial device write handler for a device connected to an I2C
*/
status_t i2c_sdev_write(sdev_t* device, uint8_t* data, size_t len)
{
    i2c_sdev_params_t* params = (i2c_sdev_params_t*)(device->params);

    return i2c_write(params->bus, params->addr, data, len);
}

/**
 * Serial device read handler for a device connected to an I2C
*/
status_t i2c_sdev_read(sdev_t* device, uint8_t* data, size_t len)
{
    i2c_sdev_params_t* params = (i2c_sdev_params_t*)(device->params);

    return i2c_read(params->bus, params->addr, data, len);
}

/**
 * Serial device test handler for a device connected to an I2C
*/
status_t i2c_sdev_test(sdev_t* device)
{
    i2c_sdev_params_t* params = (i2c_sdev_params_t*)(device->params);

    return i2c_dev_probe(params->bus, params->addr);
}

/**
 * Serial device close handler for a device connected to an I2C
*/
status_t i2c_sdev_close(sdev_t* device)
{
    i2c_sdev_params_t* params = (i2c_sdev_params_t*)(device->params);
    
    /** @todo Check if params was allocated using malloc and free if needed*/

    return STATUS_OK;
}

/**
 * Initialize the serial device object for a device with the given address
*/
status_t i2c_sdev_open(i2c_t* i2c, sdev_t* device, uint8_t addr)
{
    if (device->params == NULL) {
        #ifdef HAL_MALLOC_ENABLED
            device->params = malloc(sizeof(i2c_sdev_params_t));
            if (device->params == NULL)
                return STATUS_ERROR;
        #else
            return STATUS_ERROR;
        #endif
    }

    i2c_sdev_params_t* params = (i2c_sdev_params_t*)(device->params);
    params->bus = i2c;
    params->addr = addr;

    device->write = &i2c_sdev_write;
    device->read = &i2c_sdev_read;
    device->test = &i2c_sdev_test;
    device->close = &i2c_sdev_close;

    /** @todo To be implemented */
    device->init = NULL;
    device->ioctl = NULL;

    return STATUS_OK;
}