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
 * Initialize i2c structure
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
typedef struct i2c_sdev_context {
    i2c_t* bus;
    uint8_t addr;
} i2c_sdev_context_t;

/**
 * Serial device write handler for a device connected to an I2C
*/
static status_t i2c_sdev_write(void* context, uint8_t* data, size_t len)
{
    i2c_sdev_context_t* params = (i2c_sdev_context_t*)context;

    return i2c_write(params->bus, params->addr, data, len);
}

/**
 * Serial device read handler for a device connected to an I2C
*/
static status_t i2c_sdev_read(void* context, uint8_t* data, size_t len)
{
    i2c_sdev_context_t* params = (i2c_sdev_context_t*)context;

    return i2c_read(params->bus, params->addr, data, len);
}

/**
 * Serial device test handler for a device connected to an I2C
*/
static status_t i2c_sdev_test(void* context)
{
    i2c_sdev_context_t* params = (i2c_sdev_context_t*)context;

    return i2c_dev_probe(params->bus, params->addr);
}

/**
 * Serial device close handler for a device connected to an I2C
*/
static status_t i2c_sdev_close(void* context)
{
    i2c_sdev_context_t* params = (i2c_sdev_context_t*)context;
    
    /** 
     * @todo Rename this function to `i2c_sdev_close_static` once added separate functions
     * `i2c_sdev_open_static` and `i2c_sdev_open`, so that `i2c_sdev_close` for dynamic allocation can free the memory
     */

    return STATUS_OK;
}

/**
 * Initialize the serial device object for a device with the given address
 * 
 * @note Requires static context allocation
*/
status_t i2c_sdev_open(i2c_t* i2c, sdev_t* device, i2c_sdev_context_t* context, uint8_t addr)
{
    static sdev_ops_t i2c_sdev_ops = {
        .test = &i2c_sdev_test,
        .write = &i2c_sdev_write,
        .read = &i2c_sdev_read,
        .close = &i2c_sdev_close,
        .ioctl = NULL
    };

    if (context == NULL)
        return STATUS_ERROR;
    context->bus = i2c;
    context->addr = addr;

    return sdev_open(device, &i2c_sdev_ops, context);
}