%include	'io.asm'

section	.bss
sinput:	resb	255	;reserve a 255 byte space in memory for the users input string

section	.text
global _start
_start:
	call	main
	mov	eax, 1		 ; 1 est le code de SYS_EXIT
	int	0x80		 ; exit
main:
	push	ebp		 ; sauvegarde la valeur de ebp
	mov	ebp, esp		 ; nouvelle valeur de ebp
	sub	esp, 16	; allocation variables locales
	mov	eax, sinput
	call	readline
	mov	eax, sinput
	call	atoi
	push	eax
	pop	ebx
	mov	[ebp - 4], ebx		 ; stocke registre dans variable
	push	0
	pop	ebx
	mov	[ebp - 8], ebx		 ; stocke registre dans variable
	mov	ebx, [ebp - 4]		 ; lit variable dans ebx
	push	ebx
	push	0
	pop	ebx		 ; depile la seconde operande dans ebx
	pop	eax		 ; depile la permière operande dans eax
	cmp	eax, ebx
	jl	e2
	push	0
	jmp	e3
e2:
	push	1
e3:
	mov	ebx, [ebp - 4]		 ; lit variable dans ebx
	push	ebx
	push	0
	pop	ebx		 ; depile la seconde operande dans ebx
	pop	eax		 ; depile la permière operande dans eax
	cmp	eax, ebx
	je	e4
	push	0
	jmp	e5
e4:
	push	1
e5:
	pop	ebx		 ; depile la seconde operande dans ebx
	pop	eax		 ; depile la permière operande dans eax
	cmp	eax, 0
	je	e6
	push	1
	jmp	e7
e6:
	push	0
e7:
	cmp	ebx, 0
	je	e8
	push	1
	jmp	e9
e8:
	push	0
e9:
	pop	ebx		 ; depile la seconde operande dans ebx
	pop	eax		 ; depile la permière operande dans eax
	add	eax, ebx
	cmp	eax, 0
	je	e10
	push	1
	jmp	e11
e10:
	push	0
e11:
	pop	eax
	cmp	eax, 00
	jz	e1
	push	1
	pop	ebx
	mov	[ebp - 8], ebx		 ; stocke registre dans variable
e1:
	push	0
	pop	ebx
	mov	[ebp - 12], ebx		 ; stocke registre dans variable
	mov	ebx, [ebp - 4]		 ; lit variable dans ebx
	push	ebx
	push	100
	pop	ebx		 ; depile la seconde operande dans ebx
	pop	eax		 ; depile la permière operande dans eax
	cmp	eax, ebx
	jl	e14
	push	0
	jmp	e15
e14:
	push	1
e15:
	push	0
	mov	ebx, [ebp - 4]		 ; lit variable dans ebx
	push	ebx
	pop	ebx		 ; depile la seconde operande dans ebx
	pop	eax		 ; depile la permière operande dans eax
	cmp	eax, ebx
	jl	e16
	push	0
	jmp	e17
e16:
	push	1
e17:
	pop	ebx		 ; depile la seconde operande dans ebx
	pop	eax		 ; depile la permière operande dans eax
	cmp	eax, 0
	je	e18
	push	1
	jmp	e19
e18:
	push	0
e19:
	cmp	ebx, 0
	je	e20
	push	1
	jmp	e21
e20:
	push	0
e21:
	pop	ebx		 ; depile la seconde operande dans ebx
	pop	eax		 ; depile la permière operande dans eax
	add	eax, ebx
	cmp	eax, 2
	jne	e22
	push	1
	jmp	e23
e22:
	push	0
e23:
	pop	eax
	cmp	eax, 00
	jz	e13
	push	1
	pop	ebx
	mov	[ebp - 12], ebx		 ; stocke registre dans variable
e13:
	push	0
	pop	ebx
	mov	[ebp - 16], ebx		 ; stocke registre dans variable
	mov	ebx, [ebp - 8]		 ; lit variable dans ebx
	push	ebx
	pop	eax		 ; depile l'operande dans eax
	cmp	eax, 0
	je	e26
	push	1
	jmp	e27
e26:
	push	0
e27:
	pop	eax
	cmp	eax, 0
	jne	e28
	push	1
	jmp	e29
e28:
	push	0
e29:
	mov	ebx, [ebp - 12]		 ; lit variable dans ebx
	push	ebx
	pop	eax		 ; depile l'operande dans eax
	cmp	eax, 0
	je	e30
	push	1
	jmp	e31
e30:
	push	0
e31:
	pop	eax
	cmp	eax, 0
	jne	e32
	push	1
	jmp	e33
e32:
	push	0
e33:
	pop	ebx		 ; depile la seconde operande dans ebx
	pop	eax		 ; depile la permière operande dans eax
	cmp	eax, 0
	je	e34
	push	1
	jmp	e35
e34:
	push	0
e35:
	cmp	ebx, 0
	je	e36
	push	1
	jmp	e37
e36:
	push	0
e37:
	pop	ebx		 ; depile la seconde operande dans ebx
	pop	eax		 ; depile la permière operande dans eax
	add	eax, ebx
	cmp	eax, 2
	jne	e38
	push	1
	jmp	e39
e38:
	push	0
e39:
	pop	eax
	cmp	eax, 00
	jz	e25
	push	1
	pop	ebx
	mov	[ebp - 16], ebx		 ; stocke registre dans variable
e25:
	mov	ebx, [ebp - 8]		 ; lit variable dans ebx
	push	ebx
	pop	eax
	call	iprintLF
	mov	ebx, [ebp - 12]		 ; lit variable dans ebx
	push	ebx
	pop	eax
	call	iprintLF
	mov	ebx, [ebp - 16]		 ; lit variable dans ebx
	push	ebx
	pop	eax
	call	iprintLF
	add	esp, 16	; desallocation variables locales
	pop	ebp		 ; restaure la valeur de ebp
	ret
