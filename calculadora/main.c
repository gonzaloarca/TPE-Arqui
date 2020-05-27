#include <stdio.h>

#include "evaluator.h"

int main(int argc, char const *argv[])
{
	char input[100];
	scanf("%s", input);
	double res = evaluate(input);
	printf("%f\n", res);
	return 0;
}
