%include	'io.asm'

section	.bss
sinput:	resb	255	;reserve a 255 byte space in memory for the users input string
v$i:	rest	1
v$j:	rest	1
v$k:	rest	1
v$somme:	rest	1

section	.text
global _start
_start:
	call	main
	mov	eax, 1		 ; 1 est le code de SYS_EXIT
	int	0x80		 ; exit
main:
	push	ebp		 ; sauvegarde la valeur de ebp
	mov	ebp, esp		 ; nouvelle valeur de ebp
	push	0
	pop	ebx
	mov	[v$somme], ebx		 ; stocke registre dans variable
	push	0
	pop	ebx
	mov	[v$i], ebx		 ; stocke registre dans variable
e0:
	mov	ebx, [v$i]		 ; lit variable dans ebx
	push	ebx
	push	10
	pop	ebx		 ; depile la seconde operande dans ebx
	pop	eax		 ; depile la permière operande dans eax
	cmp	eax, ebx
	jl	e2
	push	0
	jmp	e3
e2:
	push	1
e3:
	pop	eax
	cmp	eax, 00
	jz	e1
	push	0
	pop	ebx
	mov	[v$j], ebx		 ; stocke registre dans variable
e4:
	mov	ebx, [v$j]		 ; lit variable dans ebx
	push	ebx
	mov	ebx, [v$i]		 ; lit variable dans ebx
	push	ebx
	pop	ebx		 ; depile la seconde operande dans ebx
	pop	eax		 ; depile la permière operande dans eax
	cmp	eax, ebx
	jl	e6
	push	0
	jmp	e7
e6:
	push	1
e7:
	pop	eax
	cmp	eax, 00
	jz	e5
	push	0
	pop	ebx
	mov	[v$k], ebx		 ; stocke registre dans variable
e8:
	mov	ebx, [v$k]		 ; lit variable dans ebx
	push	ebx
	mov	ebx, [v$j]		 ; lit variable dans ebx
	push	ebx
	pop	ebx		 ; depile la seconde operande dans ebx
	pop	eax		 ; depile la permière operande dans eax
	cmp	eax, ebx
	jl	e10
	push	0
	jmp	e11
e10:
	push	1
e11:
	pop	eax
	cmp	eax, 00
	jz	e9
	mov	ebx, [v$somme]		 ; lit variable dans ebx
	push	ebx
	mov	ebx, [v$i]		 ; lit variable dans ebx
	push	ebx
	pop	ebx		 ; depile la seconde operande dans ebx
	pop	eax		 ; depile la permière operande dans eax
	add	eax, ebx		 ; effectue l'opération
	push	eax		 ; empile le résultat
	mov	ebx, [v$j]		 ; lit variable dans ebx
	push	ebx
	pop	ebx		 ; depile la seconde operande dans ebx
	pop	eax		 ; depile la permière operande dans eax
	add	eax, ebx		 ; effectue l'opération
	push	eax		 ; empile le résultat
	mov	ebx, [v$k]		 ; lit variable dans ebx
	push	ebx
	pop	ebx		 ; depile la seconde operande dans ebx
	pop	eax		 ; depile la permière operande dans eax
	add	eax, ebx		 ; effectue l'opération
	push	eax		 ; empile le résultat
	pop	ebx
	mov	[v$somme], ebx		 ; stocke registre dans variable
	mov	ebx, [v$k]		 ; lit variable dans ebx
	push	ebx
	push	1
	pop	ebx		 ; depile la seconde operande dans ebx
	pop	eax		 ; depile la permière operande dans eax
	add	eax, ebx		 ; effectue l'opération
	push	eax		 ; empile le résultat
	pop	ebx
	mov	[v$k], ebx		 ; stocke registre dans variable
	jmp	e8
e9:
	mov	ebx, [v$j]		 ; lit variable dans ebx
	push	ebx
	push	1
	pop	ebx		 ; depile la seconde operande dans ebx
	pop	eax		 ; depile la permière operande dans eax
	add	eax, ebx		 ; effectue l'opération
	push	eax		 ; empile le résultat
	pop	ebx
	mov	[v$j], ebx		 ; stocke registre dans variable
	jmp	e4
e5:
	mov	ebx, [v$i]		 ; lit variable dans ebx
	push	ebx
	push	1
	pop	ebx		 ; depile la seconde operande dans ebx
	pop	eax		 ; depile la permière operande dans eax
	add	eax, ebx		 ; effectue l'opération
	push	eax		 ; empile le résultat
	pop	ebx
	mov	[v$i], ebx		 ; stocke registre dans variable
	jmp	e0
e1:
	mov	ebx, [v$somme]		 ; lit variable dans ebx
	push	ebx
	pop	eax
	call	iprintLF
	pop	ebp		 ; restaure la valeur de ebp
	ret
