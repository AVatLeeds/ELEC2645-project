#include <stdint.h>
#include "LCD_HD61830_driver.h"

#define HIGH    true
#define LOW     false

HD61830_driver::HD61830_driver(
    void (* set_E_pin_state)(bool state),
    void (* set_RW_pin_state)(bool state),
    void (* set_RS_pin_state)(bool state),
    void (* write_data)(uint8_t byte),
    uint8_t (* read_data)(),
    void (* delay)())
{
    _E = set_E_pin_state;
    _RW = set_RW_pin_state;
    _RS = set_RS_pin_state;
    _write_data = write_data;
    _read_data = read_data;
    _delay = delay;
}

void HD61830_driver::write_register(uint8_t address, uint8_t data)
{
    _RW(HIGH);
    _RS(HIGH);
    _E(HIGH);
    while ((_read_data() >> 7));

    _E(LOW);
    _RW(LOW);
    _delay();

    _E(HIGH);
    _write_data(address);
    _delay();

    _E(LOW);
    _RS(LOW);
    _delay();

    _E(HIGH);
    _write_data(data);
    _delay();

    _E(LOW);
}