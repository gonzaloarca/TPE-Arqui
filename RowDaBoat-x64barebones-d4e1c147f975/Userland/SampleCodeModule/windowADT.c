#include <screenInfo.h>
#include <windowADT.h>

typedef struct windowCDT{

	// Donde arranca el canvas de esta ventana
	int xStart;
	int yStart;

	// Ultimas posiciones dentro de la ventana (no incluye desfasaje de xStart ni yStart)
	int xLast;
	int yLast;

	// Buffer de la pantalla para permitir scrolling
	char screenBuffer[BUFFER_LINES][MAX_LINE_CHARS]; // necesito una linea mas para tener la anterior

	// Cantidad de caracteres en la linea actual
	int currentLineSize;

	// Cantidad de lineas escritas actualmente
	int lineCount;

	// Linea en la cual arranca la pantalla
	int firstLine;

} windowCDT;