GLOBAL getRegisters

section .text
getRegisters:
	push rbp
	mov rbp, rsp

	mov QWORD[rsp + 32], rax	; PREGUNTAR POR QUE DEBO GUARDAR A PARTIR DE rsp + 32 si la estructura arranca en rsp +16
	lea rax, [rsp + 16]		; son 16 registros de 64bits(8 bytes) cada uno
	mov QWORD[rax + 24], rbx
	mov QWORD[rax + 32], rcx
	mov QWORD[rax + 40], rdx
	mov QWORD[rax + 48], rsi
	push rbx
	mov rbx, rbp
	add rbx, 24					; donde se encontraba antes de llamar a esta funcion
	mov QWORD[rax + 56], rbx
	mov QWORD[rax + 64], rbx	; antes de llamar a la funcion rsp = rbp
	pop rbx
	mov QWORD[rax + 72], r8
	mov QWORD[rax + 80], r9
	mov QWORD[rax + 88], r10
	mov QWORD[rax + 96], r11
	mov QWORD[rax + 104], r12
	mov QWORD[rax + 112], r13
	mov QWORD[rax + 120], r14
	mov QWORD[rax + 128], r15


	mov rsp, rbp
	pop rbp
	ret