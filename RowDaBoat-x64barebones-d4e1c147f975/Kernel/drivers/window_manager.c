#include <window_manager.h>
#include <screenInfo.h>
#include <video_driver.h>

typedef struct{

	char character;
	int color;

}charWithColor;

typedef struct{

	// Donde arranca el canvas de la ventana
	int xStart;
	int yStart;

	// Buffer de la pantalla para permitir scrolling
	charWithColor screenBuffer[BUFFER_LINES][MAX_LINE_CHARS]; // necesito una linea mas para tener la anterior

	// Cantidad de caracteres en la linea actual
	int currentLineSize;

	// Cantidad de lineas escritas actualmente
	int lineCount;

	// Linea en la cual arranca la pantalla
	int firstLine;

	// Color a utilizar para imprimir los nuevos caracteres
	int charColor;

} Window;

static int printChar( char c, int rgb );

static Window windows[N] = {{0}};
static int activeWindow = 0;		// por default arranca en la terminal

void setWindows(){
	for(int i = 0 ; i < N ; i++){
		windows[i].xStart = i*WINDOW_WIDTH + WINDOW_MARGIN;
		windows[i].yStart = WINDOW_MARGIN;
		windows[i].firstLine = SCREEN_LINES-1;			// queremos que arranque arranque a escribir en la pantalla desde la ultima linea
		windows[i].lineCount = SCREEN_LINES-1;
		windows[i].currentLineSize = 0;
		windows[i].charColor = CHAR_COLOR;
		// Borde izquierdo
		for(int j = i*WINDOW_WIDTH; j < i*WINDOW_WIDTH+WINDOW_MARGIN ; j++)
			for(int k = WINDOW_MARGIN; k < WINDOW_HEIGHT - WINDOW_MARGIN; k++)		// no abarca nada del borde superior
				sys_writePixel( j, k, WINDOW_MARGIN_COLOUR);
		// Borde derecho
		for(int j = (i+1)*WINDOW_WIDTH- WINDOW_MARGIN; j < (i+1)*WINDOW_WIDTH ; j++)
			for(int k = WINDOW_MARGIN; k < WINDOW_HEIGHT - WINDOW_MARGIN; k++)
				sys_writePixel( j, k, WINDOW_MARGIN_COLOUR);
		// Borde superior
		for(int j = 0 ; j < WINDOW_MARGIN ; j++)
			for(int k = i*WINDOW_WIDTH; k < (i+1)*WINDOW_WIDTH ; k++)
				sys_writePixel( k, j, WINDOW_MARGIN_COLOUR);
		// Borde inferior
		for(int j = TOTAL_HEIGHT - WINDOW_MARGIN ; j < TOTAL_HEIGHT ; j++)
			for(int k = i*WINDOW_WIDTH; k < (i+1)*WINDOW_WIDTH ; k++)
				sys_writePixel( k, j, WINDOW_MARGIN_COLOUR);
	}
}

int sys_changeWindow(unsigned int newIndex){
	if(newIndex < 0 || newIndex >= N || newIndex == activeWindow)
		return 0;			// permanece en la ventana actual
	else{
		activeWindow = newIndex;
		return 1;
	}
}

int sys_changeWindowColor(int rgb){
	if(rgb < 0 || rgb > 0xFFFFFF )
		return 0;	// no indica un color
	else{
		windows[activeWindow].charColor = rgb;
		return 1;
	}
}

//  Funcion de uso interno que realiza la actualizacion de las lineas del buffer de la ventana activa, 
// si se llego al limite de lineas mostrada se encarga de realizar el scrolling del buffer
static void updateBuffer(){
	
	Window *currentWindow = &(windows[activeWindow]);

	currentWindow->lineCount++;						// Se agrego una linea en pantalla
	currentWindow->currentLineSize = 0;

	if( currentWindow->lineCount == SCREEN_LINES ){
		// Ahora la pantalla va a arrancar en una posicion mas alta en el buffer
		currentWindow->firstLine = (currentWindow->firstLine + 1) % BUFFER_LINES;
		currentWindow->lineCount--;
		int lastLine = (currentWindow->firstLine + SCREEN_LINES -1) % BUFFER_LINES; // Linea que ahora estara alfinal de la pantalla vacia

		// Solo borro caracteres que quedaron pendientes, no toda la linea
		for(int i = 0; i < MAX_LINE_CHARS && currentWindow->screenBuffer[lastLine][i].character != 0 ; i++ ){
			currentWindow->screenBuffer[lastLine][i].character = 0;
		}
	}
}

