%include		'io.asm'

section	.bss
sinput:	resb	255
$a	resw	4

section	.text
global _start
_start:
		call	main
		mov	eax, 1
		int	0x80
main:
		push	ebp
		mov		ebp, esp
		push	5;constante
		push	2;constante
		pop		ebx
		pop		eax
		imul	eax, ebx ;multiplication
		push	eax
		pop		eax
		call	iprintLF
		pop	ebp
		ret
