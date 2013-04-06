
# You may change these variables in your make commando:
# `make flash DEVICE=/dev/ttyUSB0 MAIN=usart_test`
# default device for flashing
DEVICE := /dev/ttyUSB0
# default main file (without file extension)
MAIN := simple_adc

# Flags for compiling with avr-gcc
ACFLAGS := -mmcu=atmega328 -std=gnu99 -Os -Wall -Werror -DF_CPU=16000000L -Wl,-u,vfprintf

# Flags for linking with avr-gcc
ALFLAGS := -mmcu=atmega328
# For printf variants in avr-libc's stdio (read_supply_voltage):
#ALFLAGS := -lprintf_flt -lm -Wl,-u,vfprintf

# Flags for flashing device with avrdude
DUDEFLAGS := -p m328p -c stk500v1 -P $(DEVICE) -b 115200 -c arduino

OBJS := adc.o
HEADERS := $(OBJS:.o=.h) bits.h jeenode_pins.h

HEX := $(MAIN).hex

all: $(HEX)

# flash hex onto chip
flash: $(HEX)
	avrdude $(DUDEFLAGS) -U flash:w:"$(HEX)"

# make hex
%.hex: %.elf
	avr-objcopy -j .text -j .data -O ihex $*.elf $*.hex

# compile
%.o: %.c $(HEADERS)
	avr-gcc $(ACFLAGS) -c -o $*.o $*.c

# link
%.elf: $(MAIN).o $(OBJS)
	avr-gcc $(ALFLAGS) $^ -o $*.elf

# delete generated data
clean:
	rm -f *.hex *.elf *.o
	make -C host clean

