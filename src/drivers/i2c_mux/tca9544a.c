#include "drivers/i2c_mux.h"
#include "drivers/sdev.h"

#define TCA9544A_OUT_CH_COUNT   (4)
#define TCA9544A_ADDRESS        (0x00)

static status_t tca9544a_ch_select(void* context, i2c_mux_ch_t ch)
{
    return STATUS_NOT_IMPLEMENTED;
}

status_t tca9544a_i2c_mux_open(i2c_mux_t* mux, i2c_t* in_bus)
{
    static i2c_mux_ops_t i2c_mux_ops = {
        .ch_select = &tca9544a_ch_select
    };

    return i2c_mux_open(mux, in_bus, TCA9544A_OUT_CH_COUNT, &i2c_mux_ops, NULL);
}