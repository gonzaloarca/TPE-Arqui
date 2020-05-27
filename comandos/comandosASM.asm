GLOBAL getRegisters

section .text
getRegisters:
	push rbp
	mov rbp, rsp

	mov QWORD[rbp + 32], rax	; Arranca en +32 pues antes hay 4 datos de 64b que son(en orden ascendente): rbp, canary, retorno y ????
	lea rax, [rbp + 32]			; son 16 registros de 64bits(8 bytes) cada uno
	mov QWORD[rax + 8], rbx
	mov QWORD[rax + 16], rcx
	mov QWORD[rax + 24], rdx
	mov QWORD[rax + 32], rsi
	mov QWORD[rax + 40], rdi
	push rbx
	mov rbx, rbp
	add rbx, 24					; donde se encontraba antes de llamar a esta funcion
	mov QWORD[rax + 48], rbx
	mov QWORD[rax + 56], rbx	; antes de llamar a la funcion rsp = rbp
	pop rbx
	mov QWORD[rax + 64], r8
	mov QWORD[rax + 72], r9
	mov QWORD[rax + 80], r10
	mov QWORD[rax + 88], r11
	mov QWORD[rax + 96], r12
	mov QWORD[rax + 104], r13
	mov QWORD[rax + 112], r14
	mov QWORD[rax + 120], r15


	mov rsp, rbp
	pop rbp
	ret