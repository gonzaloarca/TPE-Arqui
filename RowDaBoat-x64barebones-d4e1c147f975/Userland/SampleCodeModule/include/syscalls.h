#ifndef SYSCALLS_H
#define SYSCALLS_H

// Funcion que permite escribir count caracteres de un string en el file descriptor fd
int write(unsigned int fd, char *buffer, unsigned long count);

//	Syscall para leer de entrada estandar en un buffer hasta que se llegue a "count" caracteres o se llegue al caracter "delim"
int read( char *buffer, unsigned long int count, char delim );

//	Vaciar el buffer del teclado
void emptyBuffer();

//	Limpia la pantalla actual
void clrScreen();

//	Funcion para agregar un nuevo modulo a la lista de modulos
//	Regresa 0 si pudo agregarlo (hay espacio para correr mas procesos)
//	1 si no (no hay mas espacio)
int initProcess( void (*program)() );

void runFirstProcess();

//	Cambiar el color de letra de la ventana
int changeWindowColor(int rgb);

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
