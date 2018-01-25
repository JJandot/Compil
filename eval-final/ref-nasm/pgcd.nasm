%include	'io.asm'

section	.bss
sinput:	resb	255	;reserve a 255 byte space in memory for the users input string
v$a:	rest	1
v$b:	rest	1

section	.text
global _start
_start:
	call	main
	mov	eax, 1		 ; 1 est le code de SYS_EXIT
	int	0x80		 ; exit
pgcd:
	push	ebp		 ; sauvegarde la valeur de ebp
	mov	ebp, esp		 ; nouvelle valeur de ebp
	sub	esp, 4	; allocation variables locales
	mov	ebx, [ebp + 12]		 ; lit variable dans ebx
	push	ebx
	mov	ebx, [ebp + 12]		 ; lit variable dans ebx
	push	ebx
	mov	ebx, [ebp + 8]		 ; lit variable dans ebx
	push	ebx
	pop	ebx		 ; depile la seconde operande dans ebx
	pop	eax		 ; depile la permière operande dans eax
	mov	edx, 0		 ; initialise edx à zéro
	div	ebx		 ; effectue l'opération
	push	eax		 ; empile le résultat
	mov	ebx, [ebp + 8]		 ; lit variable dans ebx
	push	ebx
	pop	ebx		 ; depile la seconde operande dans ebx
	pop	eax		 ; depile la permière operande dans eax
	mul	ebx		 ; effectue l'opération
	push	eax		 ; empile le résultat
	pop	ebx		 ; depile la seconde operande dans ebx
	pop	eax		 ; depile la permière operande dans eax
	sub	eax, ebx		 ; effectue l'opération
	push	eax		 ; empile le résultat
	pop	ebx
	mov	[ebp - 4], ebx		 ; stocke registre dans variable
	mov	ebx, [ebp - 4]		 ; lit variable dans ebx
	push	ebx
	push	0
	pop	ebx		 ; depile la seconde operande dans ebx
	pop	eax		 ; depile la permière operande dans eax
	cmp	eax, ebx
	je	e2
	push	0
	jmp	e3
e2:
	push	1
e3:
	pop	eax
	cmp	eax, 00
	jz	e0
	mov	ebx, [ebp + 8]		 ; lit variable dans ebx
	push	ebx
	pop	eax
	mov	[ebp + 16], eax		 ; ecriture de la valeur de retour
	add	esp, 4	; desallocation variables locales
	pop	ebp		 ; restaure la valeur de ebp
	ret
	jmp	e1
e0:
	sub	esp, 4		 ; allocation valeur de retour
				; empile arg 0
	mov	ebx, [ebp + 8]		 ; lit variable dans ebx
	push	ebx
				; empile arg 1
	mov	ebx, [ebp - 4]		 ; lit variable dans ebx
	push	ebx
	call	pgcd
	add	esp, 8		; desallocation parametres
	pop	eax
	mov	[ebp + 16], eax		 ; ecriture de la valeur de retour
	add	esp, 4	; desallocation variables locales
	pop	ebp		 ; restaure la valeur de ebp
	ret
e1:
	add	esp, 4	; desallocation variables locales
	pop	ebp		 ; restaure la valeur de ebp
	ret
main:
	push	ebp		 ; sauvegarde la valeur de ebp
	mov	ebp, esp		 ; nouvelle valeur de ebp
	mov	eax, sinput
	call	readline
	mov	eax, sinput
	call	atoi
	push	eax
	pop	ebx
	mov	[v$a], ebx		 ; stocke registre dans variable
	mov	eax, sinput
	call	readline
	mov	eax, sinput
	call	atoi
	push	eax
	pop	ebx
	mov	[v$b], ebx		 ; stocke registre dans variable
	mov	ebx, [v$b]		 ; lit variable dans ebx
	push	ebx
	mov	ebx, [v$a]		 ; lit variable dans ebx
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
	jz	e4
	sub	esp, 4		 ; allocation valeur de retour
				; empile arg 0
	mov	ebx, [v$a]		 ; lit variable dans ebx
	push	ebx
				; empile arg 1
	mov	ebx, [v$b]		 ; lit variable dans ebx
	push	ebx
	call	pgcd
	add	esp, 8		; desallocation parametres
	pop	eax
	call	iprintLF
	jmp	e5
e4:
	sub	esp, 4		 ; allocation valeur de retour
				; empile arg 0
	mov	ebx, [v$b]		 ; lit variable dans ebx
	push	ebx
				; empile arg 1
	mov	ebx, [v$a]		 ; lit variable dans ebx
	push	ebx
	call	pgcd
	add	esp, 8		; desallocation parametres
	pop	eax
	call	iprintLF
e5:
	pop	ebp		 ; restaure la valeur de ebp
	ret
