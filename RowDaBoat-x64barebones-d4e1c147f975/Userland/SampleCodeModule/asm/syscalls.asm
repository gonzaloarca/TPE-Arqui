GLOBAL writePixel

section .text
writePixel:
	push rbp
	mov rbp, rsp
	push rbx
	push rcx
	push rdx

	mov rax, 7			; numero de syscall writePixel
	mov rbx, rdi		; primer parametro
	mov rcx, rsi
	mov rdx, rdx
	int 80h

	pop rdx
	pop rcx
	pop rbx
	mov rsp, rbp
	pop rbp
	ret