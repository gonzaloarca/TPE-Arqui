/* sampleCodeModule.c */
#include <console.h>
#include <shell.h>
#include <evaluator.h>

//	Esto es lo primero que se ejecuta de Userland
int main(){

	initModule(runShell, "$> ", '\n');
	initModule(calculator, "Exp: ", '=');
	startFirstProgram();

	return 0;
}
