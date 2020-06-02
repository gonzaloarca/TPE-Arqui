/* sampleCodeModule.c */
#include <graphics_engine.h>
#include <shell.h>
#include <evaluator.h>

void writePixel(int x, int y, int rgb);

//	Esto es lo primero que se ejecuta de Userland
int main(){
	// for(int i = 0; i < 400 ; i++)
	// 	writePixel(i,500,0xFFFFFF);

	//runShell();
	calculator();
	return 0;
}
