CC = gcc
ASM = nasm
LD = ld

all: os-image

run: all
	qemu-system-x86_64 -fda os-image

kernel.o: kernel.c
	$(CC) -fno-pie -m32 -ffreestanding -c $< -o $@

kernel_entry.o: kernel_entry.asm
	$(ASM) $< -f elf32 -o $@

bootstrap.bin: bootstrap.asm
	$(ASM) $< -f bin -o $@

kernel.bin: kernel_entry.o kernel.o
	$(LD) -m elf_i386 -o $@ -Ttext 0x1000 $^ --oformat binary

os-image: bootstrap.bin kernel.bin
	cat $^ > $@

clean:
	rm *.bin *.o os-image
