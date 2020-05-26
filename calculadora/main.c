#include <stdio.h>
#include <string.h>
#include "evaluator.h"

int main(int argc, char const *argv[])
{
	char input[100];
	scanf("%s", input);
	int res = evaluate(input, strlen(input));
	printf("%d\n", res);
	return 0;
}
