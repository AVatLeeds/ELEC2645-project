/*
Author: Arthur Vie
Date: 05/02/2022

Adventures in low level embedded programming:

This is my first attempt at programming stm32 ARM Cortex based microcontrollers, spurred
on by the requirements of a university module. Over the past two weeks I have investigating
various methods of developing software for the stm32:

    - Embedded operating systems such as ARM's MBED - Huge bloat for simply toggling pins
        on and off.

    - Automated code generation using ST's CubeMX - I personally couldn't get CubeMX to run
        properly, never managed to generate any code and didn't want to waste the time to 
        fiddle with it until it did. Your mileage may vary but besides I wasn't so keen on
        generating loads of black-box boilerplate, or at least was keen to know what was inside.

    - Various IDEs (ST CubeIDE, Eclipse etc etc) - Nope, nope ,nope. I've always got on fine
        before with a text editor and command line compilation tools. Don't want to install
        multi gigabytes of IDE (I have my biases. If IDEs work for you, fantastic).

    - ARM CMSIS standard headers - I think this could be a viable option. nice macros that
        enable easy register level programming. I couldn't figure out how to integrate the
        CMSIS headers into my toolchain nicely so that I could include them as I might system
        libraries when programming for linux. Perhaps I didn't look hard enough but I couldn't
        find any approach that avoided manually including each header one by one somewhere, so
        I gave up.

At this point I was rather fed up with the opacity of the whole ecosystem. My background in
embedded programming is register level programming on AVR and PIC, so I was keen to be able to
do the same on stm32 ARM platforms. At this point I've decided to pull back the curtain, start
from the beginning and bring up all the necessary code from scratch.

I have always known in concept that it would be possible to write a C program that would assign
values to specific addresses (uint32_t * register = 0x00001234; *register = 42;) and if those
addresses happend to match the peripheral registers I would be able to manipulate the
peripherals. I had never actually done this, and in starting from scratch I have had to
investigate the rather foggy boundary between compiling C code and how and where it actually
runs on the target processor. For instance I have benefitted from the happy illusion that the
processor just magically starts execution from the main() function of my code.
From my previous work I've gathered a scattering of knowledge about what the C compiler produces
in the object file, the .text, .data, .bss etc... sections, but have never had to think about
how these are instantiated on the chip. This process is highly architecture dependent so
naturally there are plenty of mechanisms that politely hide this complexity from the system / 
application level developer, but it's time to break through the obscurantism and find out how
it's done.

This file is the startup program for my ST NUCLEO-L476RG development board which, by pulling
some neat tricks in collaboration with the compiler, is able to put an interrupt vector table
into memory exactly where I want it, move the .data and .bss sections of my compiled program
into their appropriate places in SRAM on the microcontroller and then call my main function.
If all goes well this file in combination with a linker script (more details on this later)
will be the bare minimum bring-up code to get arbitrary C (and hopefully C++) programs running
on the board. */

#include <stdint.h>
#include "system.h"

/* The following macros define the start, size and end of the microcontrolers memories
(stm32l476rg) as described in the reference manual (rm0351, section 2.2.2, Figure 3). */

#define FLASH_START 0x08000000U
#define FLASH_SIZE  0x100000U // 1M
#define FLASH_END   (FLASH_START + FLASH_END)

#define SRAM1_START 0x20000000U
#define SRAM1_SIZE  0x18000U // 96KB
#define SRAM1_END   (SRAM1_START + SRAM1_SIZE)

#define SRAM2_START 0x10000000U
#define SRAM2_SIZE  0x8000U // 32KB
#define SRAM2_END   (SRAM2_START + SRAM2_SIZE)

#define STACK_START SRAM1_END

/* The following are not defined in this file as indicated by extern. The linker will attempt
to match them up in the symbol table at link time. Since in the linker script I have added 
several symbols of the same name as these variable the linker will match them. Therefore, these
variables, following linking, are the bytes found at the respective addresses of the start of
text section, start of data section etc... In our case we don't care about the value of these
variables (the values will just be bytes of the program or bytes in RAM), rather we care about
their addresses */

