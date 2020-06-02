#ifndef EVALUATOR_H

#define EVALUATOR_H

#define ANSWER 'A'
#define BUFFER_SIZE 100

//	Programa que pide que el usuario ingrese una expresion a evaluar
void calculator();

//	Recibe una expresión inorder y devuelve su resultado
//	Operaciones permitidas: + - / *
//	Ingresando 'A' se utiliza el resultado de la ultima operacion
double evaluate(char *expression);

#endif