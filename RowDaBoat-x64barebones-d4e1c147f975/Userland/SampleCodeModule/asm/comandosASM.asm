GLOBAL getTimeRTC
GLOBAL getMemoryASM
GLOBAL executeZeroException
GLOBAL executeUIException
section .text

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

; Lanza la excepcion de dividir por cero
executeZeroException:
	mov rax, 0
	div rax
	ret

; Lanza la excepcion de codigo invalido(undefined instruction)
executeUIException:
	UD2
	ret