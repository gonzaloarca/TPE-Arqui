#ifndef FRAME_ASM_H

#define FRAME_ASM_H
/*
//	Funcion de assembler que guarda el stack frame
void saveMainFrame(StackFrame *mainFrame);
*/
void getFrame(StackFrame *mainFrame);

//	Funcion de assembler que setea el frame actual
void setFrame(StackFrame *mainFrame);

#endif