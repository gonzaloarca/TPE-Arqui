#ifndef	CONSOLE_H
#define CONSOLE_H

#include <stdint.h>

#define LINE_COUNT 50
#define MAX_PROMPT 5
#define BUFFER_SIZE 100
#define DEFAULT_DELIMITER '\n'
#define DEFAULT_PROMPT ": "

typedef struct 
{
	uint64_t *rbp;
	uint64_t *rsp;
} StackFrame;

typedef struct
{
	uint64_t rbx;
	uint64_t r12;
	uint64_t r13;
	uint64_t r14;
	uint64_t r15;
} RegBackup;

//	Estructura que representa a los modulos (programas) "activos" 
typedef struct 
{
	void (*program)();					//puntero al inicio del programa
	char prompt[MAX_PROMPT+1];			//"prompt" que se imprime para pedir input
	char delimiter;						//delimitar para leer por STDIN
	StackFrame stackFrame;				//info del stack frame cuando se cambia de ventana
	RegBackup backup; 					//registros a preservar entre llamados
} Module;

//	Funcion para agregar un nuevo modulo a la lista de modulos
//	Regresa 1 si pudo agregarlo (hay ventanas vacias)
//	0 si no (no hay ventanas libres)
int initModule(void (*program)(), char prompt[MAX_PROMPT], char delimiter);

//	Empezar a correr el programa de la primera ventana
void startFirstProgram();

// Funcion que permite obtener la entrada del teclado y guardarla en un buffer, 
//	retorna la cantidad de caracteres leidos
int getInput( char *inputBuffer, unsigned long int buffer_size );

//	Espera a que el usuario presione una tecla y la devuelve
char waitInput();

//	Limpia la pantalla
void clrScreen();

#endif
