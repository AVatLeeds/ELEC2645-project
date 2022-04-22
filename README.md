# ELEC2645-project

## Introduction

I have been working on the content of this repository since the beginning of this semester (January 2022), however, only as of today (April 9th 2022) have I uploaded the content publicly.

Therefore, any commits before this date represent my various experiments with the ST Nucleo L476RG development board and are intended to serve as the basis for my ELEC2645 module project, the code for which will be uploaded here shortly.

First, some of what I have learned from my experiments.

### Look mum! No operating system:

For a long while I've been very interested in exactly how programs are loaded and executed on a processor outside of an operating system. I know for instance that operating systems like GNU/Linux or Microsoft Windows provide a set of system calls which can be accessed by the C / C++ standard library to perform tasks like memory allocation or reading and writing of input / output streams, and that the operating system handles the setting up of stack and heap memory for the program to use when it is run as a process, and is responsible for moving all of the data in the executable file into the correct places in the memory for the process to begin. However on a bare microprocessor none of these facilities are available.

I have a fair bit of past experience writing embedded software in C (never C++ before attending university) targeted at Atmel and PIC microcontrollers. However, in such cases the development environment (AVR-GCC and associated tools being what I am most familiar with) has provided a convenient hardware abstraction layer in the form of preprocessor macros for accessing the registers and has obscured the mechanisms by which the program is loaded on the processor and the "main()" function called.

I have no prior familiarity with the ARM based stm32 microprocessors so I chose this as an oportunity to investigate developing my software "from scratch" and implementing all of the necessary procedures to get my program to run.

#### Register access.

I've often thought that, in an embedded setting, in C (or C++) it should be possible to access a register by creating a pointer to a data type with the same width as the register, setting the value of that pointer to the registers memory address and then dereferencing the pointer to either assign or read it's value. Of course pulling this type of trick while running in an operating system is most likely to result in an immediate segmentation fault at least.

It turns out though that without an operating system this trick is exactly possible. The following examples demonstrate this for 32 bit wide registers as in the STM32L476RG:

```C++
    volatile uint32_t * reg_ptr;
    reg_ptr = 0x40041C;
    *reg_ptr = register_value;
```
