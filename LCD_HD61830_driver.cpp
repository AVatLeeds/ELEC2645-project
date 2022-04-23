#include <stdint.h>
#include "LCD_HD61830_driver.h"

#define HIGH    true
#define LOW     false

HD61830_driver::HD61830_driver(pin_driver * pin_driver)
{
    _pin_driver = pin_driver;
    _pin_driver->write_data(0x00);
    _pin_driver->chip_select(HIGH);
    _pin_driver->enable(LOW);
    _pin_driver->reset(LOW);
    _pin_driver->delay();
    _pin_driver->reset(HIGH);
    _pin_driver->delay();
}

void nop() __attribute__((optimize(0)));
void nop()
{
    __asm__("nop");
}

void HD61830_driver::write_register(uint8_t address, uint8_t data)
{
    _pin_driver->chip_select(LOW);

    _pin_driver->rs_state(HIGH);
    _pin_driver->rw_state(HIGH);
    _pin_driver->enable(HIGH);
    _pin_driver->delay();
    _pin_driver->enable(LOW);

    _pin_driver->rw_state(LOW);
    _pin_driver->write_data(~address);
    _pin_driver->delay();

    _pin_driver->enable(HIGH);
    _pin_driver->delay();
    _pin_driver->enable(LOW);
    _pin_driver->delay();

    _pin_driver->rs_state(LOW);
    _pin_driver->write_data(~data);
    _pin_driver->delay();

    _pin_driver->enable(HIGH);
    _pin_driver->delay();
    _pin_driver->enable(LOW);
    _pin_driver->delay();
    _pin_driver->write_data(0x0);

    _pin_driver->chip_select(HIGH);

    /*
    _pin_driver->RW_pin(HIGH);
    _pin_driver->RS_pin(HIGH);
    _pin_driver->E_pin(HIGH);
    do
    {
        nop();
        nop();
        nop();
    }
    while (_pin_driver->read_data() >> 7);
    //_pin_driver->E_pin(LOW);

    _pin_driver->RW_pin(LOW);
    //_pin_driver->RS_pin(HIGH);
    _pin_driver->write_data(address);
    _pin_driver->E_pin(HIGH);
    nop();
    nop();
    nop();
    _pin_driver->E_pin(LOW);
    nop();
    nop();
    nop();
    
    //_pin_driver->RW_pin(LOW);
    _pin_driver->RS_pin(LOW);
    _pin_driver->write_data(data);
    _pin_driver->E_pin(HIGH);
    nop();
    nop();
    nop();
    _pin_driver->E_pin(LOW);
    nop();
    nop();
    nop();

    _pin_driver->read_data();
    */

    /*
    _pin_driver->RW_pin(HIGH);
    _pin_driver->RS_pin(HIGH);
    _pin_driver->E_pin(HIGH);
    _pin_driver->delay();
    while (_pin_driver->read_data() >> 7);

    _pin_driver->E_pin(LOW);
    _pin_driver->RW_pin(LOW);
    _pin_driver->write_data(address);
    _pin_driver->delay();

    _pin_driver->E_pin(HIGH);
    _pin_driver->delay();
    _pin_driver->E_pin(LOW);
    _pin_driver->delay();

    _pin_driver->RS_pin(LOW);
    _pin_driver->write_data(data);

    _pin_driver->E_pin(HIGH);
    _pin_driver->delay();
    _pin_driver->E_pin(LOW);
    _pin_driver->delay();

    _pin_driver->read_data();
    */

    /*
    while (_pin_driver->read_data() >> 7);
    _pin_driver->RW_pin(LOW);
    _pin_driver->write_data(address);

    _pin_driver->delay();
    _pin_driver->E_pin(LOW);
    _pin_driver->delay();
    _pin_driver->E_pin(HIGH);
    _pin_driver->delay();

    _pin_driver->RS_pin(LOW);
    _pin_driver->write_data(data);

    _pin_driver->delay();
    _pin_driver->E_pin(LOW);
    _pin_driver->delay();
    _pin_driver->E_pin(HIGH);
    _pin_driver->delay();

    _pin_driver->RS_pin(HIGH);
    _pin_driver->RW_pin(HIGH);
    while (_pin_driver->read_data() >> 7);
    */

    /*
    _pin_driver->RW_pin(LOW);
    _pin_driver->RS_pin(HIGH);
    _pin_driver->write_data(address);
    _pin_driver->delay();
    _pin_driver->RW_pin(LOW);
    _pin_driver->RS_pin(LOW);
    _pin_driver->write_data(data);
    _pin_driver->delay();
    _pin_driver->E_pin(HIGH);
    _pin_driver->RW_pin(HIGH);
    _pin_driver->RS_pin(HIGH);
    do
    {
        _pin_driver->delay();
    } while (_pin_driver->read_data() >> 7);
    */

    /*
    _pin_driver->RW_pin(HIGH);
    _pin_driver->RS_pin(HIGH);
    _pin_driver->E_pin(HIGH);
    _pin_driver->delay();
    //_pin_driver->E_pin(LOW);
    //_pin_driver->delay();
    while (_pin_driver->read_data() >> 7);

    _pin_driver->E_pin(LOW);
    _pin_driver->RW_pin(LOW);
    _pin_driver->write_data(address);
    _pin_driver->delay();
    //nop();
    //nop();

    _pin_driver->E_pin(HIGH);
    _pin_driver->delay();
    //nop();
    //nop();
    //nop();
    //nop();
    //nop();
    //nop();

    _pin_driver->E_pin(LOW);
    _pin_driver->RS_pin(LOW);
    _pin_driver->write_data(data);
    _pin_driver->delay();
    //nop();
    //nop();

    _pin_driver->E_pin(HIGH);
    _pin_driver->delay();
    //nop();
    //nop();
    //nop();
    //nop();
    //nop();
    //nop();

    _pin_driver->E_pin(LOW);
    //_pin_driver->write_data(0);
    _pin_driver->read_data();
    _pin_driver->delay();
    */
}