extern uint32_t start_of_text_section; // relative to FLASH origin
extern uint32_t start_of_data_section; // relative to VMA (SRAM1 origin)
extern uint32_t start_of_bss_section;  // relative to VMA (SRAM1 origin)

extern uint32_t end_of_text_section;   // relative to FLASH origin
extern uint32_t end_of_data_section;   // relative to VMA (SRAM1 origin)
extern uint32_t end_of_bss_section;    // relative to VMA (SRAM1 origin)

extern void (* start_of_init_array)();
extern void (* end_of_init_array)();

extern int main(void);

/* All of the interrupt handler functions apart from the reset handler have a weak alias 
to the function "default_handle".
"__attrubute__" is a directive used to tell the compiler to treat an object in a particlar
way. In this case the alias attribute tells the compiler to replace references to the function
with references to "default_handler" instead. The weak attribute tells the compiler to only
acknowledge this alias if the function it is attached to is not defined anywhere elsewhere
in the code. This allows the developer to write their own implementation of each of these
interrupt handler functions which will override the default handler. The default handler
will only be used if no other implementation is found by the compiler. */

// reset interrupt handler
extern "C"
{
    void reset_handler(void);
}

/* WARNING! EXTREME CRUSTY. Default handler prototype has to be in an extern "C" block so that
the c++ compiler will allow it to be linked in the ordinary C way rather than the C++ way. This
is required in order for aliasing to be possible. Otherwise the c++ compiler mangles the symbol
name at compile time, meaning the name can no longer be known a-priori and aliasing is no longer
possible. 
    As far as I understand, the reason the c++ compiler does this is to allow for overloading of
the function but more reading about the c++ compiler and mangled names is required. */

extern "C"
{
    void default_handler(void);
}

// low level interrupt handlers
void non_maskable_interrupt_handler(void)   __attribute__((weak, alias("default_handler")));
void hard_fault_interrupt_handler(void)     __attribute__((weak, alias("default_handler")));
void memory_manager_interrupt_handler(void) __attribute__((weak, alias("default_handler")));
void bus_fault(void)                        __attribute__((weak, alias("default_handler")));
void usage_fault(void)                      __attribute__((weak, alias("default_handler")));
void SV_call(void)                          __attribute__((weak, alias("default_handler")));
void debug(void)                            __attribute__((weak, alias("default_handler")));
void pend_SV(void)                          __attribute__((weak, alias("default_handler")));
void sys_tick_handler(void)                 __attribute__((weak, alias("default_handler")));

