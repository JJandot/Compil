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
		push	123;constante
		pop		eax
		mov		[$a], eax;affectVar
		mov		eax, [$a];affectVar
		pop	ebp
		ret
