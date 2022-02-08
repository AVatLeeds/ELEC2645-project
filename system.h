#ifndef SYSTEM_H
#define SYSTEM_H

#include <stdint.h>

#define RCC_BASE        0x40021000

#define RCC_CR          *((uint32_t *)(RCC_BASE + 0x00))
#define RCC_ICSCR       *((uint32_t *)(RCC_BASE + 0x04))
#define RCC_CFGR        *((uint32_t *)(RCC_BASE + 0x08))
#define RCC_PLLCFGR     *((uint32_t *)(RCC_BASE + 0x0C))
#define RCC_PLLSAI1CFGR *((uint32_t *)(RCC_BASE + 0x10))
#define RCC_PLLSAI2CFGR *((uint32_t *)(RCC_BASE + 0x14))
#define RCC_CIER        *((uint32_t *)(RCC_BASE + 0x18))
#define RCC_CIFR        *((uint32_t *)(RCC_BASE + 0x1C))
#define RCC_CICR        *((uint32_t *)(RCC_BASE + 0x20))
#define RCC_AHB1RSTR    *((uint32_t *)(RCC_BASE + 0x28))
#define RCC_AHB2RSTR    *((uint32_t *)(RCC_BASE + 0x2C))
#define RCC_AHB3RSTR    *((uint32_t *)(RCC_BASE + 0x30))
#define RCC_APB1RSTR1   *((uint32_t *)(RCC_BASE + 0x38))
#define RCC_APB1RSTR2   *((uint32_t *)(RCC_BASE + 0x3C))
#define RCC_APB2RSTR    *((uint32_t *)(RCC_BASE + 0x40))
#define RCC_AHB1ENR     *((uint32_t *)(RCC_BASE + 0x48))
#define RCC_AHB2ENR     *((uint32_t *)(RCC_BASE + 0x4C))
#define RCC_APB3ENR     *((uint32_t *)(RCC_BASE + 0x50))
#define RCC_APB1ENR1    *((uint32_t *)(RCC_BASE + 0x58)) 
#define RCC_APB1ENR2    *((uint32_t *)(RCC_BASE + 0x5C))
#define RCC_APB2ENR     *((uint32_t *)(RCC_BASE + 0x60))
#define RCC_AHB1SMENR   *((uint32_t *)(RCC_BASE + 0x68))
#define RCC_AHB2SMENR   *((uint32_t *)(RCC_BASE + 0x6C))
#define RCC_AHB3SMENR   *((uint32_t *)(RCC_BASE + 0x70))
#define RCC_APB1SMENR1  *((uint32_t *)(RCC_BASE + 0x78))
#define RCC_APB1SMENR2  *((uint32_t *)(RCC_BASE + 0x7C))
#define RCC_APB2SMENR   *((uint32_t *)(RCC_BASE + 0x80))
#define RCC_CCIPR       *((uint32_t *)(RCC_BASE + 0x88))
#define RCC_BDCR        *((uint32_t *)(RCC_BASE + 0x90))
#define RCC_CSR         *((uint32_t *)(RCC_BASE + 0x94))
#define RCC_CRRCR       *((uint32_t *)(RCC_BASE + 0x98))
#define RCC_CCIPR2      *((uint32_t *)(RCC_BASE + 0x9C))

#define PLL_ENABLE()    RCC_CR |= (1U << 24)
#define PLL_STABLE      RCC_CR & (1U << 25)

//#define PLL_INPUT_DIVISOR
//#define PLL_INPUT_MULTIPLIER
//#define PLL_OUTPUT_DIVISOR_P
//#define PLL_OUTPUT_DIVISOR_Q
//#define PLL_OUTPUT_DIVISOR_R

//#define SYSCKL_SOURCE(SRC)

void sys_tick_handler(void);

uint32_t get_current_ticks(void);

#endif