// interrupt service routines
void ISR_window_watchdog(void)             __attribute__((weak, alias("default_handler")));
void ISR_PVD_PVM(void)                     __attribute__((weak, alias("default_handler")));
void ISR_realtime_clock_TAMP_STAMP(void)   __attribute__((weak, alias("default_handler")));
void ISR_realtime_clock_wakeup(void)       __attribute__((weak, alias("default_handler")));
void ISR_flash_global(void)                __attribute__((weak, alias("default_handler")));
void ISR_RCC_global(void)                  __attribute__((weak, alias("default_handler")));
void ISR_EXT_line0(void)                   __attribute__((weak, alias("default_handler")));
void ISR_EXT_line1(void)                   __attribute__((weak, alias("default_handler")));
void ISR_EXT_line2(void)                   __attribute__((weak, alias("default_handler")));
void ISR_EXT_line3(void)                   __attribute__((weak, alias("default_handler")));
void ISR_EXT_line4(void)                   __attribute__((weak, alias("default_handler")));
void ISR_DMA1_channel1(void)               __attribute__((weak, alias("default_handler")));
void ISR_DMA1_channel2(void)               __attribute__((weak, alias("default_handler")));
void ISR_DMA1_channel3(void)               __attribute__((weak, alias("default_handler")));
void ISR_DMA1_channel4(void)               __attribute__((weak, alias("default_handler")));
void ISR_DMA1_channel5(void)               __attribute__((weak, alias("default_handler")));
void ISR_DMA1_channel6(void)               __attribute__((weak, alias("default_handler")));
void ISR_DMA1_channel7(void)               __attribute__((weak, alias("default_handler")));
void ISR_ADC_1_and_2_global(void)          __attribute__((weak, alias("default_handler")));
void ISR_CAN1_transmit(void)               __attribute__((weak, alias("default_handler")));
void ISR_CAN1_receive0(void)               __attribute__((weak, alias("default_handler")));
void ISR_CAN1_receive1(void)               __attribute__((weak, alias("default_handler")));
void ISR_CAN1_SCE(void)                    __attribute__((weak, alias("default_handler")));
void ISR_EXT_lines_9_to_5(void)            __attribute__((weak, alias("default_handler")));
void ISR_timer15_global(void)              __attribute__((weak, alias("default_handler")));
void ISR_timer16_global(void)              __attribute__((weak, alias("default_handler")));
void ISR_timer17_global(void)              __attribute__((weak, alias("default_handler")));
void ISR_timer1_capture_compare(void)      __attribute__((weak, alias("default_handler")));
void ISR_timer2_global(void)               __attribute__((weak, alias("default_handler")));
void ISR_timer3_global(void)               __attribute__((weak, alias("default_handler")));
void ISR_timer4_global(void)               __attribute__((weak, alias("default_handler")));
void ISR_I2C1_event(void)                  __attribute__((weak, alias("default_handler")));
void ISR_I2C1_error(void)                  __attribute__((weak, alias("default_handler")));
void ISR_I2C2_event(void)                  __attribute__((weak, alias("default_handler")));
void ISR_I2C2_error(void)                  __attribute__((weak, alias("default_handler")));
void ISR_SPI1_global(void)                 __attribute__((weak, alias("default_handler")));
void ISR_SPI2_global(void)                 __attribute__((weak, alias("default_handler")));
void ISR_USART1_global(void)               __attribute__((weak, alias("default_handler")));
void ISR_USART2_global(void)               __attribute__((weak, alias("default_handler")));
void ISR_USART3_global(void)               __attribute__((weak, alias("default_handler")));
void ISR_EXT_lines_15_to_10(void)          __attribute__((weak, alias("default_handler")));
void ISR_realtime_clock_alarm(void)        __attribute__((weak, alias("default_handler")));
void ISR_DFSDM1_PLT3(void)                 __attribute__((weak, alias("default_handler")));
void ISR_timer8_break(void)                __attribute__((weak, alias("default_handler")));
void ISR_timer8_update(void)               __attribute__((weak, alias("default_handler")));
void ISR_timer8_trigger(void)              __attribute__((weak, alias("default_handler")));
void ISR_timer8_capture_compare(void)      __attribute__((weak, alias("default_handler")));
void ISR_ADC3_global(void)                 __attribute__((weak, alias("default_handler")));
void ISR_FMC(void)                         __attribute__((weak, alias("default_handler")));
void ISR_SDMMC1(void)                      __attribute__((weak, alias("default_handler")));
void ISR_timer5_global(void)               __attribute__((weak, alias("default_handler")));
void ISR_SPI3_global(void)                 __attribute__((weak, alias("default_handler")));
void ISR_UART4_global(void)                __attribute__((weak, alias("default_handler")));
void ISR_UART5_global(void)                __attribute__((weak, alias("default_handler")));
void ISR_timer6_global(void)               __attribute__((weak, alias("default_handler"))); // and DAC1 underrun ???  
void ISR_timer7_global(void)               __attribute__((weak, alias("default_handler")));
void ISR_DMA2_channel1(void)               __attribute__((weak, alias("default_handler")));
void ISR_DMA2_channel2(void)               __attribute__((weak, alias("default_handler")));
void ISR_DMA2_channel3(void)               __attribute__((weak, alias("default_handler")));
void ISR_DMA2_channel4(void)               __attribute__((weak, alias("default_handler")));
void ISR_DMA2_channel5(void)               __attribute__((weak, alias("default_handler")));
void ISR_DFSDM1_FLT0(void)                 __attribute__((weak, alias("default_handler")));
void ISR_DFSDM1_FLT1(void)                 __attribute__((weak, alias("default_handler")));
void ISR_DFSDM1_FLT2(void)                 __attribute__((weak, alias("default_handler")));
void ISR_COMP(void)                        __attribute__((weak, alias("default_handler")));
void ISR_LPTIM1(void)                      __attribute__((weak, alias("default_handler")));
void ISR_LPTIM2(void)                      __attribute__((weak, alias("default_handler")));
void ISR_OTG_FS(void)                      __attribute__((weak, alias("default_handler")));
void ISR_DMA2_channel6(void)               __attribute__((weak, alias("default_handler")));
void ISR_DMA2_channel7(void)               __attribute__((weak, alias("default_handler")));
void ISR_LPUART1(void)                     __attribute__((weak, alias("default_handler")));
void ISR_QUADSPI1(void)                    __attribute__((weak, alias("default_handler")));
void ISR_I2C3_event(void)                  __attribute__((weak, alias("default_handler")));
void ISR_I2C3_error(void)                  __attribute__((weak, alias("default_handler")));
void ISR_SAI1(void)                        __attribute__((weak, alias("default_handler")));
void ISR_SAI2(void)                        __attribute__((weak, alias("default_handler")));
void ISR_SWPMI1(void)                      __attribute__((weak, alias("default_handler")));
void ISR_TSC(void)                         __attribute__((weak, alias("default_handler")));
void ISR_LCD_global(void)                  __attribute__((weak, alias("default_handler")));
void ISR_FPU(void)                         __attribute__((weak, alias("default_handler")));

