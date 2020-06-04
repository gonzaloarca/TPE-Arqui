/* sampleCodeModule.c */
#include <console.h>
#include <shell.h>
#include <evaluator.h>

//	Esto es lo primero que se ejecuta de Userland
int main(){
	//	Marco este stack Frame como el main
	setMainFrame();
	initModule(runShell, "$> ", '\n');
	initModule(calculator, "?? ", '=');
	startFirstProgram();
	//runShell();
	//calculator();
	return 0;
}
