bits 16
org 0x7C00

KERNEL_OFFSET equ 0x1000 ; offset to reach main func in kernel.c
boot:
	mov [BOOT_DRIVE], dl ; store boot drive in DL

	mov bp, 0x9000
	mov sp, bp

	mov bx, MSG_REAL_MODE
	call print_string

	call load_kernel

	call switch_to_32

	jmp $

%include "print_string.asm"
%include "disk_load.asm"
%include "gdt.asm"
%include "print_string_pm.asm"
%include "switch_to_32.asm"

bits 16

load_kernel:
	;mov bx, MSG_LOAD_KERNEL
	;call print_string

	mov bx, KERNEL_OFFSET ; destination
	mov dh, 2 ; 2 sectors of drive
	mov dl, [BOOT_DRIVE]
	call disk_load

	mov bx, MSG_LOAD_KERNEL
	call print_string

	ret

bits 32

BEGIN_PM:
	mov ebx, MSG_PROT_MODE
	call print_string_pm

	jmp KERNEL_OFFSET

	jmp $

BOOT_DRIVE	db 0
MSG_REAL_MODE	db "Starting 16-bit real mode", 0
MSG_PROT_MODE	db "Swithed to 32-bit Protected mode!!!!!!!!!!!!!!!!!!", 0
MSG_LOAD_KERNEL	db "Loading kernel!!!", 0

times 510-($-$$) db 0
dw 0xAA55
