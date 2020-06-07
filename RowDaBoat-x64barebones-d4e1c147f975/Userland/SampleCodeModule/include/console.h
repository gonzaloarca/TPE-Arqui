#ifndef	CONSOLE_H
#define CONSOLE_H

#define LINE_COUNT 50
#define MAX_PROMPT 5
#define BUFFER_SIZE 100

//	Estructura para almacenar configuraciones del prompt de la consola
typedef struct 
{
	char symbol[MAX_PROMPT+1];			//simbolo/s que se imprime/n para pedir input
	char delimiter;						//delimitador para leer por STDIN
}PromptData;

// Funcion que permite obtener la entrada del teclado y guardarla en un buffer, imprimiendo el string de simbolos que recibe al principio de la linea, y
// recibiendo entrada hasta que se tipee la tecla que coincida con el delimitador que se le asigne
//	retorna la cantidad de caracteres leidos
int getInput( char *inputBuffer, unsigned long int buffer_size, char *promptSymbol, char promptDelimiter );

//	Espera a que el usuario presione una tecla y la devuelve
char waitInput();

#endif
