GLOBAL markFrame
GLOBAL setFrame
GLOBAL getrbp
GLOBAL getrsp

markFrame:
	mov [rdi], rbp		; en rbp tengo donde estaba rsp en main
	mov rax, [rsp+8]	; en rsp+8 esta el ebp del main
	mov [rdi+8], rax
	ret

setFrame:
	mov rbp, [rdi]		; Restauro el ebp
	mov rsp, [rdi+8]	; Restauro el ebp
	mov rax, rsi
	push rax			; Hago push de la direccion del programa a correr
	ret 				; Al hacer ret, voy a correr mi programa con el stackFram
						; armado como si hubiera sido llamada desde main

getrbp:
	mov rax, rbp
	ret

getrsp:
	mov rax, rsp
	ret
