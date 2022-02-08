#include <stdint.h>
#include "system.h"

volatile uint32_t ticks = 0;

void sys_tick_handler(void)
{
    ticks ++;
}

uint32_t get_current_ticks(void)
{
    return ticks;
}
