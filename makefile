
# You may change these variables in your make commando:
# `make flash DEVICE=/dev/ttyUSB0 MAIN=usart_test`
# default device for flashing
DEVICE := /dev/ttyUSB0
# default main file (without file extension)
MAIN := adc_test
# default I2C speed
I2C := I2C_100

# Flags for compiling with avr-gcc
ACFLAGS := -mmcu=atmega328 -std=gnu99 -Os -Wall -Werror -DF_CPU=16000000UL
ACFLAGS += -I./ -D$(I2C) -I./plugs/
# used later for link time optimization
ACFLAGS += -ffunction-sections -fdata-sections

# Flags for linking with avr-gcc
ALFLAGS := -mmcu=atmega328
# Link time optimization (remove unused functions)
ALFLAGS += -Wl,--gc-sections

# Flags for flashing device with avrdude
FLASH_BAUDRATE := 115200
#FLASH_BAUDRATE := 57600
PROGRAMMER := arduino
DUDEFLAGS := -p m328p -c $(PROGRAMMER) -P $(DEVICE) -b $(FLASH_BAUDRATE) -v

# Object files with .c and .h files
OBJS := blink.o adc.o usart.o i2c.o plugs/rtc_plug.o
OBJS += random/xorshift.o random/mt19937.o
# .h files of .c files
HEADERS := $(OBJS:.o=.h)
# .h files without .c file
HEADERS += bits.h jeenode_pins.h
# Object files with .c files only
OBJS += plugs/dimmer_plug.o plugs/output_plug.o

HEX := $(MAIN).hex
main: $(HEX)

MAINS := adc_test usart_test
MAINS += plugs/dimmer_plug_test plugs/output_plug_test plugs/rtc_plug_test
all: $(OBJS) $(MAINS:=.hex)
	make -C host

# export object file names, so you can link them with your code
lib: $(OBJS)
	echo $(OBJS) > objects_export

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

