/* sampleCodeModule.c */
#include <graphics_engine.h>
#include <shell.h>

void writePixel(int x, int y, int rgb);

//	Esto es lo primero que se ejecuta de Userland
int main(){
	// for(int i = 0; i < 400 ; i++)
	// 	writePixel(i,500,0xFFFFFF);

	setWindows();
	runShell(0);
	while(1);
	return 0;
}
