#ifndef PROCESS_MANAGER_H_
#define PROCESS_MANAGER_H_

#include <stdint.h>
#include <registers.h>

//	Estructura que representa a los modulos (programas) "activos" 
typedef struct 
{
	void (*program)();					//puntero al inicio del programa
	unsigned int pid;					//process ID del programa
	RegistersType backup;				//Backup de los registros
} Module;

//	Se guarda un backup de los registros pusheados por interrupcion
void getBackupINT(RegistersType *reg);

//	Restaura un backup de los registros pusheados por interrupcion
void setBackupINT(RegistersType *reg);

//	Setea para inciar a correr un programa
void startRunning(uint64_t rip, uint64_t rsp);

//	Setea para inciar a correr un programa luego de una interrupcion
void startRunningEXC(uint64_t rip, uint64_t rsp);

// Syscall para agregar un modulo a la cola de procesos
int sys_initModule(void (*program)());

// Se cambia al siguiente proceso en la cola
int switchProcess();

// Syscall que arranca el primer proceso, en caso de existir
void sys_runFirstProcess();

// Funcion para resetear el proceso actual debido a una excepcion
void recoverModule();

#endif
