%include		'io.asm'

section	.bss
sinput:	resb	255

section	.text
global _start
_start:
		call	main
		mov	eax, 1
		int	0x80
fibo:
		push	ebp
		mov		ebp, esp
		mov		ebx, [ebp + 8]	;parametre
		push	ebx	;parametre
		push	2;constante
		pop		ebx
		pop		eax
		cmp		eax, ebx
		jl		inst0
		jmp		inst1
inst0:
		push	1;constante
		pop		eax;retour
		mov		[ebp + 12], eax	;retour
		pop	ebp
		ret
		jmp		inst3
inst1:
		jmp		inst2;sinon
inst2:		;sinon
		sub		esp, 4		;valeur retour
		mov		ebx, [ebp + 8]	;parametre
		push	ebx	;parametre
		push	1;constante
		pop		ebx
		pop		eax
		sub		eax, ebx
		push	eax
		call	fibo
		add		esp, 4
		sub		esp, 4		;valeur retour
		mov		ebx, [ebp + 8]	;parametre
		push	ebx	;parametre
		push	2;constante
		pop		ebx
		pop		eax
		sub		eax, ebx
		push	eax
		call	fibo
		add		esp, 4
		pop		ebx
		pop		eax
		add		eax, ebx
		push	eax
		pop		eax;retour
		mov		[ebp + 12], eax	;retour
		pop	ebp
		ret
		jmp		inst3
inst3:
		pop		eax;retour
		mov		[ebp + 12], eax	;retour
		pop	ebp
		ret
main:
		push	ebp
		mov		ebp, esp
		sub		esp, 4		;valeur retour
		mov		eax, sinput;lecture
		call	readline;lecture
		call	atoi;lecture
		push	eax;lecture
		call	fibo
		add		esp, 4
		pop		eax
		call	iprintLF
		pop	ebp
		ret
