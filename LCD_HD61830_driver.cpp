#include <stdint.h>
#include "LCD_HD61830_driver.h"

#define HIGH    true
#define LOW     false

HD61830_driver::HD61830_driver(pin_driver * pin_driver)
{
    _pin_driver = pin_driver;
}

void HD61830_driver::write_register(uint8_t address, uint8_t data)
{
    _pin_driver->RW_pin(HIGH);
    _pin_driver->RS_pin(HIGH);
    _pin_driver->E_pin(HIGH);
    while ((_pin_driver->read_data() >> 7));

    _pin_driver->E_pin(LOW);
    _pin_driver->RW_pin(LOW);
    _pin_driver->delay();

    _pin_driver->E_pin(HIGH);
    _pin_driver->write_data(address);
    _pin_driver->delay();

    _pin_driver->E_pin(LOW);
    _pin_driver->RS_pin(LOW);
    _pin_driver->delay();

    _pin_driver->E_pin(HIGH);
    _pin_driver->write_data(data);
    _pin_driver->delay();

    _pin_driver->E_pin(LOW);
}