//  Funcion de uso interno que realiza el refresco de cada linea de la ventana actual, 
// modificando caracter por caracter, en caso de ser necesario, 
// teniendo en cuenta lo que se tenia escrito en pantalla(linea vieja) y lo que se esta por escribir(linea nueva)
static void refreshLine( int lineNumber ){
	// recordar que el updateBuffer me movio el firstLine una posicion

	Window *currentWindow = &(windows[activeWindow]);

	int i = 0;																					// indice para iterar sobre la linea
	int currentLineIndex = (currentWindow->firstLine + lineNumber) % BUFFER_LINES;		// indice de la linea en el buffer que debe ser impresa en lineNumber
	int previousLineIndex;																		// indice de la linea en el buffer que se encuentra antes del ciclo escrita en pantalla

	if( currentWindow->firstLine == 0 && lineNumber == 0 )
		previousLineIndex = BUFFER_LINES -1;								
	else
		previousLineIndex = (currentWindow->firstLine + lineNumber -1) % BUFFER_LINES;		// Ya me asegure que el indice sera positivo

	charWithColor *currentLine = currentWindow->screenBuffer[currentLineIndex];
	charWithColor *previousLine = currentWindow->screenBuffer[previousLineIndex];

	// En este primer ciclo sobreescribire los chars que no comparten la linea que se encuentra en pantalla y la que se debe escribir
	// Finaliza una vez terminada la linea o una de las lineas
	for( ; i < MAX_LINE_CHARS && previousLine[i].character != 0 && currentLine[i].character != 0 ; i++ )
	{
		if(previousLine[i].character != currentLine[i].character || previousLine[i].color != currentLine[i].color){
			drawChar(currentLine[i].character, currentWindow->xStart + i * FONT_WIDTH, 
				currentWindow->yStart + lineNumber * (LINE_HEIGHT) + LINE_MARGIN, currentLine[i].color, BACKGROUND_COLOR);
		}
	}

	// En caso de que la nueva linea sea mas corta que la anterior, se deben borrar en pantalla las letras que quedaron
	while( previousLine[i].character != 0 && i < MAX_LINE_CHARS) {
		drawChar(0, currentWindow->xStart + i * FONT_WIDTH, 
			currentWindow->yStart + lineNumber * (LINE_HEIGHT) + LINE_MARGIN, CHAR_COLOR, BACKGROUND_COLOR);
		i++;
	}

	// En caso de que la nueva linea sea mas larga que la anterior, se deben agregar en pantalla estas letras sobrantes
	while( currentLine[i].character != 0 && i < MAX_LINE_CHARS ) {
		drawChar(currentLine[i].character, currentWindow->xStart + i * FONT_WIDTH, 
			currentWindow->yStart + lineNumber * (LINE_HEIGHT) + LINE_MARGIN, currentLine[i].color, BACKGROUND_COLOR);
		i++;
	}
}

// Funcion interna que realiza el refresco de todas las lineas en pantalla al llegarse al maximo de lineas
static void refreshScreen() {
	for(int i = 0 ; i < SCREEN_LINES; i++) {
		refreshLine(i);
	}
}

// Funcion interna que se encarga de eliminar un caracter en la ventana actual
static void deleteChar(){
	Window *currentWindow = &(windows[activeWindow]);
	if(currentWindow->currentLineSize == 0){
		if(currentWindow->lineCount == 0)
			// No existen caracteres por borrar
			return;

		currentWindow->lineCount--;
		currentWindow->currentLineSize = MAX_LINE_CHARS;
	}
	currentWindow->currentLineSize--;	// lo borro del buffer

	int currentLine = (currentWindow->firstLine + currentWindow->lineCount) % BUFFER_LINES;
	int currentChar = currentWindow->currentLineSize;
	charWithColor aux = {0,0};
	currentWindow->screenBuffer[currentLine][currentChar] = aux;	// Borro el caracter en el buffer
	drawChar(' ', currentWindow->xStart + currentWindow->currentLineSize * FONT_WIDTH, currentWindow->yStart + currentWindow->lineCount * LINE_HEIGHT + LINE_MARGIN, BACKGROUND_COLOR, BACKGROUND_COLOR);
}

static void setNewLine(){
	if(windows[activeWindow].lineCount == (SCREEN_LINES -1)) {
		// Se llego alfinal de las lineas en pantalla, se debe subir una linea para que la ultima quede libre
		updateBuffer();
		refreshScreen();
	}else
		updateBuffer();
}

static int printChar( char c, int rgb ){
	if ( c == '\n'){
		setNewLine();
		return 0;
	}else if( c == '\b'){
		deleteChar();
		return 0;
	}else if( c == '\t' ){
		for( int i = 0; i < 4; i++ ){	// tab = 4 espacios
			printChar( 32, rgb );
		}
		return 0;
	}

	Window *currentWindow = &(windows[activeWindow]);

	if( currentWindow->currentLineSize == MAX_LINE_CHARS ){
		// Se llego al tope de caracteres que entran en una linea, se debe pasar a la siguiente
		setNewLine();
	}

	// Escribo en pantalla el nuevo caracter en la linea y posicion actual, y luego incremento la posicion para el proximo caracter
	drawChar( c, currentWindow->xStart + currentWindow->currentLineSize * FONT_WIDTH, 
		currentWindow->yStart + currentWindow->lineCount * (LINE_HEIGHT) + LINE_MARGIN, rgb, BACKGROUND_COLOR);
	
	// Cambio el caracter en el buffer
	//*(windows[activeWindow].screenBuffer[ (windows[activeWindow].firstLine + windows[activeWindow].lineCount) % BUFFER_LINES ] +
	//windows[activeWindow].currentLineSize) = c;

	int currentLine = (currentWindow->firstLine + currentWindow->lineCount) % BUFFER_LINES;
	int currentChar = currentWindow->currentLineSize;

	charWithColor newChar = {c, rgb};
	currentWindow->screenBuffer[currentLine][currentChar] = newChar;
	
	// Incremento contador de caracteres en linea
	currentWindow->currentLineSize++;

	return 0;
}

int sys_write(unsigned int fd, const char * str, unsigned long count){
	
	int color = windows[activeWindow].charColor;

	if (fd == 2)	//	STDERR
		color = 0xFF0000;
	else if (fd != 1)	//	Solo tenemos STDOUT y STDERR
		return 0;

	for( int i = 0; i < count; i++ ){
		printChar( str[i], color );
	}

	return count;
}
