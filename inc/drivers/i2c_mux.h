#ifndef DRIVERS_I2C_MUX_H
#define DRIVERS_I2C_MUX_H

#include <stdint.h>
#include <stdlib.h>
#include "common/types.h"
#include "drivers/i2c.h"


typedef enum i2c_mux_ch {
    I2C_MUX_CH_0 = 0,
    I2C_MUX_CH_1,
    I2C_MUX_CH_2,
    I2C_MUX_CH_3,
    I2C_MUX_CH_4,
    I2C_MUX_CH_5,
    I2C_MUX_CH_6,
    I2C_MUX_CH_7,
    I2C_MUX_CH_NONE
} i2c_mux_ch_t;

/**
 * I2C multiplexer driver interface
*/
typedef struct i2c_mux i2c_mux_t;

typedef struct i2c_mux_ops {
    status_t (*ch_select)(void* context, i2c_mux_ch_t ch);
} i2c_mux_ops_t;

/**
 * Create and initialize I2C multiplexer structure
 * 
 * @note Requires static allocation
*/
status_t i2c_mux_open(i2c_mux_t* mux, i2c_t* in, uint8_t nout, i2c_mux_ops_t* ops, void* context);

/**
 * Select output channel for the mux
*/
status_t i2c_mux_ch_select(i2c_mux_t* mux, i2c_mux_ch_t ch);

/**
 * Get input bus reference
*/
i2c_t* i2c_mux_get_in_bus(i2c_mux_t* mux);

/**
 * Open an I2C corresponding to the given output channel
*/
status_t i2c_mux_ch_as_bus(i2c_mux_t* mux, i2c_t* bus, i2c_mux_ch_t ch);

#endif