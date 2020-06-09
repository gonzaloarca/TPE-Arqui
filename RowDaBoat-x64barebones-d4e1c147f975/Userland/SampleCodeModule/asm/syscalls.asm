GLOBAL write
GLOBAL read
GLOBAL changeWindow
GLOBAL changeWindowColor
GLOBAL emptyBuffer
GLOBAL _hlt
GLOBAL getTime
GLOBAL getCPUTemp
GLOBAL getRegisters
GLOBAL clrScreen
GLOBAL initProcess
GLOBAL runFirstProcess

section .text

; Estas aclaraciones sobre las funciones tambien se encuentran en "syscalls.h"

;-------------------------------------------------------
;	SYSCALL WRITE: RAX = 4
;			Imprime en la ventana actual los primeros count caracteres de buffer
;			si fd = 2, sera la salida de error y se imprimira en rojo, si es 1 en el color determinado por la ventana
;-------------------------------------------------------
; Llamada en C:
;	int write( unsigned int fd, char *buffer, unsigned long count )
;-------------------------------------------------------
write:
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

;-------------------------------------------------------
;	SYSCALL WRITE: RAX = 3
;			Lee de entrada estandar(la unica posible) el input del teclado hasta count caracteres
;-------------------------------------------------------
; Llamada en C:
;	int read( char *buffer, unsigned long count )
;-------------------------------------------------------
read:					; int read( char *buffer, unsigned long count )
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

;-------------------------------------------------------
;	SYSCALL WRITE: RAX = 7
;			Limpia la pantalla actual
;-------------------------------------------------------
; Llamada en C:
;	void clrScreen()
;-------------------------------------------------------
clrScreen:
	push rbp
	mov rbp, rsp
	push rax

	mov rax, 7
	int 80h

	pop rax
	mov rsp, rbp
	pop rbp
	ret

;-------------------------------------------------------
;	SYSCALL changeWindowColor: RAX = 9
;			Recibe por parametro el nuevo color de los caracteres de la ventana actual
;-------------------------------------------------------
; Llamada en C:
;	int changeWindowColor(int rgb)
;-------------------------------------------------------
changeWindowColor:
						; retorna 1 en exito, 0 caso contrario
	push rbp
	mov rbp, rsp
	push rbx

	mov rax, 9			; numero de syscall sys_changeWindowColor
	mov rbx, rdi		; 1er parametro
	int 80h

	pop rbx
	mov rsp, rbp
	pop rbp
	ret

;-------------------------------------------------------
;	SYSCALL emptyBuffer: RAX = 10
;			Vacia el buffer del teclado
;-------------------------------------------------------
; Llamada en C:
;	void emptyBuffer()
;-------------------------------------------------------
emptyBuffer:
	push rbp
	mov rbp, rsp
	push rax

	mov rax, 10
	int 80h

	pop rax
	mov rsp, rbp
	pop rbp
	ret

;-------------------------------------------------------
;	SYSCALL getTime: RAX = 12
;			Rellena la estructura con informacion del tiempo actual
;-------------------------------------------------------
; Llamada en C:
;	void getTime( TimeFormat *time )
;-------------------------------------------------------
getTime:
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

;-------------------------------------------------------
;	SYSCALL getTime: RAX = 13
;			Retorna la temperatura del cpu
;-------------------------------------------------------
; Llamada en C:
;	int getCPUTemp()
;-------------------------------------------------------
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

;-------------------------------------------------------
;	SYSCALL getTime: RAX = 14
;			Funcion que retorna una estructura con los valores de los registros de la ultima vez que se presiono la tecla especial F1
;-------------------------------------------------------
; Llamada en C:
;	RegistersType* getRegisters()
;-------------------------------------------------------
getRegisters:
	push rbp
	mov rbp, rsp

	mov rax, 14
	int 80h

	mov rsp, rbp
	pop rbp
	ret

;-------------------------------------------------------
;	SYSCALL initProcess: RAX = 21
;			Funcion para agregar un nuevo modulo a la lista de modulos
;-------------------------------------------------------
; Llamada en C:
;	int initProcess( void (*program)() )
;-------------------------------------------------------
initProcess:
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

;-------------------------------------------------------
;	SYSCALL initProcess: RAX = 23
;			Funcion que se encarga de correr el primer proceso en la cola, en caso de existir
;-------------------------------------------------------
; Llamada en C:
;	void runFirstProcess()
;-------------------------------------------------------
runFirstProcess:
	push rbp
	mov rbp, rsp
	push rax

	mov rax, 23
	int 80h

	pop rax
	mov rsp, rbp
	pop rbp
	ret
