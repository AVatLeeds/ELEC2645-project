#include <stdint.h>
#include "bit_banged_UART.h"

bit_banged_UART::bit_banged_UART(uint32_t baud_rate, void (* set_tx)(void), void (* clear_tx)())
{
    _bit_delay = ((3958815 - (28 * baud_rate)) / (5 * baud_rate));
    _set_tx = set_tx;
    _clear_tx = clear_tx;
    _set_tx();
}

void bit_banged_UART::tx_state(uint8_t state)
{
    if (state)
    {
        _set_tx();
    }
    else
    {
        _clear_tx();
    }
}

inline void __attribute__((optimize("O3"))) bit_banged_UART::delay(uint32_t count)
{
    while (count)
    {
        count --;
        __asm__("nop");
    }
}

void bit_banged_UART::transmit_byte(uint8_t byte)
{
    uint8_t i;

    _clear_tx();
    delay(_bit_delay);
    for (i = 0; i < 8; i ++)
    {
        tx_state(byte & 0b00000001);
        byte >>= 1;
        delay(_bit_delay);
    }
    _set_tx();
    delay(_bit_delay);
}

void bit_banged_UART::print_in_binary(uint8_t byte)
{
    uint8_t i;
    for (i = 0; i < 8; i ++)
    {
        transmit_byte((((byte << i) & 0x80) >> 7) + 48);
    }
}

void bit_banged_UART::print_in_hex(uint8_t byte)
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

void bit_banged_UART::transmit_string(char * string)
{
    while (*string != '\0')
    {
        transmit_byte(*string);
        string ++;
    }
}

void bit_banged_UART::transmit_bytes(uint8_t * bytes, uint32_t num_bytes)
{
    uint32_t i;
    for (i = 0; i < num_bytes; i++)
    {
        transmit_byte(*bytes);
        bytes ++;
    } 
}

void bit_banged_UART::newline()
{
    transmit_byte('\n');
    transmit_byte('\r');
}

void bit_banged_UART::set_tx_handlers(void (*set_tx)(), void (*clear_tx)())
{
    _set_tx = set_tx;
    _clear_tx = clear_tx;
}

void bit_banged_UART::set_baud_rate(uint32_t baud_rate)
{
    _bit_delay = ((3958815 - (28 * baud_rate)) / (5 * baud_rate));
}