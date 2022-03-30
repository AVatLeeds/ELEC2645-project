#ifndef BIT_BANGED_UART_H
#define BIT_BANGED_UART_H

#include <stdint.h>

class bit_banged_UART
{
    public:
    bit_banged_UART(void);
    bit_banged_UART(uint32_t baud_rate, void (* set_tx)(void), void (* clear_tx)());

    void transmit_byte(uint8_t byte);
    void print_in_binary(uint8_t byte);
    void print_in_hex(uint8_t byte);
    void transmit_string(char * string);
    void transmit_bytes(uint8_t * bytes, uint32_t num_bytes);
    void newline(void);

    private:
    uint32_t _bit_delay;
    void (* _set_tx)(void);
    void (* _clear_tx)(void);
    void tx_state(uint8_t state);
    void delay(uint32_t count);
};

#endif