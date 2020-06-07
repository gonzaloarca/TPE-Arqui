#ifndef _EVALUATOR_H
#define _EVALUATOR_H

#include <console.h>
#include <std_io.h>

#define ANSWER 'A'
#define ERROR_MSG "\nExpresion erronea, pruebe de vuelta\n"
#define BUFFER_SIZE 100

//	Programa que pide que el usuario ingrese una expresion inorder a evaluar
//	e imprime su resultado en pantlla
//	Operaciones permitidas: + - / *
//	Ingresando 'A' se utiliza el resultado de la ultima operacion
void calculator();

#endif