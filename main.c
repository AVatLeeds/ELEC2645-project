#include <stdint.h>
#include <stdio.h>
#include <math.h>

#define FLASH_ORIGIN    0x08000000

#define RCC_BASE        0x40021000

#define RCC_APB2ENR_OFFSET  0x4C

#define PA_BASE   0x48000000
#define PB_BASE   0x48000400
#define PC_BASE   0x48000800
#define PD_BASE   0x48000C00
#define PE_BASE   0x48001000
#define PF_BASE   0x48001400
#define PG_BASE   0x48001800
#define PH_BASE   0x48001C00

#define GPIO_MODE_OFFSET    0x00
#define GPIO_TYPE_OFFSET    0x04
#define GPIO_SPEED_OFFSET   0x08
#define GPIO_PUPD_OFFSET    0x0C
#define GPIO_IDR_OFFSET     0x10
#define GPIO_ODR_OFFSET     0x14
#define GPIO_BSR_OFFSET     0x18
#define GPIO_LOCK_OFFEST    0x1C
#define GPIO_AFRL_OFFSET    0x20
#define GPIO_AFRH_OFFSET    0x24
#define GPIO_RESET_OFFSET   0x28
#define GPIO_ASC_OFFSET     0x2C

int my_global = 42;
int global_zero = 0;
int global_uninitialized;
char * my_global_string = "Hello world!";

int main(void)
{
    volatile uint8_t * origin = 0;

    *(origin + RCC_BASE + RCC_APB2ENR_OFFSET + 0) = 0b00000100;        
    *(origin + RCC_BASE + RCC_APB2ENR_OFFSET + 1) = 0b00000000;    
    *(origin + RCC_BASE + RCC_APB2ENR_OFFSET + 2) = 0b00000000;    
    *(origin + RCC_BASE + RCC_APB2ENR_OFFSET + 3) = 0b00000000;        

    *(origin + PC_BASE + GPIO_MODE_OFFSET + 0) = 0b01010101;
    *(origin + PC_BASE + GPIO_MODE_OFFSET + 1) = 0b01010101;
    *(origin + PC_BASE + GPIO_MODE_OFFSET + 2) = 0b01010101;
    *(origin + PC_BASE + GPIO_MODE_OFFSET + 3) = 0b01010101;

    //(*(volatile uint8_t *)(PC_OFFSET + GPIO_MODE_OFFSET)) = 

    while (1)
    {
        *(origin + PC_BASE + GPIO_ODR_OFFSET + 0) = 0b11111111;
        *(origin + PC_BASE + GPIO_ODR_OFFSET + 1) = 0b11111111;
        *(origin + PC_BASE + GPIO_ODR_OFFSET + 2) = 0b11111111;
        *(origin + PC_BASE + GPIO_ODR_OFFSET + 3) = 0b11111111;

        *(origin + PC_BASE + GPIO_ODR_OFFSET + 0) = 0b00000000;
        *(origin + PC_BASE + GPIO_ODR_OFFSET + 1) = 0b00000000;
        *(origin + PC_BASE + GPIO_ODR_OFFSET + 2) = 0b00000000;
        *(origin + PC_BASE + GPIO_ODR_OFFSET + 3) = 0b00000000;
    }
    
    return 0;
}