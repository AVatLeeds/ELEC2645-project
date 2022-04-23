#include "USART_driver.h"

USART::USART(uint32_t USART, uint32_t baud_rate)
{
    _USART = USART;
    USART_CLK_ENABLE(_USART);
    USART_CR1(_USART) = 0;
    USART_SET_CLK_SRC(_USART, SYSCLK);
    USART_SET_WORDLENGTH_8(_USART);
    USART_SET_BAUD(_USART, baud_rate);
    USART_SET_STOPBITS_2(_USART);
    USART_ENABLE(_USART);
}

void USART::set_usart(uint32_t USART)
{
    _USART = USART;
}

void USART::init(uint32_t baud_rate)
{
    USART_CLK_ENABLE(_USART);
    USART_CR1(_USART) = 0;
    USART_SET_CLK_SRC(_USART, SYSCLK);
    USART_SET_WORDLENGTH_8(_USART);
    USART_SET_BAUD(_USART, baud_rate);
    USART_SET_STOPBITS_2(_USART);
    USART_ENABLE(_USART);
}

void USART::transmit_byte(uint8_t byte)
{
    USART_TX_ENABLE(_USART);
    while (!USART_IS_TX_REG_EMPTY(_USART));
    USART_TDR(_USART) = byte;
    while (!USART_IS_TX_COMPLETE(_USART));
}

void USART::transmit_bytes(uint8_t * bytes, uint32_t num_bytes)
{
    uint32_t i;
    for (i = 0; i < num_bytes; i++)
    {
        transmit_byte(*bytes);
        bytes ++;
    } 
}

void USART::print_string(const char * string)
{
    USART_TX_ENABLE(_USART);
    while (*string != '\0')
    {
        while (!USART_IS_TX_REG_EMPTY(_USART));
        USART_TDR(_USART) = *string++;
    }
}

void USART::print_in_binary(uint8_t byte)
{
    uint8_t i;
    for (i = 0; i < 8; i ++)
    {
        transmit_byte((((byte << i) & 0x80) >> 7) + 48);
    }
}

void USART::print_in_hex(uint8_t byte)
{
    uint8_t lower_nibble = byte & 0x0F;
    uint8_t upper_nibble = (byte >> 4) & 0x0F;
    
    if (upper_nibble < 10)
    {
        transmit_byte(upper_nibble + 48);
    }
    else
    {
        transmit_byte(upper_nibble + 55); 
    }

    if (lower_nibble < 10)
    {
        transmit_byte(lower_nibble + 48);
    }
    else
    {
        transmit_byte(lower_nibble + 55); 
    }
}

void USART::print_register(const char * name, uint32_t reg_val)
{
    //UART->newline();
    print_string(name);
    print_string(": ");
    print_in_binary((reg_val) >> 24);
    print_in_binary((reg_val) >> 16);
    print_string(" ");
    print_in_binary((reg_val) >> 8);
    print_in_binary((reg_val) >> 0);
    newline();
}

void USART::newline()
{
    transmit_byte('\n');
    transmit_byte('\r');
}