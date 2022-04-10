#ifndef GPIO_DRIVER_H
#define GPIO_DRIVER_H

#include <stdint.h>
#include "system.h"

#define HIGH    1
#define LOW     0

#define OUT     1
#define IN      0

#define PORTA   0
#define PORTB   1
#define PORTC   2
#define PORTD   3
#define PORTE   4
#define PORTF   5
#define PORTG   6
#define PORTH   7

#define GPIO_PORT_BASE(PORT)    0x48000000 + (0x400 * (PORT))

#define GPIO_ENABLE(PORT)       RCC_AHB2ENR |= (1U << (PORT))
#define GPIO_DISABLE(PORT)      RCC_AHB2ENR &= ~(1U << (PORT))  

#define GPIO_MODE(PORT)     *((volatile uint32_t *)(GPIO_PORT_BASE((PORT)) + 0x00))
#define GPIO_TYPE(PORT)     *((volatile uint16_t *)(GPIO_PORT_BASE((PORT)) + 0x04))
#define GPIO_SPEED(PORT)    *((volatile uint32_t *)(GPIO_PORT_BASE((PORT)) + 0x08))
#define GPIO_PUPD(PORT)     *((volatile uint32_t *)(GPIO_PORT_BASE((PORT)) + 0x0C))
#define GPIO_IDR(PORT)      *((volatile uint16_t *)(GPIO_PORT_BASE((PORT)) + 0x10))
#define GPIO_ODR(PORT)      *((volatile uint16_t *)(GPIO_PORT_BASE((PORT)) + 0x14))
#define GPIO_BSRR(PORT)     *((volatile uint32_t *)(GPIO_PORT_BASE((PORT)) + 0x18))
#define GPIO_LOCKR(PORT)    *((volatile uint16_t *)(GPIO_PORT_BASE((PORT)) + 0x1C))
#define GPIO_AFR_L(PORT)    *((volatile uint32_t *)(GPIO_PORT_BASE((PORT)) + 0x20))
#define GPIO_AFR_H(PORT)    *((volatile uint32_t *)(GPIO_PORT_BASE((PORT)) + 0x24))
#define GPIO_BRR(PORT)      *((volatile uint16_t *)(GPIO_PORT_BASE((PORT)) + 0x28))
#define GPIO_ASCR(PORT)     *((volatile uint16_t *)(GPIO_PORT_BASE((PORT)) + 0x2C))

#define INPUT               0b00
#define OUTPUT              0b01
#define ALTERNATE_FUNCTION  0b10
#define ANALOGUE            0b11

#define GPIO_SET_MODE(PORT, PIN, MODE)  GPIO_MODE(PORT) = (GPIO_MODE(PORT) & ~(0b11 << (PIN * 2))) | (MODE << (PIN * 2)) 

#define PUSH_PULL   0b0
#define OPEN_DRAIN  0b1

#define GPIO_SET_TYPE(PORT, PIN, TYPE)  GPIO_TYPE(PORT) = (GPIO_TYPE(PORT) & ~(1 << PIN)) | (TYPE << PIN)

#define LOW_SPEED       0b00
#define MEDIUM_SPEED    0b01
#define HIGH_SPEED      0b10
#define VERY_HIGH_SPEED 0b11

#define GPIO_SET_SPEED(PORT, PIN, SPEED)    GPIO_SPEED(PORT) = (GPIO_SPEED(PORT) & ~(0b11 << (PIN * 2))) | (SPEED << (PIN * 2))

#define NONE        0b00
#define PULL_UP     0b01
#define PULL_DOWN   0b10

#define GPIO_SET_PUPD(PORT, PIN, PUPD)    GPIO_PUPD(PORT) = (GPIO_PUPD(PORT) & ~(0b11 << (PIN * 2))) | (PUPD << (PIN * 2))

#define GPIO_WRITE_PIN(PORT, PIN, STATE)    GPIO_ODR(PORT) = (GPIO_ODR(PORT) & ~(1 << PIN)) | (STATE << PIN)
#define GPIO_SET_PIN(PORT, PIN)             GPIO_BSRR(PORT) |= (1 << PIN)
#define GPIO_CLEAR_PIN(PORT, PIN)           GPIO_BSRR(PORT) |= (1 << (PIN + 16))
#define GPIO_TOGGLE_PIN(PORT, PIN)          GPIO_ODR(PORT) ^= (1 << PIN)

#define AF0     0b0000
#define AF1     0b0001
#define AF2     0b0010
#define AF3     0b0011
#define AF4     0b0100
#define AF5     0b0101
#define AF6     0b0110
#define AF7     0b0111
#define AF8     0b1000
#define AF9     0b1001
#define AF10    0b1010
#define AF11    0b1011
#define AF12    0b1100
#define AF13    0b1101
#define AF14    0b1110
#define AF15    0b1111

#define GPIO_SET_ALTERNATE_FUNCTION_H(PORT, PIN, AF)    GPIO_AFR_H(PORT) = (GPIO_AFR_H(PORT) & ~(0b1111 << (PIN * 4))) | (AF << (PIN * 4))
#define GPIO_SET_ALTERNATE_FUNCTION_L(PORT, PIN, AF)    GPIO_AFR_L(PORT) = (GPIO_AFR_L(PORT) & ~(0b1111 << (PIN * 4))) | (AF << (PIN * 4))
#define GPIO_SET_ALTERNATE_FUNCTION(PORT, PIN, AF)      ((PIN > 7) ? GPIO_SET_ALTERNATE_FUNCTION_H(PORT, (PIN - 8), AF) : GPIO_SET_ALTERNATE_FUNCTION_L(PORT, PIN, AF))

class GPIO_pin
{
    public:

    GPIO_pin(uint8_t port, uint16_t pin);
    void mode(bool mode);
    void set();
    void clear();
    void set_state(bool state);
    bool read();

    private:

    uint8_t _port;
    uint16_t _pin;
};

class GPIO_bus
{
    public:

    GPIO_bus(uint8_t port, uint16_t offset, uint16_t width);
    void write(uint16_t data);
    uint16_t read();

    private:

    uint8_t _port;
    uint16_t _offset;
    uint8_t _width_mask;
    uint32_t _input_mask;
    uint32_t _output_mask;
};

#endif