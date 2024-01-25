#include "hal_target_stm32l4.h"
#include "hal_adc.h"

/**
 * Start the ADC in DMA mode
*/
inline hal_status_t adc_start_dma(adc_t adc, uint8_t* buffer, uint32_t length)
{
    hal_status_t ret_status = HAL_STATUS_OK;

    if (HAL_ADC_Start_DMA(adc, (uint32_t*)buffer, length) != HAL_OK) {
        ret_status = HAL_STATUS_ERROR;
    }

    return ret_status;
}

/**
 * Stop the ADC in DMA mode
 * @note This assumes ADC was started in DMA mode
*/
inline hal_status_t adc_stop_dma(adc_t adc)
{
    hal_status_t ret_status = HAL_STATUS_OK;

    if (HAL_ADC_Stop_DMA(adc) != HAL_OK) {
        ret_status = HAL_STATUS_ERROR;
    }

    return ret_status;
}

/**
 * Set channels to be sampled in the given order
 * @note ADC should be stopped before this is called
*/
inline hal_status_t adc_set_channels(adc_t adc, uint8_t* channels, uint8_t n_channels)
{
    hal_status_t ret_status = HAL_STATUS_OK;

    ADC_ChannelConfTypeDef ch_conf = {0};
    for (uint8_t i = 0; i < n_channels; i++) {
        /** @todo Assert channels[i] is a valid channel */
        ch_conf.Channel = channels[i];
        ch_conf.Rank = i + 1;
        ch_conf.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;

        if (HAL_ADC_ConfigChannel(adc, &ch_conf) != HAL_OK) {
            ret_status = HAL_STATUS_ERROR;
            break;
        }
    }

//    if (ret_status == HAL_STATUS_OK) {
//        adc->Instance->SQR1 &= ~(ADC_SQR1_L);
//        adc->Instance->SQR1 |= ADC_SQR1(n_channels);
//    }
    /** @todo Call ADC init */

    return ret_status;
}

/**
 * Read the end of sequence conversion status flag
*/
inline uint8_t adc_read_eos_flag(adc_t adc)
{
    return __HAL_ADC_GET_FLAG(adc, ADC_FLAG_EOS) != 0;
}

/**
 * Read the end of sequence conversion status flag
*/
inline void adc_clear_eos_flag(adc_t adc)
{
    __HAL_ADC_CLEAR_FLAG(adc, ADC_FLAG_EOS);
}

/**
 * Read the position of dma write pointer relative to start of the buffer
 * @note Currently returns number of samples until the end of the buffer
 */
inline uint32_t adc_dma_get_counter(adc_t adc)
{
    uint32_t rel_to_end = __HAL_DMA_GET_COUNTER(adc->DMA_Handle);
    // uint32_t buffer_len = adc->DMA_Handle->Instance->
    return rel_to_end;
}

#ifdef HAL_ADC_USE_REGISTER_CALLBACKS
/**
 * Register a callback for ADC event
 * @retval `HAL_STATUS_OK` callback registered successfully
 * @retval `HAL_STATUS_ERROR` callback registration failed
 * @note Implement in hal_adc.c
 * @note Multiple registrations should override the last one
*/
inline hal_status_t adc_register_callback(adc_t adc, callback_t callback);
#else
/**
 * ADC end of sequence ISR
 * @note This ISR should be entered every time all channels which are
 * selected are sampled
 * @note This function should be called from lower level (driver's) ISR in the hal_adc.c
*/
__weak void adc_eos_isr(adc_t adc)
{
    UNUSED(adc);
}

/**
 * ADC buffer filled by DMA ISR
 * @note This function should be called from lower level (driver's) ISR in the hal_adc.c
*/
__weak void adc_dma_buffer_filled_isr(adc_t adc)
{
    UNUSED(adc);
}

inline void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    adc_dma_buffer_filled_isr(hadc);
}
#endif