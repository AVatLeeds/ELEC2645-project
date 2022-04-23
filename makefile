CC = arm-none-eabi-g++

CFLAGS =	-g0\
			-Os\
			-Wall\
			-Wextra\
			-mthumb\
			-mno-thumb-interwork \
			-mcpu=cortex-m4 \
			-nostartfiles\
			-specs=nano.specs\
			-specs=nosys.specs\
			-fno-rtti\
			-fno-exceptions #disabling c++ exception handling as otherwise causes a bunch of missing symbols

LD = arm-none-eabi-ld

LDFLAGS =	-nostdlib\
			-nostartfiles\
			-Map=firmware.map\
			-T startup_stm32l476rg.ld\

STL = st-flash

#INCLUDE =	-I./STM32CubeL4-1.17.1/Drivers/CMSIS/Core/Include\
			-I./STM32CubeL4-1.17.1/Drivers/CMSIS/Device/ST/STM32L4xx/Include\

firmware.bin: firmware.o
	arm-none-eabi-objcopy -O binary $^ $@

firmware.o: startup_stm32l476rg.o project.o
	$(LD) $(LDFLAGS) -o $@ $^

startup_STM32L476RG.o: startup_stm32l476rg.c

project.o: main.cpp systick.cpp USART_driver.cpp GPIO_driver.cpp SPI_driver.cpp
	$(CC) $(CFLAGS) -I. -r -o $@ $^
# -r produces a relocatable object (partial linking)

flash:
	st-flash erase
	st-flash reset
	st-flash write firmware.bin 0x08000000

clean:
	rm *.o firmware.bin firmware.map