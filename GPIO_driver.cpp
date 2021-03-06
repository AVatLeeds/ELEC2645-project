#include <stdint.h>
#include "GPIO_driver.h"

GPIO_pin::GPIO_pin(uint8_t port, uint16_t pin)
{
    GPIO_ENABLE(port);
    _port = port;
    _pin = pin;
}

void GPIO_pin::mode(bool mode)
{
    GPIO_SET_MODE(_port, _pin, mode);
}

void GPIO_pin::type(bool type)
{
    GPIO_SET_TYPE(_port, _pin, type);
}

void GPIO_pin::pull(uint8_t pull_type)
{
    GPIO_SET_PUPD(_port, _pin, pull_type);
}

void GPIO_pin::set()
{
    GPIO_SET_PIN(_port, _pin);
}

void GPIO_pin::clear()
{
    GPIO_CLEAR_PIN(_port, _pin);
}

void GPIO_pin::toggle()
{
    GPIO_TOGGLE_PIN(_port, _pin);
}

void GPIO_pin::set_state(bool state)
{
    GPIO_WRITE_PIN(_port, _pin, state);
}

bool GPIO_pin::read()
{
    return (GPIO_IDR(_port) >> _pin) & 1;
}

GPIO_bus::GPIO_bus(uint8_t port, uint16_t offset, uint16_t width)
{
    GPIO_ENABLE(port);
    _port = port;
    _offset = offset;
    _width_mask = ((1 << width) - 1);
    _input_mask = ~(((1 << (width * 2)) - 1) << (offset * 2));
    uint32_t i;
    _output_mask = 0;
    for (i = 1; i < (1U << (width * 2)); i <<= 2)
    {
        _output_mask += i;
    }
    _output_mask <<= (offset * 2);
    // TODO - add options to configure pull-up or pull-down on bus input
    GPIO_PUPD(_port) &= _input_mask; // ensure no pull-ups or pull-downs are used
    GPIO_MODE(_port) &= _input_mask;
}

void GPIO_bus::write(uint16_t data)
{
    GPIO_MODE(_port) |=  _output_mask;
    GPIO_ODR(_port) = (GPIO_ODR(_port) & ~(_width_mask << _offset)) | ((data & _width_mask) << _offset);
}

uint16_t GPIO_bus::read()
{
    GPIO_MODE(_port) &= _input_mask;
    return (GPIO_IDR(_port) & (_width_mask << _offset)) >> _offset;
}

void GPIO_bus::high_z()
{
    GPIO_MODE(_port) &= _input_mask;
}

