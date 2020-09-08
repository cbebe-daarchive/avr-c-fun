# compiler options
CC=avr-gcc
CFLAGS=-Os -DF_CPU=16000000UL -mmcu=atmega328p

all: blink.out

USBPORT:=/dev/ttyS3

%.out: %.c
	$(CC) $(CFLAGS) $< -o $@

%.hex: %.out
	avr-objcopy -O ihex -R .eeprom $< $@

# Upload to board

install.%: %.hex
	avrdude -F -V -c arduino -p ATMEGA328P -P ${USBPORT} -b 115200 -U flash:w:$<

clean:
	rm -f *.hex *.out