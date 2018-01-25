%include		'io.asm'

section	.bss
sinput:	resb	255

section	.text
global _start
_start:
		call	main
		mov	eax, 1
		int	0x80
max:
		push	ebp
		mov		ebp, esp
		mov		ebx, [ebp + 12]	;parametre
		push	ebx	;parametre
		mov		ebx, [ebp + 8]	;parametre
		push	ebx	;parametre
		pop		ebx
		pop		eax
		cmp		eax, ebx
		jl		inst0
		jmp		inst1
inst0:
		mov		ebx, [ebp + 8]	;parametre
		push	ebx	;parametre
		pop		eax;retour
		mov		[ebp + 16], eax	;retour
		pop	ebp
		ret
		jmp		inst3
inst1:
		jmp		inst2
inst2:
		mov		ebx, [ebp + 12]	;parametre
		push	ebx	;parametre
		pop		eax;retour
		mov		[ebp + 16], eax	;retour
		pop	ebp
		ret
		pop		eax;retour
		mov		[ebp + 16], eax	;retour
		pop	ebp
		ret
main:
		push	ebp
		mov		ebp, esp
		sub		esp, 8	;variables locales
		mov		eax, sinput;lecture
		call	readline;lecture
		call	atoi;lecture
		push	eax;lecture
		pop		eax
		mov		[ebp - 4], eax
		mov		eax, [ebp - 4]
		mov		eax, sinput;lecture
		call	readline;lecture
		call	atoi;lecture
		push	eax;lecture
		pop		eax
		mov		[ebp - 8], eax
		mov		eax, [ebp - 8]
		sub		esp, 4		;valeur retour
		mov		eax, [ebp - 4]
		push	eax
		mov		eax, [ebp - 8]
		push	eax
		call	max
		add		esp, 8
		mov		eax, [ebp - 4]
		push	eax
		pop		ebx
		pop		eax
		cmp		eax, ebx
		je		inst3
		jmp		inst4
inst3:
		mov		eax, [ebp - 4]
		push	eax
		pop		eax
		call	iprintLF
		jmp		inst6
inst4:
		jmp		inst5;sinon
inst5:		;sinon
		mov		eax, [ebp - 8]
		push	eax
		pop		eax
		call	iprintLF
		jmp		inst6
inst6:
		add		esp, 8	;desalloue variables locales
		pop	ebp
		ret
