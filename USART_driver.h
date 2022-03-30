#ifndef USART_DRIVER_H
#define USART_DRIVER_H

#include <stdint.h>
#include "system.h"

/* for now I'm going to rely on the fact that the system clock is 4MHz on startup. This code should
be updated to use some sort of global clock rate perameter. Hopefully I will implement this later
and get rid of this comment */

#define USART_1 1
#define USART_2 2
#define USART_3 3
#define USART_4 4
#define USART_5 5

//#define USART_BASE(USART) ((USART) - 1) ? (0x40004000 + (0x400 * ((USART) - 1))) : 0x40013800

#define USART_BASE  0x40004400

#define USART_CR1   *((volatile uint32_t *)(USART_BASE + 0x00))
#define USART_CR2   *((volatile uint32_t *)(USART_BASE + 0x04))
#define USART_CR3   *((volatile uint32_t *)(USART_BASE + 0x08))
#define USART_BRR   *((volatile uint16_t *)(USART_BASE + 0x0C))
#define USART_GTPR  *((volatile uint16_t *)(USART_BASE + 0x10))
#define USART_ROTR  *((volatile uint16_t *)(USART_BASE + 0x14))
#define USART_RQR   *((volatile uint32_t *)(USART_BASE + 0x18))
#define USART_ISR   *((volatile uint32_t *)(USART_BASE + 0x1C))
#define USART_ICR   *((volatile uint32_t *)(USART_BASE + 0x20))
#define USART_RDR   *((volatile uint16_t *)(USART_BASE + 0x24))
#define USART_TDR   *((volatile uint16_t *)(USART_BASE + 0x28))

#define USART_WORDLENGTH7() USART_CR1 = ((USART_CR1 & ~(1U << 12)) | (1U << 28))
#define USART_WORDLENGTH8() USART_CR1 &= ~((1U <<12) | (1U << 28))
#define USART_WORDLENGTH9() USART_CR1 = ((USART_CR1 & ~(1U << 28)) | (1U << 12))

#define USART_OVERSAMP8()   USART_CR1 |= (1U << 15)
#define USART_OVERSAMP16()  USART_CR1 &= ~(1U << 15)

#define USART_SET_BAUD(BAUD)    USART_BRR = (4000000 / (BAUD))
#define USART_BAUD9600()        USART_BRR = 0x01A1
#define USART_BAUD19200()       USART_BRR = 0x00D0
#define USART_BAUD115200()      USART_BRR = 0x0023

#define USART_STOPBIT2()  USART_CR2 = ((USART_CR2 & ~(1U << 12)) | (1U << 13))
#define USART_STOPBIT1()  USART_CR2 = ((USART_CR2 & ~(1U << 12)) & ~(1U << 13))

#define USART_ENABLE()  USART_CR1 |= (1U << 0)

#define USART_TE()  USART_CR1 |= (1U << 3)

#endif