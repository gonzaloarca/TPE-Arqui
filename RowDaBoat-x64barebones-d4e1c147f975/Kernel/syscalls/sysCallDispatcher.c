#include <stdint.h>
#include <keyboard.h>
#include <video_driver.h>

typedef struct{
	uint64_t rbx;
	uint64_t rcx;
	uint64_t rdx;
	uint64_t rsi;
	uint64_t rdi;
}*Registers;


//	La syscall 3 es read
// uint64_t syscall_03(uint64_t rbx, uint64_t rcx, uint64_t rdx)
// {
// 	return sys_read( (unsigned int) rbx, (char*) rcx, (unsigned long) rdx );

// }

// //	La syscall 4 es write
// uint64_t syscall_04(uint64_t rbx, uint64_t rcx, uint64_t rdx)
// {
// 	return sys_write( (unsigned int) rbx, (const char*) rcx, (unsigned long) rdx  );
// }

// La syscall 7 modifica el valor de un pixel en pantalla
uint64_t syscall_07(uint64_t rbx, uint64_t rcx, uint64_t rdx)
{
	return sys_writePixel( (int) rbx , (int) rcx, (int) rdx );
}

// La syscall 8 modifica los valores de una posicion en pantalla de forma de dibujar el caracter en rbx con la font default del sistema
uint64_t syscall_08(uint64_t rbx, uint64_t rcx, uint64_t rdx, uint64_t rsi, uint64_t rdi)
{
	return sys_drawChar( (char) rbx, (int) rcx, (int) rdx, (int) rsi, (int) rdi);
}

//	La syscall 10 vacia el buffer de teclado
uint64_t syscall_10()
{
	sys_emptyBuffer();
	return 0;	
}

//	La syscall 11 recibe una tecla del teclado y la devuelve en formato ASCII
uint64_t syscall_11()
{
	return sys_getKey();
}

//	scNumber indica a cual syscall se llamo
//	parameters es una estructura con los parametros para la syscall
//	Cada syscall se encarga de interpretar a la estructura
uint64_t sysCallDispatcher(uint64_t scNumber, Registers reg)
{

	switch(scNumber)
	{
		// case 3: return syscall_03( reg->rbx, reg->rcx, reg->rdx ); 

		// case 4: return syscall_04( reg->rbx, reg->rcx, reg->rdx ); 

		case 7: return syscall_07( reg->rbx, reg->rcx, reg->rdx );

		case 8: return syscall_08( reg->rbx, reg->rcx, reg->rdx, reg->rsi, reg->rdi );

		//case 9: return syscall_09( reg->rbx );

		case 10: return syscall_10();

		case 11: return syscall_11();
	}

	return 0;
}