/* The vector table is an array of 32-bit unsigned integers (words) which are the addresses
of the function to be run when that particular interrupt occurs. Most of the entries are the
addresses of interrupt handler functions prototyped above, cast to unsigned 32-bit integers.
Any zeros in the table are spaces that are reserved according to the reference manual (rm0351,
section 13.3, table 58). 
 
The section attribute instructs the compiler to put the "vector_table" object int a specific
section in the output object file called ".vector_table". as far as I know the dot at the 
beginning of the section name is not actually required, but rather a convention. (The linker
script can be used to tell the linker where to place this section withing the final binary). */

void (* vector_table[])() __attribute__((section(".vector_table"))) = {
    (void (*)())STACK_START,
    reset_handler,
    non_maskable_interrupt_handler,
    hard_fault_interrupt_handler,
    memory_manager_interrupt_handler,
    bus_fault,
    usage_fault,
    0,
    0,
    0,
    0,
    SV_call,
    debug,
    0,
    pend_SV,
    sys_tick_handler,
    ISR_window_watchdog,
    ISR_PVD_PVM,
    ISR_realtime_clock_TAMP_STAMP,
    ISR_realtime_clock_wakeup,
    ISR_flash_global,
    ISR_RCC_global,
    ISR_EXT_line0,
    ISR_EXT_line1,
    ISR_EXT_line2,
    ISR_EXT_line3,
    ISR_EXT_line4,
    ISR_DMA1_channel1,
    ISR_DMA1_channel2,
    ISR_DMA1_channel3,
    ISR_DMA1_channel4,
    ISR_DMA1_channel5,
    ISR_DMA1_channel6,
    ISR_DMA1_channel7,
    ISR_ADC_1_and_2_global,
    ISR_CAN1_transmit,
    ISR_CAN1_receive0,
    ISR_CAN1_receive1,
    ISR_CAN1_SCE,
    ISR_EXT_lines_9_to_5,
    ISR_timer15_global,
    ISR_timer16_global,
    ISR_timer17_global,
    ISR_timer1_capture_compare,
    ISR_timer2_global,
    ISR_timer3_global,
    ISR_timer4_global,
    ISR_I2C1_event,
    ISR_I2C1_error,
    ISR_I2C2_event,
    ISR_I2C2_error,
    ISR_SPI1_global,
    ISR_SPI2_global,
    ISR_USART1_global,
    ISR_USART2_global,
    ISR_USART3_global,
    ISR_EXT_lines_15_to_10,
    ISR_realtime_clock_alarm,
    ISR_DFSDM1_PLT3,
    ISR_timer8_break,
    ISR_timer8_update,
    ISR_timer8_trigger,
    ISR_timer8_capture_compare,
    ISR_ADC3_global,
    ISR_FMC,
    ISR_SDMMC1,
    ISR_timer5_global,
    ISR_SPI3_global,
    ISR_UART4_global,
    ISR_UART5_global,
    ISR_timer6_global,
    ISR_timer7_global,
    ISR_DMA2_channel1,
    ISR_DMA2_channel2,
    ISR_DMA2_channel3,
    ISR_DMA2_channel4,
    ISR_DMA2_channel5,
    ISR_DFSDM1_FLT0,
    ISR_DFSDM1_FLT1,
    ISR_DFSDM1_FLT2,
    ISR_COMP,
    ISR_LPTIM1,
    ISR_LPTIM2,
    ISR_OTG_FS,
    ISR_DMA2_channel6,
    ISR_DMA2_channel7,
    ISR_LPUART1,
    ISR_QUADSPI1,
    ISR_I2C3_event,
    ISR_I2C3_error,
    ISR_SAI1,
    ISR_SAI2,
    ISR_SWPMI1,
    ISR_TSC,
    ISR_LCD_global,
    ISR_FPU
};

