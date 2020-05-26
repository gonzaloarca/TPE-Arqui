#include <stdint.h>

//	La syscall 1 es write
uint64_t syscall_01(void *parameters)
{
	return 0;
}

//	scNumber indica a cual syscall se llamo
//	parameters es una estructura con los parametros para la syscall
//	Cada syscall se encarga de interpretar a la estructura
uint64_t sysCallDispatcher(uint64_t scNumber, void *parameters)
{
	switch(scNumber)
	{
		case 1: return syscall_01(parameters); 
	}

	return 0;
}
