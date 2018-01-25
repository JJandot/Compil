%include	'io.asm'

section	.bss
sinput:	resb	255	;reserve a 255 byte space in memory for the users input string

section	.text
global _start
_start:
	call	main
	mov	eax, 1		 ; 1 est le code de SYS_EXIT
	int	0x80		 ; exit
procedure:
	push	ebp		 ; sauvegarde la valeur de ebp
	mov	ebp, esp		 ; nouvelle valeur de ebp
	sub	esp, 4	; allocation variables locales
	push	1
	pop	ebx
	mov	[ebp - 4], ebx		 ; stocke registre dans variable
	mov	ebx, [ebp - 4]		 ; lit variable dans ebx
	push	ebx
	pop	eax
	call	iprintLF
	add	esp, 4	; desallocation variables locales
	pop	ebp		 ; restaure la valeur de ebp
	ret
main:
	push	ebp		 ; sauvegarde la valeur de ebp
	mov	ebp, esp		 ; nouvelle valeur de ebp
	sub	esp, 4		 ; allocation valeur de retour
	call	procedure
	add	esp, 4		 ; valeur de retour ignoree
	pop	ebp		 ; restaure la valeur de ebp
	ret
