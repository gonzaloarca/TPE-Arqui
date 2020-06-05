#ifndef	CONSOLE_H
#define CONSOLE_H

#include <stdint.h>

#define MAX_PROMPT 5
#define BUFFER_SIZE 100

typedef struct 
{
	uint64_t *rbp;
	uint64_t *rsp;
} StackFrame;

//	Estructura que representa a los modulos (programas) "activos" 
typedef struct 
{
	void (*program)();					//puntero al inicio del programa
	unsigned int windowID;				//numero de ventana que ocupa
	char buffer[BUFFER_SIZE];			//buffer de la terminal para cada programa
	unsigned int size;					//tamaño del buffer ocupado actualmente
	char prompt[MAX_PROMPT];			//"prompt" que se imprime para pedir input
	char delimiter;						//delimitar para leer por STDIN
	StackFrame stackFrame;				//info del stack frame cuando se cambia de ventana
} Module;

//	Se llama a una funcion de assembler para setear el stack frame del main
void setMainFrame();

//	Funcion para agregar un nuevo modulo a la lista de modulos
//	Regresa 1 si pudo agregarlo (hay ventanas vacias)
//	0 si no (no hay ventanas libres)
int initModule(void (*program)(), char prompt[MAX_PROMPT], char delimiter);

//	Empezar a correr el programa de la primera ventana
void startFirstProgram();

// Funcion que permite obtener la entrada del teclado y guardarla en un buffer, 
//	retorna la cantidad de caracteres leidos
int getInput( char *inputBuffer, unsigned long int buffer_size );

#endif
