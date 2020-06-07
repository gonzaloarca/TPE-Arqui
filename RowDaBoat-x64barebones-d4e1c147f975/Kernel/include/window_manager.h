#ifndef _WINDOW_MANAGER_H
#define _WINDOW_MANAGER_H

#include <registers.h>

// Funcion de booteo, establece parametros default de cada ventana a mostrar en la pantalla
void setWindows();

// Funcion que permite el cambio de ventana activa
int sys_changeWindow( unsigned int newIndex );

// Funcion que permite el cambio del color de los caracteres a escribir en la ventana actual
int sys_changeWindowColor(int rgb);

// Funcion que permite la impresion de un caracter en la ventana actual
int sys_write(unsigned int fd, const char * str, unsigned long count);

//	Syscall que limpia la pantalla activa actualmente
void sys_clrScreen();

// Funcion que imprime los registros pasados por parametro
void printRegisters(RegistersType *reg);

#endif