#ifndef DRIVERS_I2C_H
#define DRIVERS_I2C_H

#include <stdint.h>
#include <stdlib.h>
#include "common/types.h"

#define i2c_read(i2c, ...)      ((i2c)->read((i2c), __VA_ARGS__))
#define i2c_write(i2c, ...)     ((i2c)->write((i2c), __VA_ARGS__))
#define i2c_dev_probe(i2c, ...) ((i2c)->dev_probe((i2c), __VA_ARGS__))

typedef struct i2c {
    status_t (*write)(struct i2c* i2c, uint8_t addr, uint8_t* data, size_t nbyte);
    status_t (*read)(struct i2c* i2c, uint8_t addr, uint8_t* data, size_t nbyte);
    status_t (*dev_probe)(struct i2c* i2c, uint8_t addr);
    void* params;
    /** @todo add async_write and async_read with registrable callbacks */
} i2c_t;

#endif