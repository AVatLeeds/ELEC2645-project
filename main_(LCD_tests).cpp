#include <stdint.h>
#include <stddef.h>
//#include <list>
//#include <vector>
//#include <string>
#include <stdio.h>

#include "system.h"
#include "systick.h"
#include "GPIO_driver.h"
#include "USART_driver.h"
#include "LCD_HD61830_driver.h"

#define RED     1
#define GREEN   2
#define YELLOW  3
#define BLUE    4
#define PURPLE  5
#define CYAN    6
#define WHITE   7

USART uart(USART_2);

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

void print_reg_value(USART * UART, uint32_t reg_val)
{
    //UART->newline();
    UART->print_in_binary((reg_val) >> 24);
    UART->print_in_binary((reg_val) >> 16);
    UART->transmit_string(" ");
    UART->print_in_binary((reg_val) >> 8);
    UART->print_in_binary((reg_val) >> 0);
    UART->newline();
}

class LCD_pin_driver : public pin_driver
{
    public:

    LCD_pin_driver():
    _E_pin(PORTC, 8),
    _RW_pin(PORTC, 9),
    _RS_pin(PORTC, 10),
    _CS_pin(PORTC, 11),
    _RES_pin(PORTC, 12),
    _LCD_data_bus(PORTC, 0, 8)
    {
        _E_pin.mode(OUTPUT);
        _E_pin.type(OPEN_DRAIN);
        _E_pin.pull(NONE);

        _RW_pin.mode(OUTPUT);
        _RW_pin.type(OPEN_DRAIN);
        _RW_pin.pull(NONE);

        _RS_pin.mode(OUTPUT);
        _RS_pin.type(OPEN_DRAIN);
        _RS_pin.pull(NONE);

        _CS_pin.mode(OUTPUT);
        _CS_pin.type(OPEN_DRAIN);
        _CS_pin.pull(NONE);

        _RES_pin.mode(OUTPUT);
        _RES_pin.type(OPEN_DRAIN);
        _RES_pin.pull(NONE);
    }

    void enable(bool state) override
    {
        _E_pin.set_state(state);
    }

    void rw_state(bool state) override
    {
        _RW_pin.set_state(state);
    }

    void rs_state(bool state) override
    {
        _RS_pin.set_state(state);
    }

    void chip_select(bool state) override
    {
        _CS_pin.set_state(state);
    }

    void reset(bool state) override
    {
        _RES_pin.set_state(state);
    }

    void write_data(uint8_t data) override
    {
        _LCD_data_bus.write(data);
    }

    uint8_t read_data() override
    {
        return _LCD_data_bus.read();
    }

    void delay() override
    {
        delay_ms(1);
    }

    private:

    GPIO_pin _E_pin;
    GPIO_pin _RW_pin;
    GPIO_pin _RS_pin;
    GPIO_pin _CS_pin;
    GPIO_pin _RES_pin;
    GPIO_bus _LCD_data_bus;
};

int main(void)
{
    //std::string test_string = "Test string.";
    //std::list<std::string> my_list;
    systick_init();

    Status status(PORTB, 3, 4, 5);

    GPIO_ENABLE(PORTA);
    GPIO_SET_MODE(PORTA, 2, ALTERNATE_FUNCTION);
    GPIO_SET_MODE(PORTA, 3, ALTERNATE_FUNCTION);
    GPIO_SET_TYPE(PORTA, 2, PUSH_PULL);
    GPIO_SET_TYPE(PORTA, 3, PUSH_PULL);
    GPIO_SET_SPEED(PORTA, 2, HIGH_SPEED);
    GPIO_SET_SPEED(PORTA, 3, HIGH_SPEED);
    GPIO_SET_ALTERNATE_FUNCTION(PORTA, 2, AF7);
    GPIO_SET_ALTERNATE_FUNCTION(PORTA, 3, AF7);
    
    print_reg_value(&uart, USART_CR1(USART_2));

    uart.set_usart(USART_2);
    uart.init(9600);

    print_reg_value(&uart, USART_CR1(USART_2));
    print_reg_value(&uart, USART_CR2(USART_2));
    print_reg_value(&uart, USART_CR3(USART_2));
    print_reg_value(&uart, USART_BRR(USART_2));
    print_reg_value(&uart, GPIO_MODE(PORTA));
    print_reg_value(&uart, RCC_APB1ENR1);
    print_reg_value(&uart, RCC_CCIPR);
    print_reg_value(&uart, GPIO_AFR_L(PORTA));
    print_reg_value(&uart, USART_CR1(USART_2));

    status = PURPLE;
    delay_ms(1000);
    
    LCD_pin_driver LCD_pin_driver;
    HD61830_driver LCD(&LCD_pin_driver);

    //LCD.write_register(0x00, 0b00000000);
    LCD.write_register(0x00, 0x34); // display on, master mode, character mode, internal CG, cursor blink
    LCD.write_register(0x01, 0x75); //0b01110101); // 6x8 font
    LCD.write_register(0x02, 0x27); // 30 horizontal characters
    LCD.write_register(0x03, 0x3F); // 64 time divisions?
    LCD.write_register(0x04, 0x07); // 8 pixel tall cursor
    LCD.write_register(0x08, 0x00); // display start address low
    LCD.write_register(0x09, 0x00); // display start address high
    LCD.write_register(0x0A, 0x00); // cursor address low
    LCD.write_register(0x0B, 0x00); // cursor address high

    LCD.write_register(0x0A, 0xFF); // cursor address low
    LCD.write_register(0x0B, 0x01); // cursor address high

    while (1)
    {
        status = GREEN;
        LCD.write_register(0x0C, 65);
        delay_ms(500);
        status = RED;
        LCD.write_register(0x0C, 65);
        delay_ms(500);
    }

    return 0;
}