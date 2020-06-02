#ifndef SYSCALLS_H

#define SYSCALLS_H

//	Dibujar un pixel a la pantalla
//	NOTA: DECIDIR SI VALE LA PENA, PORQUE SI ESTÁ
//	EL USER TIENE ACCESS A TODA LA PANTALLA ("PELIGROSO" COMO DIRIA NICO)
void writePixel( int x, int y, int rgb );

// Funcion que permite escribir count caracteres de un string en el file descriptor fd
int write(unsigned int fd, char *buffer, unsigned long count);

//	Syscall para leer un file descriptor
int read( unsigned int fd, char *buffer, unsigned long count );

//	Espera activa a interrupcion del teclado
void _hlt();

//	Vaciar el buffer del teclado
void emptyBuffer();

//	Obtener una tecla del teclado
char getKey();

//	Cambiar la ventana activa
int changeWindow(unsigned int window);

//	Temperatura del CPU
int getCPUTemp();

typedef struct{
    uint32_t secs;
    uint32_t mins;
    uint32_t hours;
}TimeFormat;

//	Syscall para obtener la hora actual
void getTime(TimeFormat *time); //llama a syscall que llena la estructura de TimeFormat

#endif
