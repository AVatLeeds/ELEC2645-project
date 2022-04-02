#include "USART_driver.h"

USART::USART(uint32_t USART)
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

void USART::transmit_string(const char * string)
{
    USART_TX_ENABLE(_USART);
    while (*string != '\0')
    {
        while (!USART_IS_TX_REG_EMPTY(_USART));
        USART_TDR(_USART) = *string++;
    }
}