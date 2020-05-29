#include "font.h"
#include "screenInfo.h"
#include "graphics_engine.h"
#include "windowADT.c"

static windowADT windows[N];
static int activeWindow = 0;		// por default arranca en la terminal

void writePixel(int x, int y, int rgb); // funcion de ASM

void setWindows(){
	for(int i = 0 ; i < N ; i++){
		windows[i]->xStart = i*WIDTH;
		windows[i]->yStart = 0;
		windows[i]->firstLine = 0;
		windows[i]->lineCount = 0;
		windows[i]->currentLineSize = 0;
	}
}

void switchWindow(int newIndex){
	if(newIndex < 0 || newIndex >= N || newIndex == activeWindow)
		return;			// permanece en la ventana actual
	else
		activeWindow = newIndex;
}

//  Funcion de uso interno que se encarga de utilizar la syscall para dibujar un caracter en pantalla dado su esquina izq superior y un color
static int drawChar( char c, int x, int y, int rgb ){	
	const unsigned char * letra = getCharMap(c);
	if( letra == 0 ){
		letra = getCharMap(32);		//debe printear vacio
	}
	for( int i = 0; i < FONT_HEIGHT; i++ ){
		unsigned char row = letra[i];
		for( int j = FONT_WIDTH - 1; j >= 0; j-- ){
			unsigned char aux = row & 1;
			if( aux == 1 ){
				writePixel( x + j, y + i, rgb);
			}
			else {
				writePixel( x + j, y + i, BACKGROUND_COLOR ); //negro por ahora
			}
			row = row >> 1;
		}
	}	

	return 0;
}

//  Funcion de uso interno que realiza la actualizacion de las lineas del buffer de la ventana activa, 
// si se llego al limite de lineas mostrada se encarga de realizar el scrolling del buffer
static void updateBuffer(){
	windows[activeWindow]->xLast = 0;
	windows[activeWindow]->lineCount++;					// Se agrego una linea en pantalla

	if( windows[activeWindow]->lineCount == SCREEN_LINES ){
		// Ahora la pantalla va a arrancar en una posicion mas alta en el buffer
		windows[activeWindow]->firstLine = (windows[activeWindow]->firstLine + 1) % BUFFER_LINES;
		windows[activeWindow]->lineCount--;
		int lastLine = (windows[activeWindow]->firstLine + SCREEN_LINES -1) % BUFFER_LINES; // Linea que ahora estara alfinal de la pantalla vacia

		// Solo borro caracteres que quedaron pendientes, no toda la linea
		for(int i = 0; i < MAX_LINE_CHARS ; i++ ){ //&& windows[activeWindow]->screenBuffer[lastLine][i] != 0
			windows[activeWindow]->screenBuffer[lastLine][i] = 0;
		}
	}

	windows[activeWindow]->yLast = windows[activeWindow]->lineCount * LINE_HEIGHT;
	windows[activeWindow]->currentLineSize = 0;
}

