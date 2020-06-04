GLOBAL getRegisters
GLOBAL getRAX
GLOBAL getRDI
GLOBAL getTimeRTC
GLOBAL getMemoryASM

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

;-------------------------------------------------------
;	Regresa el tiempo actual en un puntero a estructura
;-------------------------------------------------------
; Llamada en C:
; void getTimeRTC(TimeFormat *time);
;-------------------------------------------------------
getTimeRTC:

	push rbp
	mov rbp, rsp

	push rax
	push rcx
	push rdx

	;	Antes de poder leer la hora, debo saber si hay una actualizacion en progreso o no
.updateCheck:
	mov rax, 0Ah
	out 70h, al
	in al, 71h
	shr al, 7				; El bit de upgrade in progress es el 8vo
	cmp al, 0
	jne .updateCheck		; Si el bit era 1, debo controlar de nuevo

	;	En al se guarda el dato PERO en formato feo:
	;	Primeros 4 bits son la decena
	;	Ultimos 4 bits son la unidad
	mov rax, 0
	out 70h, al
	in al, 71h
	mov cl, al
	shr al, 4				; En "al" queda la decena
	mov edx, 10
	mul edx					; En eax esta el resultado de eax*edx
	and cl, 15				; En "cl" queda la unidad
	add al, cl				; Los sumo
	mov DWORD [rdi], eax	;	quedan los segundos en eax

	mov rax, 2
	out 70h, al
	in al, 71h
	mov cl, al
	shr al, 4				; En "al" queda la decena
	mov edx, 10
	mul edx					; En eax esta el resultado de eax*edx
	and cl, 15				; En "cl" queda la unidad
	add al, cl				; Los sumo
	mov DWORD [rdi+4], eax	;	Minutos

	mov rax, 4
	out 70h, al
	in al, 71h
	mov cl, al
	shr al, 4				; En "al" queda la decena
	mov edx, 10
	mul edx					; En eax esta el resultado de eax*edx
	and cl, 15				; En "cl" queda la unidad
	add al, cl				; Los sumo
	mov DWORD [rdi+8], eax	;	Horas

	pop rdx
	pop rcx
	pop rax

	mov rsp, rbp
	pop rbp
	ret

getMemoryASM:
	push rbp
	mov rbp, rsp

	; en rdi se encuentra la estructura a retornar y en rsi la direccion
	; necesito imprimir 32 bytes

	mov rax, [rsi]
	mov QWORD[rdi], rax
	mov rax, [rsi + 8]		; avanzo 8 bytes
	mov QWORD[rdi + 8], rax
	mov rax, [rsi + 16]
	mov QWORD[rdi + 16], rax
	mov rax, [rsi + 24]
	mov QWORD[rdi + 24], rax

	mov rsp, rbp
	pop rbp
	ret