GLOBAL writePixel

section .text
writePixel:
	push rbp
	mov rbp, rsp
	push rbx
	push rcx
	push rdx

	mov rax, 7			; numero de syscall writePixel
	mov rbx, [rbp+16]	; primer parametro
	mov rcx, [rbp+24]
	mov rdx, [rbp+32]
	int 80h

	pop rdx
	pop rcx
	pop rbx
	mov rsp, rbp
	pop rbp
	ret