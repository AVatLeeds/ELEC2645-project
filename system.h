#ifndef SYSTEM_H
#define SYSTEM_H

#include <stdint.h>

#define RCC_BASE        0x40021000

#define RCC_CR          *((volatile uint32_t *)(RCC_BASE + 0x00))
#define RCC_ICSCR       *((volatile uint32_t *)(RCC_BASE + 0x04))
#define RCC_CFGR        *((volatile uint32_t *)(RCC_BASE + 0x08))
#define RCC_PLLCFGR     *((volatile uint32_t *)(RCC_BASE + 0x0C))
#define RCC_PLLSAI1CFGR *((volatile uint32_t *)(RCC_BASE + 0x10))
#define RCC_PLLSAI2CFGR *((volatile uint32_t *)(RCC_BASE + 0x14))
#define RCC_CIER        *((volatile uint32_t *)(RCC_BASE + 0x18))
#define RCC_CIFR        *((volatile uint32_t *)(RCC_BASE + 0x1C))
#define RCC_CICR        *((volatile uint32_t *)(RCC_BASE + 0x20))
#define RCC_AHB1RSTR    *((volatile uint32_t *)(RCC_BASE + 0x28))
#define RCC_AHB2RSTR    *((volatile uint32_t *)(RCC_BASE + 0x2C))
#define RCC_AHB3RSTR    *((volatile uint32_t *)(RCC_BASE + 0x30))
#define RCC_APB1RSTR1   *((volatile uint32_t *)(RCC_BASE + 0x38))
#define RCC_APB1RSTR2   *((volatile uint32_t *)(RCC_BASE + 0x3C))
#define RCC_APB2RSTR    *((volatile uint32_t *)(RCC_BASE + 0x40))
#define RCC_AHB1ENR     *((volatile uint32_t *)(RCC_BASE + 0x48))
#define RCC_AHB2ENR     *((volatile uint32_t *)(RCC_BASE + 0x4C))
#define RCC_APB3ENR     *((volatile uint32_t *)(RCC_BASE + 0x50))
#define RCC_APB1ENR1    *((volatile uint32_t *)(RCC_BASE + 0x58)) 
#define RCC_APB1ENR2    *((volatile uint32_t *)(RCC_BASE + 0x5C))
#define RCC_APB2ENR     *((volatile uint32_t *)(RCC_BASE + 0x60))
#define RCC_AHB1SMENR   *((volatile uint32_t *)(RCC_BASE + 0x68))
#define RCC_AHB2SMENR   *((volatile uint32_t *)(RCC_BASE + 0x6C))
#define RCC_AHB3SMENR   *((volatile uint32_t *)(RCC_BASE + 0x70))
#define RCC_APB1SMENR1  *((volatile uint32_t *)(RCC_BASE + 0x78))
#define RCC_APB1SMENR2  *((volatile uint32_t *)(RCC_BASE + 0x7C))
#define RCC_APB2SMENR   *((volatile uint32_t *)(RCC_BASE + 0x80))
#define RCC_CCIPR       *((volatile uint32_t *)(RCC_BASE + 0x88))
#define RCC_BDCR        *((volatile uint32_t *)(RCC_BASE + 0x90))
#define RCC_CSR         *((volatile uint32_t *)(RCC_BASE + 0x94))
#define RCC_CRRCR       *((volatile uint32_t *)(RCC_BASE + 0x98))
#define RCC_CCIPR2      *((volatile uint32_t *)(RCC_BASE + 0x9C))

#define PLL_ENABLE()    RCC_CR |= (1U << 24)
#define PLL_STABLE      RCC_CR & (1U << 25)

// Nested Vectored Interrupt Controller

#define NVIC_BASE 0xE000E000

#define NVIC_ISER(IDX)  *((volatile uint32_t *)(NVIC_BASE + 0x100 + (0x04 * IDX)))
#define NVIC_ICER(IDX)  *((volatile uint32_t *)(NVIC_BASE + 0x180 + (0x04 * IDX)))
#define NVIC_ISPR(IDX)  *((volatile uint32_t *)(NVIC_BASE + 0x200 + (0x04 * IDX)))
#define NVIC_ICPR(IDX)  *((volatile uint32_t *)(NVIC_BASE + 0x280 + (0x04 * IDX)))
#define NVIC_IABR(IDX)  *((volatile uint32_t *)(NVIC_BASE + 0x300 + (0x04 * IDX)))
#define NVIC_IPR(IDX)   *((volatile uint32_t *)(NVIC_BASE + 0x400 + (0x04 * IDX)))

// implementing enable and disable for now. More to do.
#define NVIC_ENABLE_INTERRUPT(IDX)      (NVIC_ISER((IDX >> 5U) & 0b111) |= (1U << (IDX & 0b11111)))
#define NVIC_DISABLE_INTERRUPT(IDX)     (NVIC_ICER((IDX >> 5U) & 0b111) |= (1U << (IDX & 0b11111)))

//#define PLL_INPUT_DIVISOR
//#define PLL_INPUT_MULTIPLIER
//#define PLL_OUTPUT_DIVISOR_P
//#define PLL_OUTPUT_DIVISOR_Q
//#define PLL_OUTPUT_DIVISOR_R

//#define SYSCKL_SOURCE(SRC)

/* Clock setup */

/* From the datasheet - The MSI clock is used as system clock after restart from Reset, wakeup from Standby and
Shutdown low-power modes. After restart from Reset, the MSI frequency is set to its default
value 4 MHz. */

#endif