# Basic OS
Very basic operating system with bootloader and kernel
BIOS checks the media for the presence of a "magic number" that distinguishes boot disks from ordinary ones: if bytes 511 and 512 in the first sector are equal to 0xAA55, then the disk is bootable.

All x86-compatible processors start their work in a primitive 16-bit mode, which is called "real mode" (real mode). Our loader switches the processor to 32-bit protected mode, converting the lower bit of the CR0 register to one. Therefore, the kernel starts loading already in 32-bit protected mode.

Keyboard is also supported using system interrupts.