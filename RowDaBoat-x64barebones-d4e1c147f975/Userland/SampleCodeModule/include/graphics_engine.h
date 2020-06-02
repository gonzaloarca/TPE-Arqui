#ifndef	_GRAPHICS_ENGINE_H
#define _GRAPHICS_ENGINE_H

// Funcion que permite la impresion de un caracter
void printChar(char c, int rgb);

// Funcion que permite la impresion de un string de count caracteres en la ventana actual
int printString( unsigned int count, char * str,int rgb );

// Funcion que permite la impresion de un string terminado en null en la ventana actual
int printNullString( char * s, int rgb );

// Funcion que permite realizar un cambio de linea en la ventana actual
void newLine();

// Funcion que permite obtener la entrada del teclado y guardarla en un buffer, retorna la cantidad de caracteres leidos
int getInput( char *inputBuffer, unsigned int buffer_size );

// Funcion que permite la impresion de un entero
int printInt( int num, int rgb );

#endif
