# ELEC2645-project

## Introduction

I have been working on the content of this repository since the beginning of this semester (January 2022), however, only as of today (April 9th 2022) have I uploaded the content publicly.

Therefore, any commits before this date represent my various experiments with the ST Nucleo L476RG development board and are intended to serve as the basis for my ELEC2645 module project, the code for which will be uploaded here shortly.

First, some of what I have learned from my experiments.

### **Look mum! No operating system**

For a long while I've been very interested in exactly how programs are loaded and executed on a processor outside of an operating system. I know for instance that operating systems like GNU/Linux or Microsoft Windows provide a set of system calls which can be accessed by the C / C++ standard library to perform tasks like memory allocation or reading and writing of input / output streams, and that the operating system handles the setting up of stack and heap memory for the program to use when it runs. However, on a bare microprocessor none of these facilities are available.

I have a fair of experience from my past employment writing embedded software in C (never C++ before attending university) targeted at Atmel and PIC microcontrollers. However, in such cases the development environment (AVR-GCC and associated tools being what I am most familiar with) has provided a convenient hardware abstraction layer in the form of preprocessor macros for accessing the registers and has obscured the mechanisms by which the program is loaded on the processor and the ```main()``` function called.

I have no prior familiarity with the ARM based stm32 microprocessors so I chose this as an opportunity to investigate developing my software "from scratch" and implementing all of the necessary procedures to get my program to run so-called "bare metal".

### **Register access**

I've often thought that, in an embedded setting, in C (or C++) it should be possible to access a register by creating a pointer to a data type with the same width as the register, setting the value of that pointer to the registers memory address and then dereferencing the pointer to either assign or read it's value. Of course, pulling this type of trick while running in an operating system is most likely to result in an immediate segmentation fault, if not worse.

It turns out though that without an operating system this trick is exactly possible. The following examples demonstrate this for 32 bit wide registers as in the STM32L476RG:

```C++
    volatile uint32_t * reg_ptr;
    reg_ptr = 0x40041C;
    *reg_ptr = register_value;
```
It is crucial that the integer pointed to by reg_pointer be declared volatile, since it can change at any time during the program execution (it represents the contents of a register which may be updated by the processor or by a peripheral at any time). If volatile is omitted certain compiler optimisations can prevent register values from being read when requested.

The above example can be simplified by taking the constant value of the address, casting it to a pointer to a volatile 32-bit unsigned integer and immediately dereferencing it before assigning it a value:

```C++
    *((volatile uint32_t *)0x40041C) = register_value;
```

The left hand side of the assignment can be packaged up as a preprocessor macro, thus providing a convenient name for the register that can be read or assigned like a variable, and provides a basic hardware abstraction layer. For example:

```C++
    #define IMPORTANT_REGISTER *((volatile uint32_t *)0x40041C)

    IMPORTANT_REGISTER = register_value;
    uint32_t read_value = IMPORTANT_REGISTER;
```

### **Objects and offsets**

To the developer, an operating system provides the convenient illusion that the application program simply begins running from ```main()```. Even in the case of a relatively low level language like C and especially in the case of C++ there are a number of run-time facilities that are required to make this happen. For instance, in a C++ program where a global instance of a class is declared, how will the constructor of that class be run if the program begins execution from ```main()```? Similarly, how will const initialised or uninitialised variables be placed into memory for the program to use? These are examples of tasks usually taken care of by the "run-time environment"

When a C++ program is compiled using the GNU ARM C++ compiler (arm-none-eabi-g++), by default the output will be a relocatable object file in the "Executable Link Format" (ELF). Such an output file contains executable machine code, however all of the addresses in the code are offsets relative to some origin that is yet to be determined. Additionally the file will contain symbols in the symbol table that are not referenced in the compiled code and are expected to be provided by the run-time environment. 

Within an operating system, on the execution of a relocatable object file the scheduler will schedule it as a process, providing the appropriate offset into main memory where it should run. The OS will also provide all of the as yet undefined symbols needed at run-time. However, absent an OS it is the developers responsibility to handle the origin memory offsets, undefined symbols and so forth. This can be done by compiling and linking the program separately and having enough start up code to handle the run-time environment tasks.