//  Funcion de uso interno que realiza el refresco de cada linea de la ventana actual, 
// modificando caracter por caracter, en caso de ser necesario, 
// teniendo en cuenta lo que se tenia escrito en pantalla(linea vieja) y lo que se esta por escribir(linea nueva)
static void refreshLine( int lineNumber ){
	// recordar que el updateBuffer me movio el firstLine una posicion

	int i = 0;																					// indice para iterar sobre la linea
	int currentLineIndex = (windows[activeWindow]->firstLine + lineNumber) % BUFFER_LINES;		// indice de la linea en el buffer que debe ser impresa en lineNumber
	int previousLineIndex;																		// indice de la linea en el buffer que se encuentra antes del ciclo escrita en pantalla

	if( windows[activeWindow]->firstLine == 0 && lineNumber == 0 )
		previousLineIndex = BUFFER_LINES -1;								
	else
		previousLineIndex = (windows[activeWindow]->firstLine + lineNumber -1) % BUFFER_LINES;		// Ya me asegure que el indice sera positivo

	// En este primer ciclo sobreescribire los chars que no comparten la linea que se encuentra en pantalla y la que se debe escribir
	// Finaliza una vez terminada la linea o una de las lineas
	for( ; i < MAX_LINE_CHARS && windows[activeWindow]->screenBuffer[previousLineIndex][i] != 0 && windows[activeWindow]->screenBuffer[currentLineIndex][i] != 0 ; i++ ){
		if(windows[activeWindow]->screenBuffer[previousLineIndex][i] != windows[activeWindow]->screenBuffer[currentLineIndex][i]){
			drawChar( windows[activeWindow]->screenBuffer[currentLineIndex][i], windows[activeWindow]->xStart + i * FONT_WIDTH, windows[activeWindow]->yStart + lineNumber * (LINE_HEIGHT) , CHAR_COLOR);
		}
	}

	// En caso de que la nueva linea sea mas corta que la anterior, se deben borrar en pantalla las letras que quedaron
	while( windows[activeWindow]->screenBuffer[previousLineIndex][i] != 0 && i < MAX_LINE_CHARS) {
		drawChar(0, windows[activeWindow]->xStart + i * FONT_WIDTH, windows[activeWindow]->yStart + lineNumber * (LINE_HEIGHT) , CHAR_COLOR);
		i++;
	}

	// En caso de que la nueva linea sea mas larga que la anterior, se deben agregar en pantalla estas letras sobrantes
	while( windows[activeWindow]->screenBuffer[currentLineIndex][i] != 0 && i < MAX_LINE_CHARS ) {
		drawChar( windows[activeWindow]->screenBuffer[currentLineIndex][i], windows[activeWindow]->xStart + i * FONT_WIDTH, windows[activeWindow]->yStart + lineNumber * (LINE_HEIGHT) , CHAR_COLOR);
		i++;
	}
}

void refreshScreen() {
	for(int i = 0 ; i < SCREEN_LINES; i++) {
		refreshLine(i);
	}
}

int printChar( char c, int rgb ){
	if ( c == '\n')
	{
		newLine();
		return 0;
	}

	if( windows[activeWindow]->currentLineSize == MAX_LINE_CHARS ){
		// Se llego al tope de caracteres que entran en una linea, se debe pasar a la siguiente
		newLine();
	}

	// Escribo en pantalla el nuevo caracter en la linea y posicion actual, y luego incremento la posicion para el proximo caracter
	drawChar( c, windows[activeWindow]->xStart + windows[activeWindow]->currentLineSize * FONT_WIDTH, windows[activeWindow]->yStart + windows[activeWindow]->lineCount * (LINE_HEIGHT) , CHAR_COLOR);
	
	// Cambio el caracter en el buffer
	*(windows[activeWindow]->screenBuffer[ (windows[activeWindow]->firstLine + windows[activeWindow]->lineCount) % BUFFER_LINES ] + windows[activeWindow]->currentLineSize) = c;
	windows[activeWindow]->currentLineSize++;
	
	windows[activeWindow]->xLast += FONT_WIDTH;

	return 0;
}

int printString( char * str, unsigned int count, int rgb ){
	for( int i = 0; i < count; i++ ){
		printChar( str[i], rgb );
	}

	return 0;
}

int printNullString( char * s, int rgb ){
	while( *s != 0 ){
		printChar( *s, rgb );
		s++;
	}

	return 0;
}

void newLine(){
	if(windows[activeWindow]->lineCount == SCREEN_LINES -1) {
		// Se llego alfinal de las lineas en pantalla, se debe subir una linea para que la ultima quede libre
		updateBuffer();
		refreshScreen();
	}else
		updateBuffer();
}

static void numToString( int num, char * str ){

	if( num == 0 ){
		str[0] = '0';
		str[1] = 0;
		return;
	}

	int dig = 0;
	int aux = num;

	while( aux != 0 ){ //cuento digitos para saber desde donde arrancar a meter los caracteres
		aux /= 10;
		dig++;
	}
	
	if( num < 0 ){
		dig++;
		str[0] = '-';
		num *= -1;		//porque num % n da negativo si num < 0
	}

	int i = dig;
	str[i--] = 0;

	while( num != 0 ){
		str[i--] = (num % 10) + '0';
		num /= 10;
	}
}

int printInt( int num, int rgb ){
	char buffer[32];
	numToString( num, buffer );
	printNullString(buffer, rgb);

	return 0;
}