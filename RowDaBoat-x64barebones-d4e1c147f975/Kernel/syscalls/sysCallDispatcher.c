#include <stdint.h>
#include <video_driver.h>

typedef struct{
	uint64_t rbx;
	uint64_t rcx;
	uint64_t rdx;
	uint64_t rsi;
	uint64_t rdi;
}*Registers;


//	La syscall 3 es read
uint64_t syscall_03(uint64_t rbx, uint64_t rcx, uint64_t rdx)
{
	return sys_read( (unsigned int) rbx, (char*) rcx, (unsigned long) rdx );

}

//	La syscall 4 es write
uint64_t syscall_04(uint64_t rbx, uint64_t rcx, uint64_t rdx)
{
	return sys_write( (unsigned int) rbx, (const char*) rcx, (unsigned long) rdx  );
}

// La syscall 7 modifica el valor de un pixel en pantalla
uint64_t syscall_07(uint64_t rbx, uint64_t rcx, uint64_t rdx)
{
	sys_writePixel( (int) rbx , (int) rcx, (int) rdx );
	return 0;
}

//	La syscall 9 setea la salida estándar actual
uint64_t syscall_09(uint64_t rbx)
{
	sys_setStdOut( (void (*)( char )) rbx );
	return 0;
}


//	scNumber indica a cual syscall se llamo
//	parameters es una estructura con los parametros para la syscall
//	Cada syscall se encarga de interpretar a la estructura
uint64_t sysCallDispatcher(uint64_t scNumber, Registers reg)
{

	switch(scNumber)
	{
		case 3: return syscall_03( reg->rbx, reg->rcx, reg->rdx ); 

		case 4: return syscall_04( reg->rbx, reg->rcx, reg->rdx ); 

		case 7: return sys_call07( reg->rbx, reg->rcx, reg->rdx );

		case 9: return sys_call09( reg->rbx );
	}

	return 0;
}
