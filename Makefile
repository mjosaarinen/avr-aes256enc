# Makefile

PROJ		= avr-aes256enc
TARGET		= atmega328p
CPUFREQ		= 16000000
WIRE		= arduino
DEVICE		= /dev/ttyUSB0
CC		= avr-gcc
OBJCOPY		= avr-objcopy
AR		= avr-ar
STRIP		= avr-strip
AVRDUDE		= avrdude

CFLAGS  	= -Wall -Ofast -mmcu=$(TARGET) -DF_CPU=$(CPUFREQ)

OBJS 		= obj/aes256enc.o

$(PROJ):	speed.c ser_print.c obj/$(PROJ).a
		$(CC) $(CFLAGS) $^ -o $@

%.hex:		%
		$(OBJCOPY) -O ihex -R .eeprom $^ $@

obj/$(PROJ).a: 	$(OBJS)
		$(AR) -ar cr $@ $^

obj/%.o: %.[cS]
		mkdir -p obj/
		$(CC) $(CFLAGS) -c $^ -o $@

obj/%.S: %.c
		$(CC) $(CFLAGS) -S $^ -o $@

clean:
		rm -rf obj $(PROJ) $(PROJ).hex $(PROJ)-*.txz

dist:		clean
		cd ..; \
		tar cfvJ $(PROJ)/$(PROJ)-`date "+%Y%m%d%H%M00"`.txz $(PROJ)/*

# simulate with avr
sim:		$(PROJ)
		simavr -v -v -v -m atmega328p $(PROJ)

# flash on device, dump serial output
flash:		$(PROJ).hex
		$(AVRDUDE) -v -c $(WIRE) -p $(TARGET) -P $(DEVICE) \
			-U flash:w:$(PROJ).hex
		stty -F $(DEVICE) raw icanon eof \^d 38400
		cat < $(DEVICE)
