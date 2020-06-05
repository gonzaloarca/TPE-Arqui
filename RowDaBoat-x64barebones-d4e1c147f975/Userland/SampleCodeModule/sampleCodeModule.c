#include <console.h>
#include <std_io.h>

#include <shell.h>
#include <evaluator.h>


//	Esto es lo primero que se ejecuta de Userland
int main()
{
	char c;

	puts("Elija que modulo quiere empezar a correr:\n");
	puts("1 - Shell\n");
	puts("2 - Calculadora\n");

	while ((c = waitInput()) != '1' && c != '2' )
		fprintf(2, "NUMERO DE MODULO INCORRECTO\n");

	//	Aca habria que vaciar la pantalla

	if (c == '1')
	{
		initModule(runShell, "$> ", '\n');
		initModule(calculator, "Exp: ", '=');
	} else {
		initModule(calculator, "Exp: ", '=');
		initModule(runShell, "$> ", '\n');
	}

	startFirstProgram();

	return 0;
}
