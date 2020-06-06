#ifndef PROCESS_MANAGER_H_
#define PROCESS_MANAGER_H_

#include <stdint.h>

typedef struct 
{
	uint64_t *rbp;
	uint64_t *rsp;
} StackFrame;

typedef struct
{
	uint64_t rbx;
	uint64_t r12;
	uint64_t r13;
	uint64_t r14;
	uint64_t r15;
} RegBackup;

//	Estructura que representa a los modulos (programas) "activos" 
typedef struct 
{
	void (*program)();					//puntero al inicio del programa
	unsigned int pid;					//process ID del programa
	StackFrame stackFrame;				//info del stack frame cuando se cambia de ventana
	RegBackup backup; 					//registros a preservar entre llamados
} Module;

//	Funcion de assembler que guarda el stack frame
void getBackup(StackFrame *frame, RegBackup *backup);

//	Funcion de assembler que setea el frame actual
void setBackup(StackFrame *frame, RegBackup *backup);

int sys_initModule(void (*program)());

int sys_switchProcess();

void sys_runFirstProcess();

#endif