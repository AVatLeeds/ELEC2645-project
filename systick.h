#ifndef SYSTICK_H
#define SYSTICK_H

#include <stdint.h>

#define SYST_BASE   0xE000E010

#define SYST_CSR   *((volatile uint32_t *)(SYST_BASE + 0x0))
#define SYST_RVR   *((volatile uint32_t *)(SYST_BASE + 0x4))
#define SYST_CVR   *((volatile uint32_t *)(SYST_BASE + 0x8))
#define SYST_CALIB *((volatile uint32_t *)(SYST_BASE + 0xC))

void systick_init();
uint32_t get_ticks();
void delay_ms(uint32_t ms);

#endif