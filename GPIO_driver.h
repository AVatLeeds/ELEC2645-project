#ifndef GPIO_DRIVER_H
#define GPIO_DRIVER_H

#include <stdint.h>
#include "system.h"

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

#define GPIO_MODE(PORT)  *((volatile uint32_t *)(GPIO_PORT_BASE((PORT)) + 0x00))
#define GPIO_TYPE(PORT)  *((volatile uint16_t *)(GPIO_PORT_BASE((PORT)) + 0x04))
#define GPIO_SPEED(PORT) *((volatile uint32_t *)(GPIO_PORT_BASE((PORT)) + 0x08))
#define GPIO_PUPDR(PORT) *((volatile uint32_t *)(GPIO_PORT_BASE((PORT)) + 0x0C))
#define GPIO_IDR(PORT)   *((volatile uint16_t *)(GPIO_PORT_BASE((PORT)) + 0x10))
#define GPIO_ODR(PORT)   *((volatile uint16_t *)(GPIO_PORT_BASE((PORT)) + 0x14))
#define GPIO_BSRR(PORT)  *((volatile uint32_t *)(GPIO_PORT_BASE((PORT)) + 0x18))
#define GPIO_LOCKR(PORT) *((volatile uint16_t *)(GPIO_PORT_BASE((PORT)) + 0x1C))
#define GPIO_AFR_L(PORT) *((volatile uint32_t *)(GPIO_PORT_BASE((PORT)) + 0x20))
#define GPIO_AFR_H(PORT) *((volatile uint32_t *)(GPIO_PORT_BASE((PORT)) + 0x24))
#define GPIO_BRR(PORT)   *((volatile uint16_t *)(GPIO_PORT_BASE((PORT)) + 0x28))
#define GPIO_ASCR(PORT)  *((volatile uint16_t *)(GPIO_PORT_BASE((PORT)) + 0x2C))

//enum ports {PORTA, PORTB, PORTC, PORTD, PORTE, PORTF, PORTG, PORTH};

/*
class GPIO_class
{
    public:
    GPIO_class();

    void set_bit(uint32_t bit);
    void set_port();
    void clear_bit(uint32_t bit);
    void clear_port();
    void reset_bit(uint32_t bit);
    void reset_port();
    void write_port(uint16_t value);

    private:
};
*/

#endif