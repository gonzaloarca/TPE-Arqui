#ifndef STD_IO_H_
#define STD_IO_H_

#include <stdarg.h>
#include <stdint.h>
#define NULL 0
#define DEFAULT_CHAR_COLOR 0xFFFFFF

int getchar();

int printf(const char *format, ...);

// Funcion que permite escribir un caracter con color default(blanco)
int putchar(char c);

// Funcion que permite escribir un caracter con color rgb
int putcharColor(char c, int rgb);

// Funcion que permite escribir count caracteres de un string con color rgb, implementada en ASM
int write( unsigned int count, char * str, int rgb );

// Funcion que permite la impresion de un string terminado en null en la ventana actual
int puts( char * s, int rgb );

int scanf(const char *format, ...);

// Funcion que retorna -1 si str1 es menor que str2, 0 si son iguales y 1 caso contrario
int strcmp(char *str1, char *str2);

// Funcion que retorna la longitud de str
int strlen(char *str);

#endif

