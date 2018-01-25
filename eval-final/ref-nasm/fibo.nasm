%include	'io.asm'

section	.bss
sinput:	resb	255	;reserve a 255 byte space in memory for the users input string

section	.text
global _start
_start:
	call	main
	mov	eax, 1		 ; 1 est le code de SYS_EXIT
	int	0x80		 ; exit
fibo:
	push	ebp		 ; sauvegarde la valeur de ebp
	mov	ebp, esp		 ; nouvelle valeur de ebp
	mov	ebx, [ebp + 8]		 ; lit variable dans ebx
	push	ebx
	push	2
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
	jz	e0
	push	1
	pop	eax
	mov	[ebp + 12], eax		 ; ecriture de la valeur de retour
	pop	ebp		 ; restaure la valeur de ebp
	ret
	jmp	e1
e0:
	sub	esp, 4		 ; allocation valeur de retour
				; empile arg 0
	mov	ebx, [ebp + 8]		 ; lit variable dans ebx
	push	ebx
	push	1
	pop	ebx		 ; depile la seconde operande dans ebx
	pop	eax		 ; depile la permière operande dans eax
	sub	eax, ebx		 ; effectue l'opération
	push	eax		 ; empile le résultat
	call	fibo
	add	esp, 4		; desallocation parametres
	sub	esp, 4		 ; allocation valeur de retour
				; empile arg 0
	mov	ebx, [ebp + 8]		 ; lit variable dans ebx
	push	ebx
	push	2
	pop	ebx		 ; depile la seconde operande dans ebx
	pop	eax		 ; depile la permière operande dans eax
	sub	eax, ebx		 ; effectue l'opération
	push	eax		 ; empile le résultat
	call	fibo
	add	esp, 4		; desallocation parametres
	pop	ebx		 ; depile la seconde operande dans ebx
	pop	eax		 ; depile la permière operande dans eax
	add	eax, ebx		 ; effectue l'opération
	push	eax		 ; empile le résultat
	pop	eax
	mov	[ebp + 12], eax		 ; ecriture de la valeur de retour
	pop	ebp		 ; restaure la valeur de ebp
	ret
e1:
	pop	ebp		 ; restaure la valeur de ebp
	ret
main:
	push	ebp		 ; sauvegarde la valeur de ebp
	mov	ebp, esp		 ; nouvelle valeur de ebp
	sub	esp, 4		 ; allocation valeur de retour
				; empile arg 0
	mov	eax, sinput
	call	readline
	mov	eax, sinput
	call	atoi
	push	eax
	call	fibo
	add	esp, 4		; desallocation parametres
	pop	eax
	call	iprintLF
	pop	ebp		 ; restaure la valeur de ebp
	ret
