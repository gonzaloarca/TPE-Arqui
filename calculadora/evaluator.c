#include <stdio.h>

#include "evaluator.h"

int evaluate(char *expression, int length)
{
	printf("Exp: ");
	for (int i = 0; i<length; i++) putchar(expression[i]);

	int operator = main_op(expression, length);

	printf(" Op: %d\n", operator);

	//	Si no hay operador
	if (operator == -1)
		return strtonum(expression, length);

	int izq, der;

	izq = evaluate(expression, operator);
	der = evaluate(expression+operator+1, length-operator-1);

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

	return 0;
}

//	La funcion main_op devuelve la posicion del 
//	operador "principal" en la expresion
//	(o sea, el operador que a izq y a der 
//	se va a evaluar recursivamente)
int main_op(char *expression, int length)
{
	//	brackets indica si hay parentesis abierto o cerrado
	int brackets = 0;
	int last_op = -1;

	for (int i = 0; i < length; i++)
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

int strtonum(char *string, int length)
{
	if (string[0] == '(' && string[length-1] == ')')
		return evaluate(string+1, length-2);

	int number = 0;
	char c;

	for (int i=0; i<length; i++)
	{
		c = string[i];
		if (c >= '0' && c <= '9')
		{
			number *= 10;
			number += c - '0';
		}
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
