#ifndef HAL_ADC_H
#define HAL_ADC_H

#include <stdint.h>
#include "hal_core.h"

#ifndef HAL_ADC_TYPEDEF
    #error "HAL_ADC_TYPEDEF not defined"
    __HAL_TEMPLATE_TYPEDEF(adc_t);
#else
    typedef HAL_ADC_TYPEDEF adc_t;
#endif

/**
 * Start the ADC in DMA mode
*/
hal_status_t adc_start_dma(adc_t adc, uint8_t* buffer, uint32_t length);

/**
 * Stop the ADC in DMA mode
 * @note This assumes ADC was started in DMA mode
*/
hal_status_t adc_stop_dma(adc_t adc);

/**
 * Set channels to be sampled in the given order
 * @note ADC should be stopped before this is called
*/
hal_status_t adc_set_channels(adc_t adc, uint8_t* channels, uint8_t n_channels);

/**
 * Read the end of sequence conversion status flag
*/
uint8_t adc_read_eos_flag(adc_t adc);

#ifdef HAL_ADC_USE_REGISTER_CALLBACKS
/**
 * Register a callback for ADC event
 * @retval `HAL_STATUS_OK` callback registered successfully
 * @retval `HAL_STATUS_ERROR` callback registration failed
 * @note Implement in hal_adc.c
 * @note Multiple registrations should override the last one
*/
hal_status_t adc_register_callback(adc_t adc, callback_t callback);
#else
/**
 * ADC end of sequence ISR
 * @note This ISR should be entered every time all channels which are
 * selected are sampled
 * @note This function should be called from lower level (driver's) ISR in the hal_adc.c
*/
void adc_eos_isr(adc_t adc);
#endif

#endif