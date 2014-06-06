all: upload

watch.o: *.c *.h
	avr-gcc -g -Os -Wall -std=c99 -mmcu=atmega48a -c watch.c

watch.elf: watch.o
	avr-gcc -g -mmcu=atmega48a watch.o -o watch.elf

watch.hex: watch.elf
	avr-objcopy -j .text -j .data -O ihex watch.elf watch.hex

upload: watch.hex
	avrdude -p m48 -c usbasp -U flash:w:watch.hex

clean:
	rm -f *.o
	rm -f *.hex
	rm -f *.elf
	rm -f *.lst
