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
		mov		ebx, [ebp + 8]	;parametre
		push	ebx	;parametre
		pop		eax
		call	iprintLF
		pop		eax;retour
		mov		[ebp + 12], eax	;retour
		pop	ebp
		ret
main:
		push	ebp
		mov		ebp, esp
		sub		esp, 4		;valeur retour
		push	10;constante
		call	procedure
		add		esp, 4
		pop	ebp
		ret
