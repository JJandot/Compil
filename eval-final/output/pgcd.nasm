%include		'io.asm'

section	.bss
sinput:	resb	255
$a	resw	4
$b	resw	4

section	.text
global _start
_start:
		call	main
		mov	eax, 1
		int	0x80
pgcd:
		push	ebp
		mov		ebp, esp
		sub		esp, 4	;variables locales
		mov		ebx, [ebp + 12]	;parametre
		push	ebx	;parametre
		mov		ebx, [ebp + 8]	;parametre
		push	ebx	;parametre
		pop		ebx
		pop		eax
		idiv		ebx
		push	eax
		mov		ebx, [ebp + 12]	;parametre
		push	ebx	;parametre
		mov		ebx, [ebp + 12]	;parametre
		push	ebx	;parametre
		mov		ebx, [ebp + 8]	;parametre
		push	ebx	;parametre
		pop		ebx
		pop		eax
		idiv		ebx
		push	eax
		mov		ebx, [ebp + 8]	;parametre
		push	ebx	;parametre
		pop		ebx
		pop		eax
		imul	eax, ebx ;multiplication
		push	eax
		pop		ebx
		pop		eax
		sub		eax, ebx
		push	eax
		pop		eax
		mov		[ebp - 4], eax
		mov		eax, [ebp - 4]
		mov		eax, [ebp - 4]
		push	eax
		push	0;constante
		pop		ebx
		pop		eax
		cmp		eax, ebx
		je		inst0
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
		jmp		inst2;sinon
inst2:		;sinon
		sub		esp, 4		;valeur retour
		mov		ebx, [ebp + 8]	;parametre
		push	ebx	;parametre
		mov		eax, [ebp - 4]
		push	eax
		call	pgcd
		add		esp, 8
		pop		eax;retour
		mov		[ebp + 16], eax	;retour
		pop	ebp
		ret
		jmp		inst3
inst3:
		add		esp, 4	;desalloue variables locales
		pop		eax;retour
		mov		[ebp + 16], eax	;retour
		pop	ebp
		ret
main:
		push	ebp
		mov		ebp, esp
		mov		eax, sinput;lecture
		call	readline;lecture
		call	atoi;lecture
		push	eax;lecture
		pop		eax
		mov		[$a], eax;affectVar
		mov		eax, [$a];affectVar
		mov		eax, sinput;lecture
		call	readline;lecture
		call	atoi;lecture
		push	eax;lecture
		pop		eax
		mov		[$b], eax;affectVar
		mov		eax, [$b];affectVar
		mov		eax, [$b];traitement exp
		push	eax
		mov		eax, [$a];traitement exp
		push	eax
		pop		ebx
		pop		eax
		cmp		eax, ebx
		jl		inst4
		jmp		inst5
inst4:
		sub		esp, 4		;valeur retour
		mov		eax, [$a];traitement exp
		push	eax
		mov		eax, [$b];traitement exp
		push	eax
		call	pgcd
		add		esp, 8
		pop		eax
		call	iprintLF
		jmp		inst7
inst5:
		jmp		inst6;sinon
inst6:		;sinon
		sub		esp, 4		;valeur retour
		mov		eax, [$b];traitement exp
		push	eax
		mov		eax, [$a];traitement exp
		push	eax
		call	pgcd
		add		esp, 8
		pop		eax
		call	iprintLF
		jmp		inst7
inst7:
		pop	ebp
		ret
