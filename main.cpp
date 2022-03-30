#include <stdint.h>
#include <stdio.h>

#include "system.h"
#include "GPIO_driver.h"
#include "USART_driver.h"
#include "bit_banged_UART.h"

uint32_t counter = 0;
uint32_t init = 0;

inline void set_tx(void)
{
    GPIO_ODR(PORTB) |= 0x01;
}

inline void clear_tx(void)
{
    GPIO_ODR(PORTB) &= ~(0x01);
}

extern "C"
{
    int _sbrk() { return -1; }
    int _write() { return -1; }
    int _close() { return -1; }
    int _fstat() { return -1; }
    int _isatty() { return -1; }
    int _lseek() { return -1; }
    int _read() { return -1; }
}

int main(void)
{
    uint32_t i;

    GPIO_ENABLE(PORTB);
    GPIO_MODE(PORTB) = 0x55555555U;
    GPIO_SPEED(PORTB) = 0xFFFF;
    GPIO_ODR(PORTB) = 0b0000000000001000;

    bit_banged_UART UART(9600, &set_tx, &clear_tx);

    UART.transmit_string("Hello world!\n\r");
    for (i = 0; i < 1000000; i++)
    {
        __asm__("nop");
    }

    // Dump GPIO registers
    uint32_t GPIO_A_base_address = 0x48000000;

    printf("%d\n\r", GPIO_A_base_address);


    while(1)
    {
        
    }

    return 0;
}

/*
int main(void)
{
    // *((uint32_t *)(0xE000E010 + 0)) |= 3;
    //RCC_APB1ENR1 &= ~(1U << 17);

    RCC_APB1ENR1 |= (1U << 17);
    RCC_CCIPR |= (1U << 2);

    GPIO_ENABLE(PORTC);
    GPIO_MODE(PORTC) = 0x5555U;

    GPIO_ENABLE(PORTB);
    GPIO_MODE(PORTB) = 0x55555555U;
    GPIO_SPEED(PORTB) = 0xFFFF;
    GPIO_ODR(PORTB) = 0b0000000000001000;

    GPIO_ENABLE(PORTA);
    GPIO_MODE(PORTA) |= 0b10100000;
    GPIO_SPEED(PORTA) |= (15U << 4);
    GPIO_AFR_L(PORTA) |= (7U << 8);
    GPIO_AFR_L(PORTA) |= (7U << 12);
    

    USART_CR1 = 0x00;
    //USART_WORDLENGTH8();
    //USART_OVERSAMP16();
    USART_BAUD9600();
    //USART_STOPBIT2();
    //USART_ENABLE();
    
    //USART_TE();

    USART_CR1 = 0x0D;

    uint32_t i;
    for (i = 0; i < 65536; i ++)
    {
        __asm__("nop");
    }

    while (1)
    {
        //sys_tick_handler();

        //USART_TDR = 0x65;
        //while (!(USART_ISR & (1U << 6)));

        GPIO_ODR(PORTB) |= 0x10;
        delay(800);
        GPIO_ODR(PORTB) &= ~(0x10);
        delay(79);
        GPIO_ODR(PORTB) |= 0x10;
        delay(79);
        GPIO_ODR(PORTB) &= ~(0x10);
        delay(79);
        GPIO_ODR(PORTB) &= ~(0x10);
        delay(79);
        GPIO_ODR(PORTB) &= ~(0x10);
        delay(79);
        GPIO_ODR(PORTB) &= ~(0x10);
        delay(79);
        GPIO_ODR(PORTB) &= ~(0x10);
        delay(79);
        GPIO_ODR(PORTB) |= 0x10;
        delay(79);
        GPIO_ODR(PORTB) &= ~(0x10);
        delay(79);
        GPIO_ODR(PORTB) |= 0x10;
        delay(79);

        //init = get_current_ticks();
        //while (init)
        //{
        //    init --;
        //}

        //USART_TDR = 0x65;
        //while (!(USART_ISR & (1U << 6)));
        //delay(79);

        //GPIO_ODR(PORTB) &= ~(0x10);

        //init = get_current_ticks();
        //while (init)
        //{
        //    init --;
        //}

    }
    
    return 0;
}*/