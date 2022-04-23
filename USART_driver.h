#ifndef USART_DRIVER_H
#define USART_DRIVER_H

#include <stdint.h>
#include "system.h"

/* for now I'm going to rely on the fact that the system clock is 4MHz on startup. This code should
be updated to use some sort of global clock rate perameter. Hopefully I will implement this later
and get rid of this comment */

#define USART_1 0
#define USART_2 1
#define USART_3 2
#define USART_4 3
#define USART_5 4

#define USART_BASE(USART) ((USART) ? (0x40004000 + (0x400 * USART)) : 0x40013800)

#define USART_CR1(USART)   *((volatile uint32_t *)(USART_BASE((USART)) + 0x00))
#define USART_CR2(USART)   *((volatile uint32_t *)(USART_BASE((USART)) + 0x04))
#define USART_CR3(USART)   *((volatile uint32_t *)(USART_BASE((USART)) + 0x08))
#define USART_BRR(USART)   *((volatile uint16_t *)(USART_BASE((USART)) + 0x0C))
#define USART_GTPR(USART)  *((volatile uint16_t *)(USART_BASE((USART)) + 0x10))
#define USART_RTOR(USART)  *((volatile uint16_t *)(USART_BASE((USART)) + 0x14))
#define USART_RQR(USART)   *((volatile uint32_t *)(USART_BASE((USART)) + 0x18))
#define USART_ISR(USART)   *((volatile uint32_t *)(USART_BASE((USART)) + 0x1C))
#define USART_ICR(USART)   *((volatile uint32_t *)(USART_BASE((USART)) + 0x20))
#define USART_RDR(USART)   *((volatile uint16_t *)(USART_BASE((USART)) + 0x24))
#define USART_TDR(USART)   *((volatile uint16_t *)(USART_BASE((USART)) + 0x28))     

#define USART_SET_WORDLENGTH_7(USART) USART_CR1(USART) = ((USART_CR1(USART) & ~(1U << 12)) | (1U << 28))
#define USART_SET_WORDLENGTH_8(USART) USART_CR1(USART) &= ~((1U << 12) | (1U << 28))
#define USART_SET_WORDLENGTH_9(USART) USART_CR1(USART) = ((USART_CR1(USART) & ~(1U << 28)) | (1U << 12))

#define USART_SET_OVERSAMP_8(USART)   USART_CR1(USART) |= (1U << 15)
#define USART_SET_OVERSAMP_16(USART)  USART_CR1(USART) &= ~(1U << 15)

#define USART_SET_BAUD(USART, BAUD)     USART_BRR(USART) = ((USART_CR1(USART) >> 15) & 0b1) ? ((4000000 * 2) / (BAUD)) : (4000000 / (BAUD)) // Different behaviour depending on oversampling setting
#define USART_SET_BAUD9600(USART)       USART_BRR(USART) = 0x01A1
#define USART_SET_BAUD19200(USART)      USART_BRR(USART) = 0x00D0
#define USART_SET_BAUD115200(USART)     USART_BRR(USART) = 0x0023

#define USART_SET_STOPBITS_2(USART)  USART_CR2(USART) = ((USART_CR2(USART) & ~(0b11 << 12)) | (0b1 << 13))
#define USART_SET_STOPBITS_1(USART)  USART_CR2(USART) &= ~(0b11 << 12)

#define USART_CLK_ENABLE(USART)     ((USART) ? (RCC_APB1ENR1 |= (0b1 << (USART + 16))) : (RCC_APB2ENR |= (0b1 << 14)))
#define USART_CLK_DISABLE(USART)    ((USART) ? (RCC_APB1ENR1 &= (0b1 << ~(USART + 16))) : (RCC_APB2ENR &= ~(0b1 << 14)))

#define PCLK    0b00
#define SYSCLK  0b01
#define HSI16   0b10
#define LSE     0b11

#define USART_SET_CLK_SRC(USART, SRC)   RCC_CCIPR = (RCC_CCIPR & ~(0b11 << (USART * 2))) | (SRC << (USART * 2))

#define USART_ENABLE(USART)  USART_CR1(USART) |= (0b1 << 0)
#define USART_DISABLE(USART)  USART_CR1(USART) &= ~(0b1 << 0)

#define USART_TX_ENABLE(USART)  USART_CR1(USART) |= (0b1 << 3)
#define USART_TX_DISABLE(USART) USART_CR1(USART) &= ~(0b1 << 3)

#define USART_RX_ENABLE(USART)  USART_CR1(USART) |= (0b1 << 2)
#define USART_RX_DISABLE(USART) USART_CR1(USART) &= ~(0b1 << 2)

#define USART_IS_TX_REG_EMPTY(USART)    ((USART_ISR(USART) >> 7) & 0b1)
#define USART_IS_TX_COMPLETE(USART)     ((USART_ISR(USART) >> 6) & 0b1)

class USART_driver
{
    public:
    USART_driver(uint32_t USART, uint32_t baud_rate);
    void set_usart(uint32_t USART);
    void init(uint32_t baud_rate);
    void transmit_byte(uint8_t byte);
    void transmit_bytes(uint8_t * bytes, uint32_t num_bytes);
    void print_string(const char * string);
    void print_in_binary(uint8_t byte);
    void print_in_hex(uint8_t byte);
    void print_register(const char * name, uint32_t reg_val);
    void newline(void);
    

    private:
    uint32_t _USART;
};

#endif