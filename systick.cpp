#include <stdint.h>
#include "system.h"
#include "systick.h"

volatile uint32_t ticks = 0;
volatile uint32_t counter = 0;

void systick_init()
{
    SYST_CSR = 0;
    SYST_RVR = (SYST_CALIB & 0xFFFFFF);
    SYST_CVR = 0;
    SYST_CSR |= (1 << 2); // Set the systick clock source to the processor clock
    SYST_CSR |= (1 << 1); // Enable the systick interrupt
    SYST_CSR |= (1 << 0); // Enable the systick counter
}

void sys_tick_handler()
{
    ticks ++;
    counter --;
}

uint32_t get_current_ticks()
{
    return ticks;
}

void delay_ms(uint32_t ms)
{
    counter = ms;
    while (counter);
}