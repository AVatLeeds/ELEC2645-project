CC = arm-none-eabi-gcc

CFLAGS =	-g0\
			-Os\
			-Wall\
			-Wextra\
			-mthumb\
			-mno-thumb-interwork \
			-mcpu=cortex-m4 \
			-nostartfiles\

LD = arm-none-eabi-ld

LDFLAGS =	-nostdlib\
			-Map=firmware.map\
			-T startup_stm32l476rg.ld\

STL = st-flash

OBJS =	startup_stm32l476rg.o\
		main.o\
		system.o\

firmware.bin: firmware.o
	arm-none-eabi-objcopy -O binary $^ $@

firmware.o: $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $^

startup_STM32L476RG.o: startup_stm32l476rg.c

main.o: main.c
system.o: system.c system.h

flash:
	st-flash erase
	st-flash reset
	st-flash write firmware.bin 0x08000000

clean:
	rm *.o firmware.bin firmware.map