GLOBAL cpuVendor
GLOBAL getTimeRTC
GLOBAL canReadKey
GLOBAL getScanCode
GLOBAL int80htest

section .text
	
cpuVendor:
	push rbp
	mov rbp, rsp

	push rbx

	mov rax, 0
	cpuid


	mov [rdi], ebx
	mov [rdi + 4], edx
	mov [rdi + 8], ecx

	mov byte [rdi+13], 0

	mov rax, rdi

	pop rbx

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
	shr al, 7				; El bit de upgrade in progress es el 7mo
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

;-------------------------------------------------------
;	Indica si se puede leer el teclado
;	1 = Se puede
;	0 = No se puede
;-------------------------------------------------------
; Llamada en C:
; 	unsigned int canReadKey();
;-------------------------------------------------------
canReadKey:
	mov rax, 0
	in al, 64h
	and al, 1
	ret

;-------------------------------------------------------
;	Devuelve la lectura del teclado
;-------------------------------------------------------
; Llamada en C:
;	unsigned int getScanCode();
;-------------------------------------------------------
getScanCode:
	mov rax, 0
	in al, 60h
	ret


