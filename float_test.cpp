#include "GPIO_driver.h"

GPIO_pin time(PORTA, 1);

int main() __attribute__((optimize("-O0")));
int main()
{
    time.mode(OUTPUT);
    float a=1.0098;
    float b=1.9987;
    
    GPIO_ODR(PORTA) = 2;
    float c; c=a*b;
    GPIO_ODR(PORTA) = 0;

    while (true);

    return 0;
}