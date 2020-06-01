GLOBAL writePixel
GLOBAL read
GLOBAL emptyBuffer
GLOBAL getKey
GLOBAL drawChar
GLOBAL _hlt
GLOBAL getTime

section .text
writePixel:				; void writePixel( int x, int y, int rgb )
	push rbp
	mov rbp, rsp
	push rbx
	push rcx
	push rdx

	mov rax, 7			; numero de syscall sys_writePixel
	mov rbx, rdi		; primer parametro
	mov rcx, rsi
	;en rdx ya esta cargado el 3er parametro
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

read:					; int read( unsigned int fd, char *buffer, unsigned long count )
	push rbp
	mov rbp, rsp
	push rbx
	push rcx
	push rdx

	mov rax, 3			; numero de syscall sys_read
	mov rbx, rdi		; 1er parametro 
	mov rcx, rsi		; 2do parametro
	;en rdx ya esta cargado el 3er parametro
	int 80h

	pop rdx
	pop rcx
	pop rbx
	mov rsp, rbp
	pop rbp
	ret

emptyBuffer:			; void emptyBuffer()
	push rbp
	mov rbp, rsp
	push rax

	mov rax, 10
	int 80h

	pop rax
	mov rsp, rbp
	pop rbp
	ret

getKey:					; char getKey()
	push rbp
	mov rbp, rsp

	mov rax, 11
	int 80h

	mov rsp, rbp
	pop rbp
	ret

getTime:				; void getTime( TimeFormat *time )
	push rbp
	mov rbp, rsp

	mov rax, 12
	mov rbx, rdi
	int 80h

	mov rsp, rbp
	pop rbp
	ret

_hlt:
	sti
	hlt
	ret
	