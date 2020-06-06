#include <console.h>
#include <std_io.h>
#include <syscalls.h>
#include <shell.h>
#include <evaluator.h>

//	En el main se setean los programas que van a correr en cada ventana
int main()
{
	char c;

	puts("Elija que modulo quiere empezar a correr:\n");
	puts("1 - Shell\n");
	puts("2 - Calculadora\n");

	while ((c = waitInput()) != '1' && c != '2' )
		fprintf(2, "NUMERO DE MODULO INCORRECTO\n");

	clrScreen();

	if (c == '1')
	{
		initProcess(runShell);	//hay que dejar el prompt y el delimitador en Userland.
		initProcess(calculator);
	} else {
		initProcess(calculator);
		initProcess(runShell);
	}

	runFirstProcess();

	return 0;
}
