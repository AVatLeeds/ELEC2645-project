#ifndef STATUS_LED_H
#define STATUS_LED_H

#include <stdint.h>
#include "GPIO_driver.h"

#define RED     1
#define GREEN   2
#define YELLOW  3
#define BLUE    4
#define PURPLE  5
#define CYAN    6
#define WHITE   7

class status_LED
{
    public:
    status_LED(uint8_t port, uint8_t pin1, uint8_t pin2, uint8_t pin3);
    void set_status(uint8_t status);
    uint8_t get_status();
    uint8_t operator=(uint8_t status);
    private:
    uint32_t _port;
    uint32_t _pin1;
    uint32_t _pin2;
    uint32_t _pin3;
    uint8_t _status;
};

status_LED::status_LED(uint8_t port, uint8_t pin1, uint8_t pin2, uint8_t pin3)
{
    _port = port;
    _pin1 = pin1;
    _pin2 = pin2;
    _pin3 = pin3;
    GPIO_ENABLE(_port);
    GPIO_SET_MODE(_port, _pin1, OUTPUT);
    GPIO_SET_MODE(_port, _pin2, OUTPUT);
    GPIO_SET_MODE(_port, _pin3, OUTPUT);
    _status = 0;
}

void status_LED::set_status(uint8_t status)
{
    status &= 0b111;
    _status = status;
    GPIO_WRITE_PIN(_port, _pin1, (status & 0b1));
    GPIO_WRITE_PIN(_port, _pin2, ((status >> 1) & 0b1));
    GPIO_WRITE_PIN(_port, _pin3, ((status >> 2) & 0b1));
}

uint8_t status_LED::get_status()
{
    return _status;
}

uint8_t status_LED::operator=(uint8_t status)
{
    status &= 0b111;
    _status = status;
    GPIO_WRITE_PIN(_port, _pin1, (status & 0b1));
    GPIO_WRITE_PIN(_port, _pin2, ((status >> 1) & 0b1));
    GPIO_WRITE_PIN(_port, _pin3, ((status >> 2) & 0b1));
    return status;
}

#endif