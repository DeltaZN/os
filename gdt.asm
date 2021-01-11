gdt_start:
gdt_null:
	dd 0x0
	dd 0x0

gdt_code:
	; base=0x0, limit=0xfffff
	; 1st flags: (present)1 (privilege)00 (descr type)1 -> 1001b
	; type flags: (code)1 (conforming)0 (readable)1 (accessed)0 -> 1010b
	; 2nd flags: (granularity)1 (32-bit default)1 (64-bit seg)0 (AVL)0 -> 1100b
	dw 0xfffff	; Limit 0-15
	dw 0x0		; Base 0-15
	db 0x0		; Base 16-23
	db 10011010b	; 1st flag, type flag
	db 11001111b	; 2nd flags, Limit 16-19
	db 0x0

gdt_data:
	;type flags: (code)0 (expand down)0 (writable)1 (accessed)0 -> 0010b
	dw 0xfffff
	dw 0x0
	dw 0x0
	db 10010010b
	db 11001111b
	db 0x0

gdt_end:

gdt_descriptor:
	dw gdt_end - gdt_start - 1
	dd gdt_start

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start
