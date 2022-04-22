# ELEC2645-project

## Introduction

I have been working on the content of this repository since the beginning of this semester (January 2022), however, only as of today (April 9th 2022) have I uploaded the content publicly.

Therefore, any commits before this date represent my various experiments with the ST Nucleo L476RG development board and are intended to serve as the basis for my ELEC2645 module project, the code for which will be uploaded here shortly.

First, some of what I have learned from my experiments.

### Look mum! No operating system:

For a long while I've been very interested in exactly how programs are loaded and executed on a processor outside of an operating system. I know for instance that operating systems like GNU/Linux or Microsoft Windows provide a set of system calls which can be accessed by the C / C++ standard library to perform tasks like memory allocation or reading and writing of input / output streams, and that the operating system handles the setting up of stack and heap memory for the program to use when it is run as a process, and is responsible for moving all of the data in the executable file into the correct places in the memory for the process to begin. However on a bare microprocessor none of these facilities are available.

I have a fair bit of past experience writing embedded software in C (never C++ before attending university) targeted at Atmel and PIC microcontrollers. However, in such cases the development environment (AVR-GCC and associated tools being what I am most familiar with) has provided a convenient hardware abstraction layer in the form of preprocessor macros for accessing the registers and has obscured the mechanisms by which the program is loaded on the processor and the "main()" function called.

I have no prior familiarity with the ARM based stm32 microprocessors so I chose this as an opportunity to investigate developing my software "from scratch" and implementing all of the necessary procedures to get my program to run.

### Register access.

I've often thought that, in an embedded setting, in C (or C++) it should be possible to access a register by creating a pointer to a data type with the same width as the register, setting the value of that pointer to the registers memory address and then dereferencing the pointer to either assign or read it's value. Of course pulling this type of trick while running in an operating system is most likely to result in an immediate segmentation fault at least.

It turns out though that without an operating system this trick is exactly possible. The following examples demonstrate this for 32 bit wide registers as in the STM32L476RG:

```C++
    volatile uint32_t * reg_ptr;
    reg_ptr = 0x40041C;
    *reg_ptr = register_value;
```
It is crucial that the integer pointed to by reg_pointer be declared volatile, since it can change at any time during the program execution (it represents the contents of a register which may be updated by the processor or by a peripheral). If volatile is omitted certain compiler optimisations can prevent register values from being read when requested.

The above example can be simplified by taking the constant value of the address, casting it to a pointer to a 32-bit unsigned integer and immediately dereferencing it before assigning it a value:

```C++
    *((volatile uint32_t *)0x40041C) = register_value;
```

The left hand side of the assignment can be packaged up as a preprocessor macro, thus providing a convenient name for the register that can be read or assigned like a variable, and provides a basic hardware abstraction layer. For example:

```C++
    #define IMPORTANT_REGISTER *((volatile uint32_t *)0x40041C)

    IMPORTANT_REGISTER = register_value;

    uint32_t read_value = IMPORTANT_REGISTER;
```

### Objects and offsets

To the developer, an operating system provides the convenient illusion that the application program simply begins running from ```main()```. Even in the case of a relatively low level language like C and especially in the case of C++ there are a number of run-time facilities that are required to make this happen. For instance, in a C++ program where a global instance of a class is declared how will the constructor of that class be run if the program begins execution from main? Similarly, how will const initialised or uninitialised variables be placed into memory for the program to use? These are examples of tasks usually taken care of by the "run-time environment"

When a C++ program is compiled using the GNU ARM C++ compiler (arm-none-eabi-g++), by default the output will be a relocatable object file in the "Exectuable Link Format" (ELF). Such an output file contains executable machine code, however all of the addresses in the code are offsets relative to some origin that is yet to be determined. Additionally the file will contain symbols in the symbol table that are not referenced in the compiled code and are expected to be provided by the run-time environment. 

Within an operating system, on the execution of a relocatable object file the scheduler will schedule it as a process, providing the appropriate offset into main memory where it should run. The OS will also provide all of the as yet undefined symbols needed at run-time. However, absent an OS it is the developers responsibility to handle the origin memory offsets, undefined symbols and so forth. This can be done by compiling and linking the program separately and having enough start up code to handle the run-time tasks.

For my project I compile and partially link all of the sources for the content of my project, "main.cpp" and the sources for all other libraries, drivers and classes, then compile my hardware specific startup sourcecode "startup_stm32l476rg.c" and then link the two resulting object files into one finished executable





The object file that is produced from the compiled source consists of several different sections (the compiled information in the file is of course just a string of bytes, however some of these bytes make up instructions and some are data, as indicated by the ELF header at the start of the file). These are the ".text" section, the ".rodata" section, the ".init_array" section, the ".data" section and the ".bss" section.

- **.text** - contains all of the machine code instructions that make up the executable program. On the STM32L476RG these of the ARM "thumb" instruction set, 16-bits wide.

- **.rodata** - contains read only data. Anything that is a compile time constant within the source, such as numerical constants, character constants or string literal constants.

- **.init_array** - is a contiguous list of 32-bit words generated by the compiler which are pointers to all of the constructor functions of objects declared outside of ```main()```, or any functions declared with ```__attribute((constructor))```. These function pointers must be run at initialisation before the main application is started.

- **.data** - contains the values of all of the initialised variables within the source code.

- **.bss** - is a section of all zero large enough to contain all of the declared but uninitialised variables.

before loading the compiled program onto the microcontroller all of the extraneous header and format information must be stripped out of the object file. This is done with a GNU utility called "objcopy", specifically "arm-none-eabi-objcopy" in the case of ARM. This results in a binary file "firmware.bin".

The application binary is loaded into the flash memory of the microcontroller



