#include <stdio.h>

#include "evaluator.h"

/*	Esta funcion funciona recursivamente.
**	Encuentra el ultimo operador que debe evaluar
**		(o sea, el primero de menor precedencia empezando 
**		por el final de la expresion) y luego evalua
**		las expresiones a su izquierda y derecha.
**	Finalmente, evalua utilizando su operador.
*/
double evaluate(char *expression, int length)
{
	//	Busco la posicion del operador a evaluar
	int operator = main_op(expression, length);

	//	Codigo de Testeo, ignorar
	//printf("Exp: ");
	//for (int i = 0; i<length; i++) putchar(expression[i]);
	//printf(" Op: %d\n", operator);

	//	Si no habia operador principal, tengo que convertir a numero
	//	Nota: tambien entra en este if las expresion con parentesis
	if (operator == -1)
	{
		//	Si era una expresion con parentesis, tengo que evaluarla
		if (expression[length-1] == ')')
			return evaluate(expression, length-1);
		//	Si no, era un numero y la tengo que convertir
		return strtonum(expression, length);
	}

	//	Evaluo expresiones de la izquierda y la derecha del operador
	double izq, der;
	izq = evaluate(expression, operator);
	der = evaluate(expression+operator+1, length-operator-1);

	//	Segun cual operador era, devuelvo su resultado correspondiente
	switch(expression[operator])
	{
		case '+':
			return izq + der;
		case '-':
			return izq - der;
		case '*':
			return izq * der;
		case '/':
			return izq / der;
	}

	//	Si llega hasta aca, debe haber algun error, retorno 0 por las dudas
	return 0;
}

/*	La funcion main_op devuelve la posicion del 
**		operador "principal" en la expresion
**		(el ultimo de menor precedencia)
**	Este se debe encontrar fuera de un parentesis
**		si no, se lo ignora
*/
int main_op(char *expression, int length)
{
	//	brackets indica si hay parentesis abierto o cerrado
	int brackets = 0;
	//	posicion del ultimo operador encontrado
	int last_op = -1;

	for (int i = length-1; i >= 0; i--)
	{
		switch(expression[i])
		{
			case '+': case '-':
				if (brackets==0)
				{
					if (last_op == -1 || expression[last_op] == '*' 
						|| expression[last_op] == '/')
					{
						return i;
					}
				}
				break;

			case '*': case '/':
				if (brackets==0 && last_op==-1)
				{
					last_op = i;
				}
				break;			

			case '(':
				brackets++;
				break;

			case ')':
				brackets--;
				break;
		}
	}

	return last_op;
}

double strtonum(char *string, int length)
{
	//	El numero acumulado hasta ahora
	double number = 0, aux,
		exp = 1; 	//Exponente para la parte real del numero
	//	El char actual
	char c;
	//	Indica la posicion del punto decimal
	int decimal = -1;	

	for (int i=0; i<length; i++)
	{
		c = string[i];
		if (c >= '0' && c <= '9')
		{
			//	Si todavia no llegue al punto, es la parte entera
			if (decimal == -1)
			{
				number *= 10;
				number += c - '0';
			}
			//	En cambio, si ya llegué tengo que realizar otro calculo
			else
			{
				exp /= 10;
				aux = c - '0';
				aux *= exp;
				number += aux;
			}
		}
		else if (c == '.')
			decimal = i;
		// Si hay un operador, hubo algun error, devuelvo 0
		else if (is_operand(c))
			return 0;
	}

	return number;
}

int is_operand(char c)
{
	switch(c)
	{
		case '+': case '-': case '/': case '*':
		return 1;
	}
	return 0;
}
