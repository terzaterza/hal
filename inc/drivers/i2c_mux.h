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
    I2C_MUX_CH_3
} i2c_mux_ch_t;

/**
 * I2C multiplexer API definition
*/
typedef struct i2c_mux {
    status_t (*ch_select)(struct i2c_mux* mux, i2c_mux_ch_t ch);
    i2c_mux_ch_t ch_current;
    uint8_t ch_count;
    i2c_t* input_bus;
    void* params;
} i2c_mux_t;

/**
 * Select output channel for the mux
 * @note API function
 * @note Implemented as static inline instead of #define with va args to implement current channel checking
*/
static inline status_t i2c_mux_ch_select(i2c_mux_t* mux, i2c_mux_ch_t ch)
{
    if (ch >= mux->ch_count)
        return STATUS_ERROR;

    if (mux->ch_current == ch)
        return STATUS_OK;

    if (mux->ch_select(mux, ch) == STATUS_OK) {
        mux->ch_current = ch;
        return STATUS_OK;
    }
    return STATUS_ERROR;
}

#endif