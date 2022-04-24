#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <sys/stat.h>

#include "status_LED.h"
#include "system.h"
#include "systick.h"
#include "GPIO_driver.h"
#include "USART_driver.h"
#include "SPI_driver.h"

// temporary for experiments only
void nop() __attribute__((optimize(0)));
void nop()
{
    __asm__("nop");
}

status_LED status(PORTB, 3, 4, 5);

USART_driver uart(USART_2, 115200);

extern "C"
{
    int _sbrk()
    {
        uart.print_string("in sbrk\n");
        return 0;
    }

    int _write(int fd, char * ptr, int len)
    {
        uart.print_string("in write\n");
        uart.transmit_bytes((uint8_t *)ptr, len);
        return len;
    }

    int _close(int fd)
    {
        uart.print_string("in close\n");
        return -1;
    }

    int _lseek(int fd, int ptr, int dir)
    {
        uart.print_string("in lseek\n");
        return -1;
    }

    int _read(int fd, char * ptr, int len)
    {
        uart.print_string("in read\n");
        return -1;
    }

    int _fstat()
    {
        uart.print_string("in fstat\n");
        return S_IFCHR;
    }

    int _isatty(int fd)
    {
        uart.print_string("in sbrk\n");
        return 1;
    }
}

SPI_driver SPI(SPI1);

void setup_uart_pins() // this needs to go in the USART class constructor when I have time
{
    GPIO_ENABLE(PORTA);
    // USART2 transmit and receive are on pins 2 and 3 of PORTA
    GPIO_SET_MODE(PORTA, 2, ALTERNATE_FUNCTION);
    GPIO_SET_MODE(PORTA, 3, ALTERNATE_FUNCTION);
    GPIO_SET_TYPE(PORTA, 2, PUSH_PULL);
    GPIO_SET_TYPE(PORTA, 3, PUSH_PULL);
    GPIO_SET_SPEED(PORTA, 2, HIGH_SPEED);
    GPIO_SET_SPEED(PORTA, 3, HIGH_SPEED);
    // alternate function 7 is USART
    GPIO_SET_ALTERNATE_FUNCTION(PORTA, 2, AF7);
    GPIO_SET_ALTERNATE_FUNCTION(PORTA, 3, AF7);
}

void SPI_setup()
{
    /* on the STM32L476RG SPI1 is available as alternate function 5 on a number of pins.
    I have chosen these ones:
        - NSS - PA4
        - SCK - PA5
        - MISO - PA6
        - MOSI - PA7
    */

    GPIO_ENABLE(PORTA);

    //GPIO_SET_MODE(PORTA, 4, ALTERNATE_FUNCTION); NSS functionality not needed for ADC
    GPIO_SET_MODE(PORTA, 5, ALTERNATE_FUNCTION);
    GPIO_SET_MODE(PORTA, 6, ALTERNATE_FUNCTION);
    GPIO_SET_MODE(PORTA, 7, ALTERNATE_FUNCTION);

    //GPIO_SET_ALTERNATE_FUNCTION(PORTA, 4, 5); NSS functionality not needed for ADC
    GPIO_SET_ALTERNATE_FUNCTION(PORTA, 5, 5);
    GPIO_SET_ALTERNATE_FUNCTION(PORTA, 6, 5);
    GPIO_SET_ALTERNATE_FUNCTION(PORTA, 7, 5);

    SPI.baud_rate_divisor(0); // this is actually a divisor of 2. Update the method so that the numbers match
    SPI.clock_polarity(1);
    SPI.clock_phase(1);
    SPI.frame_MSB_first();
    SPI.CRC_enabled(false);
    SPI.master_mode();
    SPI.data_size(14);
    SPI.SS_output_enable(true);
    SPI.FIFO_threshold_16bit();

    SPI.enable();
}

void SPI_transmit_byte(uint8_t byte)
{
    SPI.registers->data_reg = byte;
    SPI.enable();
}

int main(void)
{
    systick_init();
    setup_uart_pins();

    status = PURPLE;
    delay_ms(1000);

    uart.print_register("SPI control reg 1", SPI.registers->control_reg_1);
    uart.print_register("SPI control reg 2", SPI.registers->control_reg_2);
    uart.newline();

    SPI_setup(); 
    SPI.enable();

    uart.print_register("SPI control reg 1", SPI.registers->control_reg_1);
    uart.print_register("SPI control reg 2", SPI.registers->control_reg_2);
    uart.newline();

    uart.print_register("SPI status reg", SPI.registers->status_reg);
    uart.newline();

    GPIO_pin ADC_conv(PORTA, 4);
    ADC_conv.mode(OUTPUT);
    ADC_conv.type(PUSH_PULL);

    status = YELLOW;

    while (true)
    {
        //status = YELLOW;

        // ADC minimum conversion time is 225 ns so one or to nops should be sufficient delay
        // this is only a temporary solution
        // nevermind. GPIO function are very sluggish :(
        // this takes a whole 5 us. Needs optimising. inline?
        ADC_conv.set();
        ADC_conv.clear();

        SPI.registers->data_reg = 0x0000;
        //uart.print_register("SPI status reg", SPI.registers->status_reg);
        uint16_t data = SPI.registers->data_reg;
        uart.print_in_hex(data >> 8);
        uart.print_in_hex(data);
        //printf("%d\n", data);
        uart.newline();
        //uart.print_register("SPI status reg", SPI.registers->status_reg);
        //uart.newline();

        //delay_ms(200);
        //status = BLUE;
        //delay_ms(200);
    }
}