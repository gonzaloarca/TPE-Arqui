#include <graphics_engice.h>
#include <windowADT.h>
#include <screenInfo.h>

typedef struct windowCDT{

	// Donde arranca el canvas de esta ventana
	int xStart;
	int yStart

	// Ultimas posiciones dentro de la ventana (no incluye desfasaje de xStart ni yStart)
	int xLast = 0;
	int yLast = 0;

	// Buffer de la pantalla para permitir scrolling
	char screenBuffer[BUFFER_LINES][MAX_LINE_CHARS] = {0}; // necesito una linea mas para tener la anterior

	// Cantidad de caracteres en la linea actual
	int currentLineSize = 0;

	// Cantidad de lineas escritas actualmente
	int lineCount = 0;

	// Linea en la cual arranca la pantalla
	int firstLine = 0;

} windowCDT;