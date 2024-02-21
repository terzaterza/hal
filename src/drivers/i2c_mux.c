#include "drivers/i2c_mux.h"
#include "drivers/i2c.h"

/**
 * I2C multiplexer driver structure
*/
typedef struct i2c_mux {
    i2c_mux_ch_t selected;
    i2c_t* in_bus;
    uint8_t n_out_bus;
    i2c_mux_ops_t* ops;
    void* context;
} i2c_mux_t;

/**
 * Initialize i2c_mux structure
*/
status_t i2c_mux_open(i2c_mux_t* mux, i2c_t* in, uint8_t nout, i2c_mux_ops_t* ops, void* context)
{
    if (ops == NULL)
        return STATUS_ERROR;

    mux->selected = I2C_MUX_CH_NONE;
    mux->in_bus = in;
    mux->n_out_bus = nout;

    mux->ops = ops;
    mux->context = context;

    return STATUS_OK;
}

/**
 * Select output channel (if different then currently selected)
*/
status_t i2c_mux_ch_select(i2c_mux_t* mux, i2c_mux_ch_t ch)
{
    if (ch >= mux->n_out_bus)
        return STATUS_ERROR;

    if (mux->selected == ch)
        return STATUS_OK;

    if (mux->ops->ch_select(mux->context, ch) == STATUS_OK) {
        mux->selected = ch;
        return STATUS_OK;
    }

    return STATUS_ERROR;
}