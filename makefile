CC = arm-none-eabi-gcc

CFLAGS =	-g0 \
			-Os \
			-Wall \
			-Wextra \
			-mthumb \
			-mno-thumb-interwork \
			-mcpu=cortex-m4 \
			-nostartfiles \

LD = arm-none-eabi-ld

LDFLAGS =	-nostdlib \
			-Map=firmware.map \
			-T startup_stm32l476rg.ld

STL = st-flash

firmware.bin: firmware.o
	arm-none-eabi-objcopy -O binary firmware.o firmware.bin

firmware.o: startup_stm32l476rg.o main.o
	$(LD) $(LDFLAGS) -o firmware.o startup_stm32l476rg.o main.o

main.o: main.c

startup_STM32L476RG.o: startup_stm32l476rg.c

flash:
	st-flash erase
	st-flash reset
	st-flash write firmware.bin 0x08000000

clean:
	rm main.o startup_stm32l476rg.o firmware.o firmware.bin firmware.map