%include		'io.asm'

section	.bss
sinput:	resb	255
$i	resw	4
$carre	resw	4

section	.text
global _start
_start:
		call	main
		mov	eax, 1
		int	0x80
main:
		push	ebp
		mov		ebp, esp
		push	0;constante
		pop		eax
		mov		[$i], eax;affectVar
		mov		eax, [$i];affectVar
inst2:
		mov		eax, [$i];traitement exp
		push	eax
		push	10;constante
		pop		ebx
		pop		eax
		cmp		eax, ebx
		jl		inst0
		jmp		inst1
inst0:
		mov		eax, [$i];traitement exp
		push	eax
		mov		eax, [$i];traitement exp
		push	eax
		pop		ebx
		pop		eax
		imul	eax, ebx ;multiplication
		push	eax
		pop		eax
		mov		[$carre], eax;affectVar
		mov		eax, [$carre];affectVar
		mov		eax, [$carre];traitement exp
		push	eax
		pop		eax
		call	iprintLF
		mov		eax, [$i];traitement exp
		push	eax
		push	1;constante
		pop		ebx
		pop		eax
		add		eax, ebx
		push	eax
		pop		eax
		mov		[$i], eax;affectVar
		mov		eax, [$i];affectVar
		jmp		inst2
inst1:
		pop	ebp
		ret
