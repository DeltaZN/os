[bits 16]

switch_to_32:
	cli
	lgdt [gdt_descriptor] ; load global descriptor table

	mov eax, cr0
	or eax, 0x1
	mov cr0, eax

	jmp CODE_SEG:init_pm

[bits 32]
init_pm:
	;inc eax
	;jmp $
	mov ecx, DATA_SEG
;jmp $
	mov ds, ecx
	mov ss, ecx
	mov es, ecx
	mov fs, ecx
	mov gs, ecx

	mov ebp, 0x90000
	mov esp, ebp

	call BEGIN_PM
