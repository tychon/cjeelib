
# You may change these variables in your make commando:
# `make flash DEVICE=/dev/ttyUSB0 MAIN=usart_test`
# default device for flashing
DEVICE := /dev/ttyUSB0
# default main file (without file extension)
MAIN := simple_adc
# default I2C speed
I2C := I2C_100

# Flags for compiling with avr-gcc
ACFLAGS := -mmcu=atmega328 -std=gnu99 -Os -Wall -Werror -DF_CPU=16000000UL
ACFLAGS += -I./ -D$(I2C)
ACFLAGS += -Wl,-u,vfprintf

# Flags for linking with avr-gcc
ALFLAGS := -mmcu=atmega328
# For printf variants in avr-libc's stdio (read_supply_voltage):
#ALFLAGS := -lprintf_flt -lm -Wl,-u,vfprintf

# Flags for flashing device with avrdude
FLASH_BAUDRATE := 115200
#FLASH_BAUDRATE := 57600
PROGRAMMER := arduino
DUDEFLAGS := -p m328p -c $(PROGRAMMER) -P $(DEVICE) -b $(FLASH_BAUDRATE) -v

# Object files with .c and .h files
OBJS := adc.o usart.o i2c.o
OBJS += random/xorshift.o random/mt19937.o
# .h files of .c files
HEADERS := $(OBJS:.o=.h)
# .h files without .c file
HEADERS += bits.h jeenode_pins.h
# Object files with .c files only
OBJS += plugs/dimmer_plug.o

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
	find -name \*.hex -exec rm {} \;
	find -name \*.elf -exec rm {} \;
	find -name \*.o   -exec rm {} \;
	make -C host clean

