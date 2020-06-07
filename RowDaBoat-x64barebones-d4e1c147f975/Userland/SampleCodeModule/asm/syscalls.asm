GLOBAL write
GLOBAL read
GLOBAL changeWindow
GLOBAL changeWindowColor
GLOBAL emptyBuffer
GLOBAL _hlt
GLOBAL getTime
GLOBAL getCPUTemp
GLOBAL getCPUInfo
GLOBAL getRegisters
GLOBAL clrScreen
GLOBAL initProcess
GLOBAL runFirstProcess

section .text

write:					; int write( unsigned int fd, char *buffer, unsigned long count )
	push rbp
	mov rbp, rsp
	push rbx
	push rcx
	push rdx

	mov rax, 4			; numero de syscall sys_writePixel
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

read:					; int read( unsigned int fd, char *buffer, unsigned long count )
	push rbp
	mov rbp, rsp
	push rbx
	push rcx
	push rdx

	mov rax, 3			; numero de syscall sys_read
	mov rbx, rdi		; 1er parametro 
	mov rcx, rsi		; 2do parametro
	int 80h

	pop rdx
	pop rcx
	pop rbx
	mov rsp, rbp
	pop rbp
	ret

clrScreen:				;void clrScreen()
	push rbp
	mov rbp, rsp
	push rax

	mov rax, 7
	int 80h

	pop rax
	mov rsp, rbp
	pop rbp
	ret

changeWindow:			; int changeWindow(unsigned int window)
						; retorna 1 en exito, 0 caso contrario
	push rbp
	mov rbp, rsp
	push rbx

	mov rax, 8			
	mov rbx, rdi		; 1er parametro
	int 80h

	pop rbx
	mov rsp, rbp
	pop rbp
	ret

changeWindowColor:		; int changeWindowColor(int rgb)
						; retorna 1 en exito, 0 caso contrario
	push rbp
	mov rbp, rsp
	push rbx

	mov rax, 9			; numero de syscall sys_read
	mov rbx, rdi		; 1er parametro
	int 80h

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

getTime:				; void getTime( TimeFormat *time )
	push rbp
	mov rbp, rsp
	push rax

	mov rax, 12
	mov rbx, rdi
	int 80h

	pop rax
	mov rsp, rbp
	pop rbp
	ret

getCPUTemp:				; int getCPUTemp()
	push rbp
	mov rbp, rsp
	
	mov rax, 13
	int 80h

	mov rsp, rbp
	pop rbp
	ret

_hlt:
	sti
	hlt
	ret

getCPUInfo:
	push rbp
	mov rbp, rsp

	mov rax, 20
	mov rbx, rdi
	int 80h

	mov rsp, rbp
	pop rbp
	ret
	
getRegisters:
	push rbp
	mov rbp, rsp

	mov rax, 14
	int 80h

	mov rsp, rbp
	pop rbp
	ret

initProcess:		;int initProcess( void (*program)() )
	push rbp
	mov rbp, rsp
	push rbx

	mov rax, 21
	mov rbx, rdi
	int 80h

	pop rbx
	mov rsp, rbp
	pop rbp
	ret

runFirstProcess:	;void runFirstProcess()
	push rbp
	mov rbp, rsp
	push rax

	mov rax, 23
	int 80h

	pop rax
	mov rsp, rbp
	pop rbp
	ret
