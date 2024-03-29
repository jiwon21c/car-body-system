###############################################################################
# Makefile for the project Door1_CAN_slave
###############################################################################

## General Flags
PROJECT = Door1_CAN_slave
MCU = atmega128
TARGET = Door1_CAN_slave.elf
CC = avr-gcc.exe

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -gdwarf-2 -O0
CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d 

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS += 


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0


## Objects that must be built in order to link
OBJECTS = Door1_CAN_slave.o dpy_trm_s01.o mcp2515.o mcu_avr_atmega128_api.o 

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: $(TARGET) Door1_CAN_slave.hex Door1_CAN_slave.eep size

## Compile
Door1_CAN_slave.o: ../Door1_CAN_slave.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

dpy_trm_s01.o: ../dpy_trm_s01.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

mcp2515.o: ../mcp2515.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

mcu_avr_atmega128_api.o: ../mcu_avr_atmega128_api.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

##Link
$(TARGET): $(OBJECTS)
	 $(CC) $(LDFLAGS) $(OBJECTS) $(LINKONLYOBJECTS) $(LIBDIRS) $(LIBS) -o $(TARGET)

%.hex: $(TARGET)
	avr-objcopy -O ihex $(HEX_FLASH_FLAGS)  $< $@

%.eep: $(TARGET)
	avr-objcopy $(HEX_EEPROM_FLAGS) -O ihex $< $@

%.lss: $(TARGET)
	avr-objdump -h -S $< > $@

size: ${TARGET}
	@echo
	@avr-size -C --mcu=${MCU} ${TARGET}

## Clean target
.PHONY: clean
clean:
	-rm -rf $(OBJECTS) Door1_CAN_slave.elf dep/* Door1_CAN_slave.hex Door1_CAN_slave.eep

## Other dependencies
-include $(shell mkdir dep 2>/dev/null) $(wildcard dep/*)

