#include <stdint.h>
#include <keyboard.h>
#include <video_driver.h>
#include <window_manager.h>
#include <libasm64.h>
#include <cpuinfo.h>
#include <registers.h>
#include <rtc_driver.h>
#include <process_manager.h>

typedef struct{
	uint64_t rbx;
	uint64_t rcx;
	uint64_t rdx;
	uint64_t rsi;
	uint64_t rdi;
}*Registers;

// La syscall 3 es read
uint64_t syscall_03(uint64_t rbx, uint64_t rcx)
{
	return sys_read( (char*) rbx, (unsigned long int) rcx );

}

//	La syscall 4 es write
uint64_t syscall_04(uint64_t rbx, uint64_t rcx, uint64_t rdx)
{
	return sys_write( (unsigned int) rbx, (const char*) rcx, (unsigned long) rdx  );
}

// La syscall 8 permite el intercambio de ventana actual
uint64_t syscall_08(uint64_t rbx)
{
	return sys_changeWindow((unsigned int) rbx);
}

// La syscall 9 permite el cambio de color de los caracteres a escribir en la ventana actual
uint64_t syscall_09(uint64_t rbx)
{
	return sys_changeWindowColor((int) rbx);
}

//	La syscall 10 vacia el buffer de teclado
uint64_t syscall_10()
{
	sys_emptyBuffer();
	return 0;	
}

//	La syscall 12 recibe un puntero a estructura de tipo TimeFormat y la rellena con los datos actuales del RTC
uint64_t syscall_12( uint64_t rbx )
{
	sys_getTime( (TimeFormat *) rbx );
	return 0;
}

//	La syscall 13 devuelve la temperatura del CPU como un conjunto en celsius
uint64_t syscall_13()
{
	return sys_getCPUTemp();
}

// La syscall 14 devuelve los registros con los valores de cuando se presiono por ultima vez F1
uint64_t syscall_14()
{
	return (uint64_t) sys_getRegisters();
}

//	La syscall 20 devuelve informacion del CPU
uint64_t syscall_20( uint64_t rbx )
{
	return sys_cpuinfo((CpuInfo *) rbx);
}

//	La syscall 21 recibe un puntero a funcion de un proceso y lo inicializa
uint64_t syscall_21( uint64_t rbx ){
	return sys_initModule((void (*)()) rbx);
}

//	La syscall 22 cambia el programa activo al siguiente en la lista de programas
uint64_t syscall_22(){
	return sys_switchProcess();
}

//	La syscall 23 corre el primer proceso si es que hay al menos uno cargado
uint64_t syscall_23(){
	sys_runFirstProcess();
	return 0;
}

//	scNumber indica a cual syscall se llamo
//	parameters es una estructura con los parametros para la syscall
//	Cada syscall se encarga de interpretar a la estructura
uint64_t sysCallDispatcher(uint64_t scNumber, Registers reg)
{

	switch(scNumber)
	{
		case 3: return syscall_03( reg->rbx, reg->rcx ); 

		case 4: return syscall_04( reg->rbx, reg->rcx, reg->rdx ); 

		case 8: return syscall_08( reg->rbx );

		case 9: return syscall_09( reg->rbx );
	
		case 10: return syscall_10();

		case 12: return syscall_12( reg->rbx );

		case 13: return syscall_13();

		case 14: return syscall_14();

		case 20: return syscall_20( reg->rbx );

		case 21: return syscall_21( reg->rbx );

		case 22: return syscall_22();

		case 23: return syscall_23();

	}

	return 0;
}
