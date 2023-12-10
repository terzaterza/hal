#ifndef HAL_CALLBACK_H
#define HAL_CALLBACK_H

#include "hal_core.h"

typedef void (*callback_t)(hal_status_t);

#if !defined(CALLBACK_PERIPHERAL_TYPE) || \
    !defined(CALLBACK_SOURCE_TYPE) || \
    !defined(CALLBACK_HAL_PREFIX)
    #error "Callback types not defined"
#else
    #define CB_PER_T    CALLBACK_PERIPHERAL_TYPE
    #define CB_SRC_T    CALLBACK_SOURCE_TYPE
    #define CB_REG_NAME CALLBACK_HAL_PREFIX ## _register_callback

    hal_status_t CB_REG_NAME(CB_PER_T CB_PREFIX, callback_t callback, CB_SRC_T source);
#endif


#endif