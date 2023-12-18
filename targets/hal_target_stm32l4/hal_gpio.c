#include "hal_target_stm32l4.h"
#include "hal_gpio.h"

/**
 * Read gpio port
 * @note Implement in hal_gpio.c
 */
inline gpio_pin_t gpio_port_read(gpio_port_t port)
{
    return (gpio_pin_t)(((GPIO_TypeDef*)port)->IDR);
}

/**
 * Set GPIO pins to 1
 * @note Implement in hal_gpio.c
 */
inline void gpio_port_set(gpio_port_t port, gpio_pin_t pins)
{
    HAL_GPIO_WritePin(port, pins, GPIO_PIN_SET);
}

/**
 * Set GPIO pins to 0
 * @note Implement in hal_gpio.c
 */
inline void gpio_port_clear(gpio_port_t port, gpio_pin_t pins)
{
    HAL_GPIO_WritePin(port, pins, GPIO_PIN_RESET);
}

/**
 * Toggle GPIO pins
 * @note Implement in hal_gpio.c
 */
inline void gpio_port_toggle(gpio_port_t port, gpio_pin_t pins)
{
    HAL_GPIO_TogglePin(port, pins);
}

#ifdef HAL_GPIO_USE_REGISTER_CALLBACKS
/**
 * Register a callback for GPIO external interrupt
 * @retval `HAL_STATUS_OK` callback registered successfully
 * @retval `HAL_STATUS_ERROR` callback registration failed
 * @note Implement in hal_gpio.c
 * @note Multiple registrations should override the last one
*/
inline hal_status_t gpio_register_callback(gpio_t gpio, gpio_pin_t pin, callback_t callback)
{
    #error "GPIO register callback not implemented"
}
#else
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    /** @todo Implement checking for which port EXTI is setup and passing it */
    gpio_exti_isr((gpio_port_t)0, (gpio_pin_t)GPIO_Pin);
}
#endif