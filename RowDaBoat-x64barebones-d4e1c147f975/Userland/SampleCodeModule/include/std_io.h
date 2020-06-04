#ifndef STD_IO_H_
#define STD_IO_H_

#include <stdarg.h>
#include <stdint.h>
#include <syscalls.h>

#define NULL 0
#define DEFAULT_CHAR_COLOR 0xFFFFFF

int getchar();

int printf(const char *format, ...);

// Funcion que permite escribir un caracter con color default(blanco)
int putchar(char c);

// Funcion que permite la impresion de un string terminado en null en la ventana actual
int puts(char * s);

int scanf(const char *format, ...);

// Funcion que retorna -1 si str1 es menor que str2, 0 si son iguales y 1 caso contrario
int strcmp(char *str1, char *str2);

// Funcion que retorna la longitud de str
int strlen(char *str);

// Funcion que permite la impresion de un entero
int printInt(int num);

//	Convierte un long int a su representacion hexa, lo guarda en buffer[count]
char *intToHEX(unsigned long int num, char *buffer, unsigned int count);

#endif

