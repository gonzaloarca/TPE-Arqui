GLOBAL getBackup
GLOBAL setBackup

;---------------------------------------------------------------
;	Funcion para realizar un backup de los registros necesarios
;---------------------------------------------------------------
; Llamada en C:
;	void getBackup(StackFrame *frame, RegBackup *backup);
;---------------------------------------------------------------
getBackup:
	mov [rdi], rbp		
	mov [rdi+8], rsp
	mov [rsi], rbx
	mov [rsi+8], r12
	mov [rsi+16], r13
	mov [rsi+24], r14
	mov [rsi+32], r15
	ret

;---------------------------------------------------------------
;	Funcion para hacer un restore del backup
;---------------------------------------------------------------
; Llamada en C:
;	void setBackup(StackFrame *frame, RegBackup *backup);
;---------------------------------------------------------------
setBackup:
	mov rbp, [rdi]		
	mov rsp, [rdi+8]	
	mov rbx, [rsi]
	mov r12, [rsi+8]
	mov r13, [rsi+16]
	mov r14, [rsi+24]
	mov r15, [rsi+32]
	ret

