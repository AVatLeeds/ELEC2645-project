## Developing for ARM Cortex-M microprocessors on Linux.

ARM licenses the design of the Cortex-M processor to vendors who produce their own physical implementations in silicon with various peripheral feature-sets.
The ARM® Cortex® Microcontroller Software Interface Standard (CMSIS) is a vendor-independent hardware abstraction layer for the Cortex-M processor series and specifies debugger interfaces. [From here](http://regalis.com.pl/en/arm-cortex-stm32-gnulinux/)

MBED is real time operating system produced by ARM for use on their Cortex-M microcontrollers. It consists of a large set of library files wrapped into a single header, mbed.h. It provides C++ classes to access and configure the microcontrollers peripherals. MBED probably provides a convenient solution for rapid development. However, it's performance should still be verified.

- direct register manipulation
- CMSIS headers
- ST LL (low level) driver headers
- ST HAL (hardware abstraction layer) driver headers
- ARM MBED

links:
- example with CMSIS header: https://embetronicx.com/tutorials/microcontrollers/stm32/stm32-gpio-tutorial/#GPIOx_MODER
- STM32 bare metal programming: https://github.com/a5221985/tutorials/blob/master/Embedded_Systems/embedded_systems_bare_metal_programming_ground_up.md
- using opencm3: https://rhye.org/post/stm32-with-opencm3-0-compiling-and-uploading/
- STM32 programming: http://regalis.com.pl/en/arm-cortex-stm32-gnulinux/
- bare metal, assembly, linker script: https://vivonomicon.com/2018/04/02/bare-metal-stm32-programming-part-1-hello-arm/
- writing directly to memory addresses: https://stackoverflow.com/questions/2417195/whats-the-shortest-code-to-write-directly-to-a-memory-address-in-c-c
- bare metal blink: https://eleceng.dit.ie/frank/arm/BareMetalSTM32L476Discovery/index.html
- another bare metal blink, assembly run time: https://freeelectron.ro/bare-metal-stm32-led-blink/
- reusable firmware development: https://www.amazon.co.uk/Reusable-Firmware-Development-Practical-Approach/dp/1484232968

links with noteworthy examples:
- https://vivonomicon.com/2018/04/02/bare-metal-stm32-programming-part-1-hello-arm/
- https://vivonomicon.com/2018/04/20/bare-metal-stm32-programming-part-2-making-it-to-main/
    - bare minimum startup code done in assembly language

- https://github.com/Ahmed0Ayman/STM32-Compilation-Process-/blob/master/startup.c
    - it appears the same thing can be done in C

- https://stackoverflow.com/questions/38443734/how-to-compile-stm32f103-program-on-ubuntu
    - a useful SO answer with detailed examples in assembly language. Don't fully understand

- https://hackaday.com/series_of_posts/stm32-bootcamp/
- https://github.com/MayaPosch/Nodate
    - a Hackaday series on the stm32. Posts not hugely useful but Github link provides a bare minimum framework