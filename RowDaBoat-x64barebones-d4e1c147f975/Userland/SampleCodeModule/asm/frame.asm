GLOBAL saveMainFrame
GLOBAL setFrame
GLOBAL getFrame

;---------------------------------------------------------------
;	Funcion para guardar el stack frame del main
;---------------------------------------------------------------
; Llamada en C:
;	void saveMainFrame(StackFrame *mainFrame);
;---------------------------------------------------------------
saveMainFrame:
	mov rax, [rbp]
	mov [rdi], 	rax		;	rbp apunta al rbp del main()
	mov rax, rbp		
	add rax, 8			;	rbp+8 es el rsp del main
	mov [rdi+8], rax
	ret

;---------------------------------------------------------------
;	Funcion para guardar el stack frame actual
;---------------------------------------------------------------
; Llamada en C:
;	void getFrame(StackFrame *mainFrame);
;---------------------------------------------------------------
getFrame:
	mov [rdi], rbp		;	rbp no cambia al entrar a esta funcion
	mov [rdi+8], rsp	;	rsp no cambia al entrar a esta funcion
	ret

;---------------------------------------------------------------
;	Funcion para cambiar el stack frame actual
;---------------------------------------------------------------
; Llamada en C:
;	void setFrame(StackFrame *mainFrame);
;---------------------------------------------------------------
setFrame:
	mov rbp, [rdi]		; Restauro el rbp
	mov rsp, [rdi+8]	; Restauro el rsp
	ret
