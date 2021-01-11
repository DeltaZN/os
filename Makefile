CC = gcc
ASM = nasm

all: os-image

run: all
	qemu-system-x86_64 os-image

kernel.o: kernel.c
	$(CC) -ffreestanding -c $< -o $@

bootstrap.bin: bootstrap.asm
	$(ASM) $< -f bin -o $@

kernel.bin: kernel.o
	$(LD) -o $@ -Ttext 0x1000 $^ --oformat binary

os-image: bootstrap.bin kernel.bin
	cat $^ > $@

clean:
	rm *.bin *.o os-image
