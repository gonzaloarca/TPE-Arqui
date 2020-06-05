#ifndef FRAME_ASM_H

#define FRAME_ASM_H

//	Funcion de assembler que guarda el stack frame
void getBackup(StackFrame *frame, RegBackup *backup);

//	Funcion de assembler que setea el frame actual
void setBackup(StackFrame *frame, RegBackup *backup);

#endif
