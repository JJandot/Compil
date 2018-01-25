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
	mov	ebx, [ebp + 8]		 ; lit variable dans ebx
	push	ebx
	pop	eax
	call	iprintLF
	pop	ebp		 ; restaure la valeur de ebp
	ret
main:
	push	ebp		 ; sauvegarde la valeur de ebp
	mov	ebp, esp		 ; nouvelle valeur de ebp
	sub	esp, 4		 ; allocation valeur de retour
				; empile arg 0
	push	10
	call	procedure
	add	esp, 4		; desallocation parametres
	add	esp, 4		 ; valeur de retour ignoree
	pop	ebp		 ; restaure la valeur de ebp
	ret