The object file that is produced from the compiled source consists of several different sections (the compiled information in the file is of course just a string of bytes, however some of these bytes make up instructions and some are data, as indicated by the ELF header at the start of the file). These are the ".text" section, the ".rodata" section, the ".init_array" section, the ".data" section and the ".bss" section.

- **.text** - contains all of the machine code instructions that make up the executable program. On the STM32L476RG these of the ARM "thumb" instruction set, 16-bits wide.

- **.rodata** - contains read only data. Anything that is a compile time constant within the source, such as numerical constants, character constants or string literals.

- **.init_array** - is a contiguous list of 32-bit words generated by the compiler which are pointers to all of the constructor functions of objects declared outside of ```main()```, or any functions declared with ```__attribute((constructor))```. These function pointers must be run at initialisation before the main application is started.

- **.data** - contains the values of all of the initialised variables within the source-code.

- **.bss** - is a section of all zero large enough to contain all of the declared but uninitialised variables.

### **Anatomy of my project**

The following paragraphs explain how the various source file of my project are compiled and linked together, and the linker script I have written to facilitate this. I use a makefile to build my project and flash the resulting binary to the microcontroller, so all of the details of the compilation steps and all of the compiler flags can be viewed in [makefile](https://github.com/AVatLeeds/ELEC2645-project/blob/master/makefile).
My project is split into two seperately compiled and linked chunks, my main application and a startup file that provides run-time utilities.

[main.cpp](https://github.com/AVatLeeds/ELEC2645-project/blob/master/main.cpp), when it is finished, will contain the main body of my application. It includes the header files of all the various drivers and libraries I have been writing, such as [GPIO_driver.h](https://github.com/AVatLeeds/ELEC2645-project/blob/master/GPIO_driver.h), [USART_driver.h](https://github.com/AVatLeeds/ELEC2645-project/blob/master/USART_driver.h), [SPI.h](https://github.com/AVatLeeds/ELEC2645-project/blob/master/SPI.h),  [LCD_HD61830_driver.h](https://github.com/AVatLeeds/ELEC2645-project/blob/master/LCD_HD61830_driver.h). The main source file "main.cpp" and the source files for all of the other drivers and libraries are compiled together and partially linked as shown by the following section from the makefile:

```makefile
    project.o: main.cpp systick.cpp USART_driver.cpp GPIO_driver.cpp LCD_HD61830_driver.cpp
	    $(CC) $(CFLAGS) -I. -r -o $@ $^
```

- The object file project.o is made from all of the sources using the arm-none-eabi-g++ compiler.

- The option "-I." adds the current directory to the include path so that all of the header files are searched during preprocessing.

- The "-r" option specifies that the compiler should produce a relocatable object file. In this case all of the sources will be compiled and linked with each other but, based on the compiler flags, no linking with external run-time utilites will be attempted and all of the addresses in the object file will be relative.

My startup file is compiled seperately from "main.cpp"

```makefile
    startup_STM32L476RG.o: startup_stm32l476rg.c
```

- The "startup_STM32L476RG.o" object is made from "startup_stm32l476rg.cpp" using the default compiler (in my case arm-none-eabi-g++). The default compiler flags that I have set up in the make file include "-C" so this source file is only compiled and not linked.

I now have object files for my main application and for my startup run-time environment. The final step is to link these two object files together and specify all of the addresses and sections correctly for the target platform. For this I have used a linker script [startup_stm32l476rg.ld](https://github.com/AVatLeeds/ELEC2645-project/blob/master/startup_stm32l476rg.ld).

A linker scripts provides instructions to the linker about how it should organise the sections of the object file that it produces, information about the different memories of the target platform which it can use to correctly order the addresses of everything in the object file. In my project I also use the linker script to resolve the values of certain symbols during the linking process which are referenced in my start up source code.

```
    MEMORY
    {
        FLASH(rx) : ORIGIN = 0x08000000, LENGTH = 1024K
        SRAM1(rwx) : ORIGIN = 0x20000000, LENGTH = 96K
        SRAM2(rwx) : ORIGIN = 0x10000000, LENGTH = 32K   /* SRAM2 not presently used */
    }
```

This section of my linker script specifies the different memories avalible on my target platform, their attributes (readable, writable or executable), their origin address, and their size.

```
    SECTIONS
    {
        .text :
        {
            start_of_text_section = .;  /* start of text section symbol with current address of location counter */
            startup_stm32l476rg.o(.vector_table)
            *(.text)                    /* combine text sections from all object files */
            *(.rodata)                  /* combine read only sections from all object files */
            . = ALIGN(4);               /* ensure the location counter is aligned to a word boundary at end of the section */
            end_of_text_section = .;    /* end of text section symbol with current address of location counter */
        } > FLASH                       /* VMA and LMA are both relative to FLASH origin */
    
        ...
    }
```

In the rest of my linker script I specify the layout and order of the different sections of the final object file.

The ".text" section of my executable combines the ".text" and ".rodata" sections of all the other object files that are being linked using the syntax ```*(.text)``` and ```*(.rodata)```.

The linker script has a feature called the location counter, syntactically denoted by ".". When the location counter is read it will return the address of that particular point withing the object file, allowing symbols to be created with the address in the object file at certain points. For example ```start_of_text_section = .;``` and ```end_of_text_section = .;```. The address counter can also be written to, as in ```. = ALIGN(4);```. I have done this in order to ensure that the boundary between each section is aligned to 32-bit word boundaries.

The symbols that are declared in the linker script can be accessed within the source code by declaring variables of the same name as ```extern```. This way The linker is expecting to resolve these symbols external to the source and finds them in the linker script. This is a crucial method for passing information about the addresses of the section boundaries that can only be know at link time back into source code. This is The way that my startup code moves necessary data into RAM when the processor boots, as I will show further on.

In the makefile the final firmware object file is produced using my linker script as follows:

```makefile
    firmware.o: startup_stm32l476rg.o project.o
    	$(LD) $(LDFLAGS) -o $@ $^
```

"firmware.o" will still contain extraneous header information as a result of the ELF format that must be removed before flashing the program onto the microcontroller. This is done with a GNU utility called "objcopy", specifically "arm-none-eabi-objcopy" in the case of ARM. This results in a binary file "firmware.bin".

```makefile
    firmware.bin: firmware.o
    	arm-none-eabi-objcopy -O binary $^ $@
```

### **Interrupt vector table**

The STM32L476RG requires that the first thing to appear in the program memory, starting at address 0x08000000, is the interrupt vector table. This is so that the interrupt handlers are predetermined to be found at the correct addresses for each of the interrupts (when an event interrupts the processor the program counter gets set to the address of that particular interrupt vector).

To achieve this within my startup code I create an array of function pointers in the correct order that their addresses will match those specified in the datasheet. The "vector_table" block will be positioned in the correct place within the finished object file by the linker, by using the section attribute

```C++
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
        sys_tick_handler
        
        ...
    }
```

- The ```__attribute__((section(".vector_table")))``` will put all data and instructions generated from compiling this block into a section called ".vector_table" within the object file.

- zeroes are including to pad out regions in the vector table that are reserved.

- the first entry that is required in the vector table is the initial value of the stack pointer, denoted by "STACK_START". The value of "STACK_START" is cast to a void function pointer with no arguments to conform with the type of the vector table array.

- The value of "STACK_START" is the address of the end of the SRAM. The end address is used since the stack grows downward from the top or RAM.

- The rest of the entries in the vector table array are the various handler functions for each of the interrupt vectors.

The vector table array must contain entries for all of the interrupt handler functions so that it takes up the correct amount of space in the program memory and so that every vector contains the address of a valid function. However, it would be very inconvenient to have to implement every interrupt handler up front. Therefore, aliasing is used so that most of the interrupt vectors can run a default handler and a more specific implementation can be done later, as shown in the following example:

```C++
    extern "C"
    {
        void default_handler(void);
    }
    
    void sys_tick_handler(void) __attribute__((weak, alias("default_handler")));

    void default_handler(void)
    {
        while (1);
    }
```

- the "sys_tick" handler function is declared with a weak alias attribute to "default_handler". The weak part of the attribute means that the alias can be overwritten if the linker sees an alternative symbol for a function of the same name anywhere else in the source-code.

- The "default_handler" declaration must be done inside an ```extern "C"``` block for the alias to work. This is because, to enable overloading of functions the C++ compiler performs a process called symbol mangling. Function overloading means the compiler may have to produce multiple instantiations of a single function depending on its usage in the source, and these functions cannot have the same name in the symbol table. Therefore (even if no function overloading is performed) the C++ compiler will modify the symbol name by appending and prepending a selection of characters. This will cause the alias attribute to break and results in undefined symbol errors at link time, since the alias expects a non-mangled symbol name (the same as the function name).

- The only handler that critically must be implemented is the reset handler. The program counter is set to the reset vector when the processor powers on or comes out of sleep, so the reset handler is the first function that will be run.

### **Boot up**

The compiled binary file "firmware.bin" is loaded into the flash memory on the microcontroller. However, when the application runs it expects to access all of the global program variables in the microcontrollers RAM. All of the initialised global variables must be moved to the RAM and enough space made for the uninitialised variables.

The STM32L476RG has to sections of static RAM (SRAM). Within my project I am only using the larger of the two SRAM sections, SRAM1, 96 Kilobytes starting at address 0x20000000.

The reset interrupt handler function which gets run when the processor powers on is responsible for setting up the memory as described above. Here's an example:

```C++
    extern uint32_t start_of_text_section; // relative to FLASH origin
    extern uint32_t start_of_data_section; // relative to VMA (SRAM1 origin)
    extern uint32_t start_of_bss_section;  // relative to VMA (SRAM1 origin)

    extern uint32_t end_of_text_section;   // relative to FLASH origin
    extern uint32_t end_of_data_section;   // relative to VMA (SRAM1 origin)
    extern uint32_t end_of_bss_section;    // relative to VMA (SRAM1 origin)

    void reset_handler(void)
    {
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

        ...
```

- The variables at the top of the example are the addresses for the start and end of each section of the compiled object file. They are declared extern as they are not known until link time, when they will be determined using the linker script.

- Inside the "reset_handler" function pointers are made to the bytes in the flash and the sram and then the size of the data section (in bytes) calculated by taking the difference of the start and end addresses

- A for loop is used to copy the value of the flash pointer to the sram pointer over the entire length of the data section. This copies all of the initialised global variables to the SRAM.

- A pointer is then made to the start of the bss section in SRAM, it's size calculated and then a for loop used to fill all of the bytes in the bss section in SRAM with zero.

As mentioned earlier on, as part of the startup process the constructors for any objects declared outside of ```main()``` must be run before ```main()``` is called. The C++ compiler generates an array of function pointers for all such constructors in a section called ".init" array of the compiled object file. This provides a means for the startup program to execute these constructors, as shown in the following example.

```C++
    extern void (* start_of_init_array)();
    extern void (* end_of_init_array)();

    void reset_handler(void)
    {
        ...

        // iterate over the list of pointers to constructors each time dereferencing the constructor pointer and executing it
        void (** constructor_ptr)();
        for (constructor_ptr = &start_of_init_array; constructor_ptr < &end_of_init_array; constructor_ptr ++)
        {
            (*constructor_ptr)();
        }

        ...
```

- "start_of_init_array" and "end_of_init_array" are the function pointers at the beginning and end of the array. They are declared extern as their values only become known at final link time, using the linker script.

- within the reset handler function a for loop iterates over all of the function pointer in the array, executing each one.

The final step that "reset_handler" performs is to call the main function, ```main();```. "main" having previously been declared as ```extern int main(void)``` (since, again, it's address is not known until final link time).

The full startup code can be explored in detail in [startup_stm32l476rg.c](https://github.com/AVatLeeds/ELEC2645-project/blob/master/startup_stm32l476rg.c)










