/**
 * Timer clocks and frequency division are left to be implement for the specific platform.
*/

#ifndef HAL_TIMER_H
#define HAL_TIMER_H

#include "stdint.h"
#include "hal_core.h"

#ifndef HAL_TIMER_TYPEDEF
    #error "HAL_TIMER_TYPEDEF not defined"
    __TEMPLATE_TYPEDEF(timer_t);
#else
    typedef HAL_TIMER_TYPEDEF timer_t;
#endif

#ifdef HAL_TIMER_32BIT
    typedef uint32_t timer_count_t;
#else
    typedef uint16_t timer_count_t;
#endif

typedef enum {
    TIMER_COUNT_MODE_STOP,
    TIMER_COUNT_MODE_UP,
    // TIMER_COUNT_MODE_UP_FULL,
    // TIMER_COUNT_MODE_DOWN,
    // TIMER_COUNT_MODE_UP_DOWN
} timer_count_mode_t;

/**
 * Set timer counting mode
 * @note Can be used to stop the timer using `TIMER_COUNT_MODE_STOP`
 * @note Up to implementation if can be called during timer running
*/
inline hal_status_t timer_set_mode(timer_t timer, timer_count_mode_t timer_mode);

/**
 * Set timer period
 * @note By default should be max value (for 16-bit 0xffff)
 * @note If enabled, triggers `timer_period_isr` when timer count hits `period`
 * @note If set to value less than current count while timer is running, timer should trigger isr and reset to 0
*/
inline hal_status_t timer_set_period(timer_t timer, timer_count_t period);

/**
 * Start the timer (enable counting)
 * @retval `HAL_STATUS_OK` if timer started successfully
 * @retval `HAL_STATUS_ERROR` if timer starting failed
 * @note This should not reset the current value of the counter
*/
inline hal_status_t timer_start(timer_t timer);

/**
 * Stop the timer (disable counting)
 * @retval `HAL_STATUS_OK` if timer stopped successfully
 * @retval `HAL_STATUS_ERROR` if timer stopping failed
 * @note This should not reset the current value of the counter
*/
inline hal_status_t timer_stop(timer_t timer);

/**
 * Clear the timer count value (set to 0)
 * @retval `HAL_STATUS_OK` if timer cleared successfully
 * @retval `HAL_STATUS_ERROR` if timer clearing failed
 * @note Should work and should not change if the timer is enabled or disabled
*/
inline hal_status_t timer_clear(timer_t timer);

/**
 * Get current count of the timer
 * @note This should just act as a macro for reading timer counter register
*/
inline timer_count_t timer_get_count(timer_t timer);

#ifdef HAL_TIMER_USE_REGISTER_CALLBACKS

#else
/**
 * Timer periodic ISR
 * Called when timer count hits timer period value
*/
inline void timer_period_isr(timer_t timer);
#endif

#endif /* HAL_TIMER_H */