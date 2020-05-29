#ifndef	_GRAPHICS_ENGINE_H
#define _GRAPHICS_ENGINE_H

// Funcion de booteo, establece parametros default de cada ventana a mostrar en la pantalla
void setWindows();

// Funcion que permite el cambio de ventana activa
void switchWindow( int newIndex );

// Funcion que permite la impresion de un caracter en la ventana actual
int printChar( char c, int rgb );

// Funcion que permite la impresion de un string de count caracteres en la ventana actual
int printString( char * str, unsigned int count, int rgb );

// Funcion que permite la impresion de un string terminado en null en la ventana actual
int printNullString( char * s, int rgb );

// Funcion que permite realizar un cambio de linea en la ventana actual
void newline();

// Funcion que permite realizar un refresco de la ventana, borrando todas las lineas mostradas
void refreshScreen();

// Funcion que permite la impresion de un entero
int printInt( int num, int rgb );

#endif