GLOBAL writePixel
GLOBAL drawChar

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

drawChar:
	push rbp
	mov rbp, rsp
	push rbx
	push rcx
	push rdx

	mov rax, 8			; numero de syscall writePixel
	mov rbx, rdi		; primer parametro
	push rcx			; es el cuarto parametro, debe estar en rsi
	mov rcx, rsi		; 2do parametro
	mov rdx, rdx		; 3er parametro
	pop rsi				; 4to parametro
	mov rdi, r8			; 5to parametro
	int 80h

	pop rdx
	pop rcx
	pop rbx
	mov rsp, rbp
	pop rbp
	ret