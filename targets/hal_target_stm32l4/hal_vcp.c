#include "hal_target_stm32l4.h"
#include "hal_vcp.h"

#ifndef HAL_VCP_USE_USB
    HAL_GET_RESOURCE(uart_t, VCP_UART);
#endif