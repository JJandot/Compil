%include		'io.asm'

section	.bss
sinput:	resb	255
$i	resw	4
$j	resw	4
$k	resw	4
$somme	resw	4

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
		mov		[$somme], eax;affectVar
		mov		eax, [$somme];affectVar
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
		push	0;constante
		pop		eax
		mov		[$j], eax;affectVar
		mov		eax, [$j];affectVar
inst2:
		mov		eax, [$j];traitement exp
		push	eax
		mov		eax, [$i];traitement exp
		push	eax
		pop		ebx
		pop		eax
		cmp		eax, ebx
		jl		inst0
		jmp		inst1
inst0:
		push	0;constante
		pop		eax
		mov		[$k], eax;affectVar
		mov		eax, [$k];affectVar
inst2:
		mov		eax, [$k];traitement exp
		push	eax
		mov		eax, [$j];traitement exp
		push	eax
		pop		ebx
		pop		eax
		cmp		eax, ebx
		jl		inst0
		jmp		inst1
inst0:
		mov		eax, [$somme];traitement exp
		push	eax
		mov		eax, [$i];traitement exp
		push	eax
		pop		ebx
		pop		eax
		add		eax, ebx
		push	eax
		mov		eax, [$j];traitement exp
		push	eax
		pop		ebx
		pop		eax
		add		eax, ebx
		push	eax
		mov		eax, [$k];traitement exp
		push	eax
		pop		ebx
		pop		eax
		add		eax, ebx
		push	eax
		pop		eax
		mov		[$somme], eax;affectVar
		mov		eax, [$somme];affectVar
		mov		eax, [$k];traitement exp
		push	eax
		push	1;constante
		pop		ebx
		pop		eax
		add		eax, ebx
		push	eax
		pop		eax
		mov		[$k], eax;affectVar
		mov		eax, [$k];affectVar
		jmp		inst2
inst1:
		mov		eax, [$j];traitement exp
		push	eax
		push	1;constante
		pop		ebx
		pop		eax
		add		eax, ebx
		push	eax
		pop		eax
		mov		[$j], eax;affectVar
		mov		eax, [$j];affectVar
		jmp		inst2
inst1:
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
		mov		eax, [$somme];traitement exp
		push	eax
		pop		eax
		call	iprintLF
		pop	ebp
		ret
