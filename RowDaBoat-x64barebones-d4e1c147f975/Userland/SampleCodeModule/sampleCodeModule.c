/* sampleCodeModule.c */
#include <graphics_engine.h>

void writePixel(int x, int y, int rgb);

//	Esto es lo primero que se ejecuta de Userland
int main() {
	// for(int i = 0; i < 400 ; i++)
	// 	writePixel(i,500,0xFFFFFF);

	setWindows();
	int s = 0;
	for(int i = 0 ; i < 100000; i++){
		printInt(i, 0xFFFFFF);
		printChar(" ", 0xFFFFFF);
	}
	return 0;
}