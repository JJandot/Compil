%include		'io.asm'

section	.bss
sinput:	resb	255
$tab	resw	40

section	.text
global _start
_start:
		call	main
		mov	eax, 1
		int	0x80
initialiser:
		push	ebp
		mov		ebp, esp
		push	0;constante
		push	8;constante
		pop		eax
		mov		[$tab], eax;affectVar
		mov		eax, [$tab];affectVar
		push	1;constante
		push	6;constante
		pop		eax
		mov		[$tab], eax;affectVar
		mov		eax, [$tab];affectVar
		push	2;constante
		push	9;constante
		pop		eax
		mov		[$tab], eax;affectVar
		mov		eax, [$tab];affectVar
		push	3;constante
		push	9;constante
		pop		eax
		mov		[$tab], eax;affectVar
		mov		eax, [$tab];affectVar
		push	4;constante
		push	4;constante
		pop		eax
		mov		[$tab], eax;affectVar
		mov		eax, [$tab];affectVar
		push	5;constante
		push	2;constante
		pop		eax
		mov		[$tab], eax;affectVar
		mov		eax, [$tab];affectVar
		push	6;constante
		push	3;constante
		pop		eax
		mov		[$tab], eax;affectVar
		mov		eax, [$tab];affectVar
		push	7;constante
		push	1;constante
		pop		eax
		mov		[$tab], eax;affectVar
		mov		eax, [$tab];affectVar
		push	8;constante
		push	4;constante
		pop		eax
		mov		[$tab], eax;affectVar
		mov		eax, [$tab];affectVar
		push	9;constante
		push	5;constante
		pop		eax
		mov		[$tab], eax;affectVar
		mov		eax, [$tab];affectVar
		pop	ebp
		ret
afficher:
		push	ebp
		mov		ebp, esp
		sub		esp, 4	;variables locales
		push	0;constante
		pop		eax
		mov		[ebp - 4], eax
		mov		eax, [ebp - 4]
inst2:
		mov		eax, [ebp - 4]
		push	eax
		mov		ebx, [ebp + 8]	;parametre
		push	ebx	;parametre
		pop		ebx
		pop		eax
		cmp		eax, ebx
		jl		inst0
		jmp		inst1
inst0:
		mov		eax, [ebp - 4]
		push	eax
		mov		eax, [$tab];traitement exp
		push	eax
		pop		eax
		call	iprintLF
		mov		eax, [ebp - 4]
		push	eax
		push	1;constante
		pop		ebx
		pop		eax
		add		eax, ebx
		push	eax
		pop		eax
		mov		[ebp - 4], eax
		mov		eax, [ebp - 4]
		jmp		inst2
inst1:
		push	0;constante
		pop		eax
		call	iprintLF
		add		esp, 4	;desalloue variables locales
		pop		eax;retour
		mov		[ebp + 12], eax	;retour
		pop	ebp
		ret
echanger:
		push	ebp
		mov		ebp, esp
		sub		esp, 4	;variables locales
		mov		ebx, [ebp + 8]	;parametre
		push	ebx	;parametre
		mov		eax, [$tab];traitement exp
		push	eax
		pop		eax
		mov		[ebp - 4], eax
		mov		eax, [ebp - 4]
		mov		ebx, [ebp + 8]	;parametre
		push	ebx	;parametre
		mov		ebx, [ebp + 12]	;parametre
		push	ebx	;parametre
		mov		eax, [$tab];traitement exp
		push	eax
		pop		eax
		mov		[$tab], eax;affectVar
		mov		eax, [$tab];affectVar
		mov		ebx, [ebp + 12]	;parametre
		push	ebx	;parametre
		mov		eax, [ebp - 4]
		push	eax
		pop		eax
		mov		[$tab], eax;affectVar
		mov		eax, [$tab];affectVar
		add		esp, 4	;desalloue variables locales
		pop		eax;retour
		mov		[ebp + 16], eax	;retour
		pop	ebp
		ret
trier:
		push	ebp
		mov		ebp, esp
		sub		esp, 12	;variables locales
		mov		ebx, [ebp + 8]	;parametre
		push	ebx	;parametre
		pop		eax
		mov		[ebp - 12], eax
		mov		eax, [ebp - 12]
		push	1;constante
		pop		eax
		mov		[ebp - 4], eax
		mov		eax, [ebp - 4]
inst2:
		mov		eax, [ebp - 4]
		push	eax
		push	1;constante
		pop		ebx
		pop		eax
		cmp		eax, ebx
		je		inst0
		jmp		inst1
inst0:
		push	0;constante
		pop		eax
		mov		[ebp - 4], eax
		mov		eax, [ebp - 4]
		push	0;constante
		pop		eax
		mov		[ebp - 8], eax
		mov		eax, [ebp - 8]
inst2:
		mov		eax, [ebp - 8]
		push	eax
		mov		eax, [ebp - 12]
		push	eax
		push	1;constante
		pop		ebx
		pop		eax
		sub		eax, ebx
		push	eax
		pop		ebx
		pop		eax
		cmp		eax, ebx
		jl		inst0
		jmp		inst1
inst0:
		mov		eax, [ebp - 8]
		push	eax
		push	1;constante
		pop		ebx
		pop		eax
		add		eax, ebx
		push	eax
		mov		eax, [ebp - 8]
		push	eax
		mov		eax, [$tab];traitement exp
		push	eax
		mov		eax, [$tab];traitement exp
		push	eax
		pop		ebx
		pop		eax
		cmp		eax, ebx
		jl		inst0
		jmp		inst1
inst0:
		sub		esp, 4		;valeur retour
		mov		eax, [ebp - 8]
		push	eax
		mov		eax, [ebp - 8]
		push	eax
		push	1;constante
		pop		ebx
		pop		eax
		add		eax, ebx
		push	eax
		call	echanger
		add		esp, 8
		push	1;constante
		pop		eax
		mov		[ebp - 4], eax
		mov		eax, [ebp - 4]
		jmp		inst3
inst1:
		jmp		inst2
inst2:
		mov		eax, [ebp - 8]
		push	eax
		push	1;constante
		pop		ebx
		pop		eax
		add		eax, ebx
		push	eax
		pop		eax
		mov		[ebp - 8], eax
		mov		eax, [ebp - 8]
		jmp		inst5
inst4:
		mov		eax, [ebp - 12]
		push	eax
		push	1;constante
		pop		ebx
		pop		eax
		sub		eax, ebx
		push	eax
		pop		eax
		mov		[ebp - 12], eax
		mov		eax, [ebp - 12]
		jmp		inst5
inst4:
		add		esp, 12	;desalloue variables locales
		pop		eax;retour
		mov		[ebp + 12], eax	;retour
		pop	ebp
		ret
main:
		push	ebp
		mov		ebp, esp
		sub		esp, 4		;valeur retour
		call	initialiser
		sub		esp, 4		;valeur retour
		push	10;constante
		call	afficher
		add		esp, 4
		sub		esp, 4		;valeur retour
		push	10;constante
		call	trier
		add		esp, 4
		sub		esp, 4		;valeur retour
		push	10;constante
		call	afficher
		add		esp, 4
		pop	ebp
		ret
