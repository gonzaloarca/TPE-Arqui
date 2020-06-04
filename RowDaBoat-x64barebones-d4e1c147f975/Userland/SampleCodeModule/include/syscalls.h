#ifndef SYSCALLS_H

#define SYSCALLS_H

//	Dibujar un pixel a la pantalla
//	NOTA: DECIDIR SI VALE LA PENA, PORQUE SI ESTÁ
//	EL USER TIENE ACCESS A TODA LA PANTALLA ("PELIGROSO" COMO DIRIA NICO)
void writePixel( int x, int y, int rgb );

// Funcion que permite escribir count caracteres de un string en el file descriptor fd
int write(unsigned int fd, char *buffer, unsigned long count);

//	Syscall para leer un file descriptor
int read( char *buffer, unsigned long int count );

//	Espera activa a interrupcion del teclado
void _hlt();

//	Vaciar el buffer del teclado
void emptyBuffer();

//	Cambiar la ventana activa
int changeWindow(unsigned int window);

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

typedef struct{
	char vendor[13];
	char brand[48];
	int family;
	int model;
} CpuInfo;

//	Efectua la syscall para obtener informacion del CPU
void getCPUInfo(CpuInfo* info);

#endif
