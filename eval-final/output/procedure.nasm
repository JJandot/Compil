%include		'io.asm'

section	.bss
sinput:	resb	255

section	.text
global _start
_start:
		call	main
		mov	eax, 1
		int	0x80
procedure:
		push	ebp
		mov		ebp, esp
		push	1;constante
		pop		eax
		call	iprintLF
		pop	ebp
		ret
main:
		push	ebp
		mov		ebp, esp
		sub		esp, 4		;valeur retour
		call	procedure
		pop	ebp
		ret
