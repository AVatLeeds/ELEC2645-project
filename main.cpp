#include <stdint.h>

#include "GPIO_driver.h"
#include "system.h"

int my_global = 42;
int global_zero = 0;
int global_uninitialized;
char * my_global_string = "Hello world!";

uint32_t counter = 0;
uint32_t init = 0;

int main(void)
{
    *((uint32_t *)(0xE000E010 + 0)) |= 3;
    GPIO_ENABLE(PORTC);
    GPIO_MODE(PORTC) = 0x5555U;

    while (1)
    {
        sys_tick_handler();

        GPIO_ODR(PORTC) = 0xFFFFU;

        init = get_current_ticks();
        while (init)
        {
            init --;
        }

        GPIO_ODR(PORTC) = 0x0000U;

        init = get_current_ticks();
        while (init)
        {
            init --;
        }

    }
    
    return 0;
}