GLOBAL getRegisters
GLOBAL getRAX
GLOBAL getRDI

section .text

getRAX:
	ret

getRDI:
	mov rax, rdi
	ret

getRegisters:
	push rbp
	mov rbp, rsp

	mov QWORD[rdi], rbx
	mov QWORD[rdi + 8], rcx
	mov QWORD[rdi + 16], rdx
	mov QWORD[rdi + 24], rsi
	push rbx
	mov rbx, [rbp]			; donde se encontraba antes de llamar a esta funcion
	mov QWORD[rdi + 32], rbx	; rbp antiguo se encuentra en stack
	mov rbx, rbp
	add rbx, 16
	mov QWORD[rdi + 40], rbx	; valor de rsp

	; calculo de RIP
	mov rbx, rbp
	mov rbx, [rbx+8]
	sub rbx, 49				; Tamanio que ocupan los llamados a funciones utilizadas previamente

	mov QWORD[rdi + 112], rbx	; RIP
	pop rbx

	mov QWORD[rdi + 48], r8
	mov QWORD[rdi + 56], r9
	mov QWORD[rdi + 64], r10
	mov QWORD[rdi + 72], r11
	mov QWORD[rdi + 80], r12
	mov QWORD[rdi + 88], r13
	mov QWORD[rdi + 96], r14
	mov QWORD[rdi + 104], r15

	mov rsp, rbp
	pop rbp
	ret