void default_handler (void)
{
    while (1);
}

void reset_handler(void)
{
    /* The linker script instructs the linker to put the ".vector_table" section at the
    beginning of the compiled binary. Since this section contains the vector table array this
    is the first thing that will appear in the code memory. The system reset interrupt loads
    the program counter with the address of the second entry in the interrupt vector table
    and in turn loads from there the address of this function, the reset handler. So this
    function is the first that will be run at processor startup. Therefore, it's responsibility
    is to move all of the initalized data into RAM and to allocate enough space in RAM for the
    zero initialized data (bss). After these tasks and any other setup main is called. */

    uint32_t i;
    uint8_t * flash_pointer;
    uint8_t * sram_pointer;

    flash_pointer = (uint8_t *)&end_of_text_section;
    sram_pointer = (uint8_t *)&start_of_data_section;
    uint32_t data_section_size = &end_of_data_section - &start_of_data_section; // size of the data section in bytes

    // populate SRAM1 with the data section from FLASH from data start to data end
    for (i = 0; i < data_section_size; i++)
    {
        *sram_pointer = *flash_pointer;
        sram_pointer++;
        flash_pointer++;
    }

    sram_pointer = (uint8_t *)&start_of_bss_section;
    uint32_t bss_section_size = &end_of_bss_section - &start_of_bss_section; // size of the bss section in bytes

    // fill the SRAM1 with zeroes from bss start to bss end
    for (i = 0; i < bss_section_size; i++)
    {
        *sram_pointer = 0;
        sram_pointer++;
    }

    // iterate over the list of pointers to constructors each time dereferencing the constructor pointer and executing it
    void (** constructor_ptr)();
    for (constructor_ptr = &start_of_init_array; constructor_ptr < &end_of_init_array; constructor_ptr ++)
    {
        (*constructor_ptr)();
    }

    // enable floating point coprocessor
    CPACR |= (0b1111 << 20);

    main();
}