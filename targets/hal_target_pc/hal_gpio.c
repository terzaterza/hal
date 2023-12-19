#include "hal_target_pc.h"
#include "hal_gpio.h"

static hal_target_pc_gpio_t gpio_port[HAL_GPIO_PORT_COUNT];

/**
 * Read gpio port
 * @note Implement in hal_gpio.c
 */
inline gpio_pin_t gpio_port_read(gpio_port_t port)
{
    return port->in_reg;
}

/**
 * Set GPIO pins to 1
 * @note Implement in hal_gpio.c
 */
inline void gpio_port_set(gpio_port_t port, gpio_pin_t pins)
{
    port->out_reg |= pins;
}

/**
 * Set GPIO pins to 0
 * @note Implement in hal_gpio.c
 */
inline void gpio_port_clear(gpio_port_t port, gpio_pin_t pins)
{
    port->out_reg &= ~pins;
}

/**
 * Toggle GPIO pins
 * @note Implement in hal_gpio.c
 */
inline void gpio_port_toggle(gpio_port_t port, gpio_pin_t pins)
{
    port->out_reg ^= pins;
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
#endif

/**
 * Called if the first byte from the socket stream was SOCKET_GPIO_ID
 * 
 * Socket GPIO message - <VALUE - size in bytes>
 * <PORT_INDEX - 1> <PINS_TO_CHANGE - 2> <PIN_VALUES - 2>
*/
void peripheral_socket_handle_gpio()
{
    /** @todo Can move receiving port id to peripheral socket and passing as argument */
    uint8_t message[1 + 2 + 2];

    /* Get rest of the message from the socket */
    socket_read(message, sizeof(message));

    /* Get port selected by the message */
    if (message[0] >= HAL_GPIO_PORT_COUNT) {
        return;
    }
    gpio_port_t port = gpio_port + message[0];

    /* Get which pins are being changed */
    /** @todo & pins with port->dir to only affect input regs */
    gpio_pin_t pins = *(gpio_pin_t*)(message + 1);

    /* Get new values for the input register */
    gpio_pin_t values = *(gpio_pin_t*)(message + 3);

    /* Check for pin state changes trigger interrupts if enabled */
    gpio_pin_t diff = (port->in_reg ^ values) & pins;
    gpio_pin_t triggers = ~(values ^ port->intr_edge) & diff & port->intr;

    /* Update port input register */
    port->in_reg |= values & pins;
    port->in_reg &= ~(~values & pins);

    /* Interrupt callback */
    if (triggers != 0) {
        gpio_exti_isr(port, triggers);
    }
}