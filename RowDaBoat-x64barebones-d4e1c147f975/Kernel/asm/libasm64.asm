GLOBAL cpuVendor
GLOBAL cpuBrand
GLOBAL cpuModel
GLOBAL getTimeRTC
GLOBAL canReadKey
GLOBAL getScanCode
GLOBAL sys_getCPUTemp
GLOBAL saveRegistersASM

GLOBAL saveRegistersASMexcp

section .text
	
;-------------------------------------------------------
;	Indica el fabricante del CPU
;-------------------------------------------------------
; Llamada en C:
;	char *cpuVendor(char buffer[13]);
;-------------------------------------------------------
cpuVendor:
	push rbp
	mov rbp, rsp

	push rbx

	mov rax, 0
	cpuid

	mov [rdi], ebx
	mov [rdi + 4], edx
	mov [rdi + 8], ecx
	mov byte [rdi + 12], 0

	mov rax, rdi

	pop rbx

	mov rsp, rbp
	pop rbp
	ret

;-------------------------------------------------------
;	Indica la marca del CPU
;-------------------------------------------------------
; Llamada en C:
;	char *cpuBrand(char buffer[48]);
;-------------------------------------------------------
cpuBrand:
	push rbp
	mov rbp, rsp
	push rbx

	; Esta instruccion es para checkear si existe la info que queremos
	mov eax, 80000000h
	cpuid
	cmp eax, 80000004h
	jb .return

	mov rsi, rdi

	; Con las siguientes llamadas obtengo el string en partes
	mov eax, 80000002h
	cpuid
	mov [rdi], eax
	mov [rdi+4], ebx
	mov [rdi+8], ecx
	mov [rdi+12], edx
	add rdi, 16

	mov eax, 80000003h
	cpuid
	mov [rdi], eax
	mov [rdi+4], ebx
	mov [rdi+8], ecx
	mov [rdi+12], edx
	add rdi, 16

	mov eax, 80000004h
	cpuid
	mov [rdi], eax
	mov [rdi+4], ebx
	mov [rdi+8], ecx
	mov [rdi+12], edx

	mov rax, rsi

.return:
	pop rbx
	mov rsp, rbp
	pop rbp
	ret

;-------------------------------------------------------
;	Indica el modelo y la familia del CPU
;-------------------------------------------------------
; Llamada en C:
;	void cpumodel(int buffer[2]);
;-------------------------------------------------------
cpuModel:
	push rbp
	mov rbp, rsp
	push rbx

	mov eax, 1
	cpuid

	; Me guardo el valor base de model
	mov ebx, eax
	shr ebx, 4
	and ebx, 0xF
	mov [rdi+4], ebx

	; Obtengo el FamilyID
	mov ebx, eax
	shr ebx, 8
	and ebx, 0xF
	; Me guardo su valor base
	mov [rdi], ebx

	; Veo si es 15 o 6
	cmp ebx, 15
	je .quince
	cmp ebx, 6
	je .seis
	jmp .return

	; Si es 15, necesito el Model y Family extendido
.quince:
	mov ebx, eax
	shr ebx, 20
	and ebx, 0xFF
	add DWORD [rdi], ebx
	; Si es 6, solo necesito el Model extendido
.seis:
	mov ebx, eax
	shr ebx, 16
	and ebx, 0xF
	shl ebx, 4
	add [rdi+4], ebx

.return:
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


;-------------------------------------------------------
;	Devuelve la temperatura absoluta en ºC del CPU 
;-------------------------------------------------------
; Llamada en C:
;	int sys_getCPUTemp();
;-------------------------------------------------------
sys_getCPUTemp:			
	push rbp
	mov rbp, rsp

	push rcx
	push rdx
	push rbx

	mov rbx, 0					
	mov rax, 0			;vacio rax para tener los 32 bits mas significativos en 0
	mov ecx, 412		;leo la temperatura offset del CPU
	rdmsr				;ejecuto la instruccion read msr
	
						;me quedo en edx:eax el registro entero
						;debo solo acceder a los bits 22:16 de edx:eax

	and eax, 0x7F0000	;en eax me quedan los bits 22:16
	shr eax, 16			;ahora queda en eax el valor de la temperatura offset
	mov ebx, eax		;almaceno el valor en ebx

	mov rax, 0
	mov ecx, 418		
	rdmsr				;leo la temperatura maxima que se aguanta el CPU

	and eax, 0xFF0000	;en eax me quedan los bits 23:16
	shr eax, 16			

	sub eax, ebx		;Obtengo la temperatura absoluta haciendo Tmax - Toffset

	pop rbx
	pop rdx
	pop rcx

	mov rsp, rbp
	pop rbp
	ret

	

;-------------------------------------------------------
;	Guarda en la estructura indicada por parametro los registros al tocar f1
;-------------------------------------------------------
; Llamada en C:
;	void saveRegistersASM(registersType reg)
;-------------------------------------------------------
saveRegistersASM:
	push rbp
	mov rbp, rsp

	mov rax, [rbp]		; rbp de saveRegisters de C

	mov rax, [rax]		; rbp de keyboardHandler

	mov rax, [rax]		; rbp de int_21

	mov rax, [rax]		; rbp de irqDispatcher

	; en rax tengo la posicion donde esta guardando el rbp viejo
	; en rax+8 retorno a irq Master
	; en rax+16 tengo el ultimo registro pusheado q es r15

	add rax, 16

	call fillRegisters

	mov rsp, rbp
	pop rbp
	ret


;-------------------------------------------------------
;	Guarda en la estructura indicada por parametro los registros al lanzarse una excepcion
;-------------------------------------------------------
; Llamada en C:
;	void saveRegistersASMexcp(registersType reg)
;-------------------------------------------------------
saveRegistersASMexcp:
	push rbp
	mov rbp, rsp

	mov rax, [rbp]		; rbp de zero_division

	mov rax, [rax]		; rbp de exceptionDispatcher

	; en rax tengo la posicion donde esta guardando el rbp viejo
	; en rax+8 retorno a exception Handler
	; en rax+16 tengo el ultimo registro pusheado q es r15

	add rax, 16

	call fillRegisters

	mov rsp, rbp
	pop rbp
	ret



;-------------------------------------------------------
;	Funcion interna para guardar en la estructura recibida en rax los 16 registros
;-------------------------------------------------------
; 	fillRegisters		
;			recibe en rax el puntero a r15 y en rdi el puntero a la estructura de tipo RegistersType
;			(sigue el comportamiento de como se pushean los registros en una interrupcion)
;-------------------------------------------------------
fillRegisters:
	push rbp
	mov rbp, rsp
	push rbx
	push rcx

	mov rcx, 15

.ciclo:
	dec rcx
	mov rbx, [rax]
	mov QWORD[rdi + 8*rcx], rbx
	add rax, 8
	cmp rcx, 0
	jnz .ciclo

	; ahora tengo RIP, CS, EFLAGS, RSP y SS que fueron guardados por la excepcion(no hay error code)

	mov rbx, [rax]
	mov QWORD[rdi + 8*16], rbx				;rip

	mov rbx, [rax+24]
	mov QWORD[rdi + 8*15], rbx				;rsp

	pop rcx
	pop rbx
	mov rsp, rbp
	pop rbp
	ret