PROJECT=temp
EXECUTABLE=$(PROJECT).elf
BIN_IMAGE=$(PROJECT).bin

CC=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy
ARCH=CM3

LIB_STM = ./libstm

TOOLCHAIN_PATH ?= /usr/local/csl/arm-2012.03/arm-none-eabi
C_LIB= $(TOOLCHAIN_PATH)/lib/thumb2

CFLAGS=-g -O2 -mlittle-endian -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=softfp
CFLAGS+=-mcpu=cortex-m4
CFLAGS+=-ffreestanding -nostdlib
#CFLAGS+=-D __FPU_PRESENT=1 -D ARM_MATH_CM4

# to run from FLASH
CFLAGS+=-Wl,-T,$(LIB_STM)/stm32_flash.ld

CFLAGS+=-I./

# stm32f4_discovery lib
CFLAGS+=-I./libstm/STM32F4xx_StdPeriph_Driver/inc
CFLAGS+=-I./libstm/STM32F4xx_StdPeriph_Driver/inc/device_support
CFLAGS+=-I./libstm/STM32F4xx_StdPeriph_Driver/inc/core_support


#STM32F4xx_StdPeriph_Driver\inc
CFLAGS+=-I./libstm/STM32F4xx_StdPeriph_Driver/inc

#Utilities
CFLAGS+=-I./libstm/Utilities/STM32F4-Discovery

#Source Files
SRC += main.c system_stm32f4xx.c stm32f4xx_it.c usart3.c

#STARTUP
SRC+=$(LIB_STM)/startup_stm32f4xx.s

#Utilities
SRC+=$(LIB_STM)/Utilities/STM32F4-Discovery/stm32f4_discovery.c  \
#	$(LIB_STM)/Utilities/STM32F4-Discovery/stm32f4_discovery_lis302dl.c


all: $(BIN_IMAGE)

libstm_build:
	$(MAKE) -C libstm/STM32F4xx_StdPeriph_Driver/build

$(BIN_IMAGE): $(EXECUTABLE)
	$(OBJCOPY) -O binary $^ $@

$(EXECUTABLE): main.c $(SRC)
	$(CC) $(CFLAGS) $^ -o $@  -L./libstm/STM32F4xx_StdPeriph_Driver/build \
		-lSTM32F4xx_StdPeriph_Driver -L$(C_LIB) -lc -lm -lg -lgcc

clean:
	rm -rf $(EXECUTABLE)
	rm -rf $(BIN_IMAGE)

flash:
	st-flash write $(BIN_IMAGE) 0x8000000

.PHONY: all clean
