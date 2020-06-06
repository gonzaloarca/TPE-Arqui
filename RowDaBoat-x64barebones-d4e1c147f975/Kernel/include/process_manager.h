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

// Syscall para agregar un modulo a la cola de procesos
int sys_initModule(void (*program)());

// Syscall que cambia al siguiente proceso en la cola
int sys_switchProcess();

// Syscall que arranca el primer proceso, en caso de existir
void sys_runFirstProcess();

// Funcion para resetear el proceso actual debido a una excepcion
void recoverModule();

#endif