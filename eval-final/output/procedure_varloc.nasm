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
		sub		esp, 4	;variables locales
		push	1;constante
		pop		eax
		mov		[ebp - 4], eax
		mov		eax, [ebp - 4]
		mov		eax, [ebp - 4]
		push	eax
		pop		eax
		call	iprintLF
		add		esp, 4	;desalloue variables locales
		pop	ebp
		ret
main:
		push	ebp
		mov		ebp, esp
		sub		esp, 4		;valeur retour
		call	procedure
		pop	ebp
		ret
