#include "hal_target_stm32l4.h"
#include "hal_timer.h"

/**
 * Set timer counting mode
 * @note Can be used to stop the timer using `TIMER_COUNT_MODE_STOP`
 * @note Up to implementation if can be called during timer running
*/
inline hal_status_t timer_set_mode(timer_t timer, timer_count_mode_t timer_mode)
{
    /** @todo Implement */
    return HAL_STATUS_ERROR;
}

/**
 * Set timer period
 * @note By default should be max value (for 16-bit 0xffff)
 * @note If enabled, triggers `timer_period_isr` when timer count hits `period`
 * @note If set to value less than current count while timer is running, timer should trigger isr and reset to 0
*/
inline hal_status_t timer_set_period(timer_t timer, timer_count_t period)
{
    __HAL_TIM_SET_AUTORELOAD(timer, period-1);
    return HAL_STATUS_OK;
}

/**
 * Start the timer (enable counting)
 * @retval `HAL_STATUS_OK` if timer started successfully
 * @retval `HAL_STATUS_ERROR` if timer starting failed
 * @note This should not reset the current value of the counter
*/
inline hal_status_t timer_start(timer_t timer)
{
    hal_status_t ret_status = HAL_STATUS_OK;

    if (HAL_TIM_Base_Start(timer) != HAL_OK) {
        ret_status = HAL_STATUS_ERROR;
    }

    return ret_status;
}

/**
 * Stop the timer (disable counting)
 * @retval `HAL_STATUS_OK` if timer stopped successfully
 * @retval `HAL_STATUS_ERROR` if timer stopping failed
 * @note This should not reset the current value of the counter
*/
inline hal_status_t timer_stop(timer_t timer)
{
    hal_status_t ret_status = HAL_STATUS_OK;

    if (HAL_TIM_Base_Stop(timer) != HAL_OK) {
        ret_status = HAL_STATUS_ERROR;
    }

    return ret_status;
}

/**
 * Clear the timer count value (set to 0)
 * @retval `HAL_STATUS_OK` if timer cleared successfully
 * @retval `HAL_STATUS_ERROR` if timer clearing failed
 * @note Should work and should not change if the timer is enabled or disabled
*/
inline hal_status_t timer_clear(timer_t timer)
{
    __HAL_TIM_SET_COUNTER(timer, 0);

    return HAL_STATUS_OK;
}

/**
 * Get current count of the timer
 * @note This should just act as a macro for reading timer counter register
*/
inline timer_count_t timer_get_count(timer_t timer)
{
    return (timer_count_t)__HAL_TIM_GET_COUNTER(timer);
}

#ifdef HAL_TIMER_USE_REGISTER_CALLBACKS

#else
HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
{
    timer_period_isr(htim);
}
#endif