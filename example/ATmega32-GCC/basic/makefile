CC = avr-gcc
OBJCPY = avr-objcopy

MCU = atmega32
CLK = 8000000
OPT = -Os
CFLAGS = -Wall -Wextra -g -std=c99 -Wl,-u,vfprintf -lprintf_flt -lm

TARGET = output
BUILD_DIR = build
INC_DIR = ../../../src/include ../../../port/ATmega32-GCC ../common_files/Retarget
SRC = ./main.c ../../../src/MAX44009.c ../../../port/ATmega32-GCC/MAX44009_platform.c ../common_files/Retarget/Retarget.c


ifeq ($(OS),Windows_NT)
FIXPATH = $(subst /,\,$1)
RMD = rd /s /q
MD = mkdir
else
FIXPATH = $1
RMD = rm -r
MD = mkdir -p
endif


SOURCES = $(filter %.c, $(SRC))
INCLUDES = $(patsubst %,-I%, $(INC_DIR:%/=%))
CFLAGS +=  -mmcu=$(MCU) -DF_CPU=$(CLK) $(OPT)
OUTPUT_ELF = $(addsuffix .elf,$(call FIXPATH,$(BUILD_DIR)/$(TARGET)))
OUTPUT_HEX = $(addsuffix .hex,$(call FIXPATH,$(BUILD_DIR)/$(TARGET)))


all: $(BUILD_DIR) $(TARGET).hex

clean:
	$(RMD) $(call FIXPATH,$(BUILD_DIR))

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $(call FIXPATH,$(addprefix $(BUILD_DIR)/,$(notdir $@)))

# elf file
$(TARGET).elf: $(SOURCES:.c=.o)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(OUTPUT_ELF) $(call FIXPATH,$(addprefix $(BUILD_DIR)/,$(notdir $(SOURCES:.c=.o))))

# hex file
$(TARGET).hex: $(TARGET).elf
	$(OBJCPY) -j .text -j .data -O ihex $(OUTPUT_ELF) $(OUTPUT_HEX)

$(BUILD_DIR):
	$(MD) $(call FIXPATH,$(BUILD_DIR))
