#include <stdint.h>
#include <stddef.h>

#include "system.h"
#include "systick.h"
#include "GPIO_driver.h"
#include "USART_driver.h"
#include "bit_banged_UART.h"

#define RED     1
#define GREEN   2
#define YELLOW  3
#define BLUE    4
#define PURPLE  5
#define CYAN    6
#define WHITE   7

void set_tx(void)
{
    GPIO_WRITE_PIN(PORTB, 0, 1);
}

void clear_tx(void)
{
    GPIO_WRITE_PIN(PORTB, 0, 0);
}

class Status
{
    public:
    Status(uint32_t port, uint32_t pin1, uint32_t pin2, uint32_t pin3);
    void set_status(uint8_t status);
    uint8_t operator=(uint8_t status);
    private:
    uint32_t _port;
    uint32_t _pin1;
    uint32_t _pin2;
    uint32_t _pin3;
};

Status::Status(uint32_t port, uint32_t pin1, uint32_t pin2, uint32_t pin3)
{
    _port = port;
    _pin1 = pin1;
    _pin2 = pin2;
    _pin3 = pin3;
    GPIO_ENABLE(_port);
    GPIO_SET_MODE(_port, _pin1, OUTPUT);
    GPIO_SET_MODE(_port, _pin2, OUTPUT);
    GPIO_SET_MODE(_port, _pin3, OUTPUT);
}

void Status::set_status(uint8_t status)
{
    status &= 0b111;
    GPIO_WRITE_PIN(_port, _pin1, (status & 0b1));
    GPIO_WRITE_PIN(_port, _pin2, ((status >> 1) & 0b1));
    GPIO_WRITE_PIN(_port, _pin3, ((status >> 2) & 0b1));
}

uint8_t Status::operator=(uint8_t status)
{
    status &= 0b111;
    GPIO_WRITE_PIN(_port, _pin1, (status & 0b1));
    GPIO_WRITE_PIN(_port, _pin2, ((status >> 1) & 0b1));
    GPIO_WRITE_PIN(_port, _pin3, ((status >> 2) & 0b1));
    return status;
}


/*
extern "C"
{
    int _sbrk() { return -1; }
    int _write(int fd, char * buf, size_t count)
    {
        GPIO_ODR(PORTB) |= 0b0000000000111000;
        UART.transmit_bytes((uint8_t *)buf, (uint32_t)count);
        return count;
    }
    int _close() { return -1; }
    int _fstat() { return -1; }
    int _isatty() { return -1; }
    int _lseek() { return -1; }
    int _read() { return -1; }
}*/

void print_reg_value(bit_banged_UART * UART, uint32_t reg_val)
{
    UART->newline();
    UART->print_in_binary((reg_val) >> 24);
    UART->print_in_binary((reg_val) >> 16);
    UART->transmit_string(" ");
    UART->print_in_binary((reg_val) >> 8);
    UART->print_in_binary((reg_val) >> 0);
    UART->newline();
}

int main(void)
{
    uint32_t i;
    uint32_t reg_ptr = 0;

    systick_init();
    char * test_string = "test string\n\r";

    GPIO_ENABLE(PORTB);
    GPIO_SET_MODE(PORTB, 0, OUTPUT);
    bit_banged_UART UART(9600, set_tx, clear_tx);
    Status status(PORTB, 3, 4, 5);
    UART.transmit_string("START");

    GPIO_ENABLE(PORTA);
    GPIO_SET_MODE(PORTA, 2, ALTERNATE_FUNCTION);
    GPIO_SET_MODE(PORTA, 3, ALTERNATE_FUNCTION);
    GPIO_SET_TYPE(PORTA, 2, PUSH_PULL);
    GPIO_SET_TYPE(PORTA, 3, PUSH_PULL);
    GPIO_SET_SPEED(PORTA, 2, HIGH_SPEED);
    GPIO_SET_SPEED(PORTA, 3, HIGH_SPEED);
    GPIO_SET_ALTERNATE_FUNCTION(PORTA, 2, AF7);
    GPIO_SET_ALTERNATE_FUNCTION(PORTA, 3, AF7);
    USART uart(USART_2);
    print_reg_value(&UART, USART_CR1(USART_2));

    uart.init(9600);

    print_reg_value(&UART, USART_CR1(USART_2));
    print_reg_value(&UART, USART_CR2(USART_2));
    print_reg_value(&UART, USART_CR3(USART_2));
    print_reg_value(&UART, USART_BRR(USART_2));
    print_reg_value(&UART, GPIO_MODE(PORTA));
    print_reg_value(&UART, RCC_APB1ENR1);
    print_reg_value(&UART, RCC_CCIPR);
    print_reg_value(&UART, GPIO_AFR_L(PORTA));
    print_reg_value(&UART, USART_CR1(USART_2));

    status = PURPLE;
    delay_ms(1000);

    while (1)
    {
        status = GREEN;
        uart.transmit_string("Hello world!\n\r");
        delay_ms(100);
        status = RED;
        delay_ms(100);
    }

    return 0;
}