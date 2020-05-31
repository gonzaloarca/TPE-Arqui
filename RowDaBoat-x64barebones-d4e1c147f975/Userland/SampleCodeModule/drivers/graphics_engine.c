#include <screenInfo.h>
#include <graphics_engine.h>
#include <window.h>

static Window windows[N] = {{0}};
static int activeWindow = 0;		// por default arranca en la terminal

// Funciones ASM que ejecutan las correspondientes syscalls
void writePixel(int x, int y, int rgb);
void drawChar( char c, int x, int y, int rgb, int backgroundColour );
int read( unsigned int fd, char *buffer, unsigned long count ); //funcion de ASM
void emptyBuffer();	//funcion de ASM
char getKey();	//funcion de ASM
void _hlt();

void setWindows(){
	for(int i = 0 ; i < N ; i++){
		windows[i].xStart = i*WINDOW_WIDTH + WINDOW_MARGIN;
		windows[i].yStart = WINDOW_MARGIN;
		windows[i].firstLine = 0;
		windows[i].lineCount = 0;
		windows[i].currentLineSize = 0;
		// Borde izquierdo
		for(int j = i*WINDOW_WIDTH; j < i*WINDOW_WIDTH+WINDOW_MARGIN ; j++)
			for(int k = WINDOW_MARGIN; k < WINDOW_HEIGHT - WINDOW_MARGIN; k++)		// no abarca nada del borde superior
				writePixel( j, k, WINDOW_MARGIN_COLOUR);
		// Borde derecho
		for(int j = (i+1)*WINDOW_WIDTH- WINDOW_MARGIN; j < (i+1)*WINDOW_WIDTH ; j++)
			for(int k = WINDOW_MARGIN; k < WINDOW_HEIGHT - WINDOW_MARGIN; k++)
				writePixel( j, k, WINDOW_MARGIN_COLOUR);
		// Borde superior
		for(int j = 0 ; j < WINDOW_MARGIN ; j++)
			for(int k = i*WINDOW_WIDTH; k < (i+1)*WINDOW_WIDTH ; k++)
				writePixel( k, j, WINDOW_MARGIN_COLOUR);
		// Borde inferior
		for(int j = TOTAL_HEIGHT - WINDOW_MARGIN ; j < TOTAL_HEIGHT ; j++)
			for(int k = i*WINDOW_WIDTH; k < (i+1)*WINDOW_WIDTH ; k++)
				writePixel( k, j, WINDOW_MARGIN_COLOUR);
	}
}

void switchWindow(int newIndex){
	if(newIndex < 0 || newIndex >= N || newIndex == activeWindow)
		return;			// permanece en la ventana actual
	else
		activeWindow = newIndex;
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
		for(int i = 0; i < MAX_LINE_CHARS && currentWindow->screenBuffer[lastLine][i] != 0 ; i++ ){
			currentWindow->screenBuffer[lastLine][i] = 0;
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

	char *currentLine = currentWindow->screenBuffer[currentLineIndex];
	char *previousLine = currentWindow->screenBuffer[previousLineIndex];

	// En este primer ciclo sobreescribire los chars que no comparten la linea que se encuentra en pantalla y la que se debe escribir
	// Finaliza una vez terminada la linea o una de las lineas
	for( ; i < MAX_LINE_CHARS && previousLine[i] != 0 && currentLine[i] != 0 ; i++ )
	{
		if(previousLine[i] != currentLine[i]){
			drawChar(currentLine[i], currentWindow->xStart + i * FONT_WIDTH, 
				currentWindow->yStart + lineNumber * (LINE_HEIGHT) + LINE_MARGIN, CHAR_COLOUR, BACKGROUND_COLOUR);
		}
	}

	// En caso de que la nueva linea sea mas corta que la anterior, se deben borrar en pantalla las letras que quedaron
	while( previousLine[i] != 0 && i < MAX_LINE_CHARS) {
		drawChar(0, currentWindow->xStart + i * FONT_WIDTH, 
			currentWindow->yStart + lineNumber * (LINE_HEIGHT) + LINE_MARGIN, CHAR_COLOUR, BACKGROUND_COLOUR);
		i++;
	}

	// En caso de que la nueva linea sea mas larga que la anterior, se deben agregar en pantalla estas letras sobrantes
	while( currentLine[i] != 0 && i < MAX_LINE_CHARS ) {
		drawChar(currentLine[i], currentWindow->xStart + i * FONT_WIDTH, 
			currentWindow->yStart + lineNumber * (LINE_HEIGHT) + LINE_MARGIN, CHAR_COLOUR, BACKGROUND_COLOUR);
		i++;
	}
}

void refreshScreen() {
	for(int i = 0 ; i < SCREEN_LINES; i++) {
		refreshLine(i);
	}
}

// Funcion interna que se encarga de eliminar un caracter en la ventana actual
static void delete(){
	Window *currentWindow = &(windows[activeWindow]);
	if(currentWindow->currentLineSize == 0){
		if(currentWindow->lineCount == 0)
			// No existen caracteres por borrar
			return;

		currentWindow->lineCount--;
		currentWindow->currentLineSize = MAX_LINE_CHARS;
	}
	currentWindow->currentLineSize--;
	drawChar(' ', currentWindow->xStart + currentWindow->currentLineSize * FONT_WIDTH, currentWindow->yStart + currentWindow->lineCount * LINE_HEIGHT + LINE_MARGIN, BACKGROUND_COLOUR, BACKGROUND_COLOUR);
}

int printChar( char c, int rgb ){
	if ( c == '\n')
	{
		newLine();
		return 0;
	}else if( c == '\b'){
		delete();
		return 0;
	}	
	Window *currentWindow = &(windows[activeWindow]);

	if( currentWindow->currentLineSize == MAX_LINE_CHARS ){
		// Se llego al tope de caracteres que entran en una linea, se debe pasar a la siguiente
		newLine();
	}

	// Escribo en pantalla el nuevo caracter en la linea y posicion actual, y luego incremento la posicion para el proximo caracter
	drawChar( c, currentWindow->xStart + currentWindow->currentLineSize * FONT_WIDTH, 
		currentWindow->yStart + currentWindow->lineCount * (LINE_HEIGHT) + LINE_MARGIN, rgb, BACKGROUND_COLOUR);
	
	// Cambio el caracter en el buffer
	//*(windows[activeWindow].screenBuffer[ (windows[activeWindow].firstLine + windows[activeWindow].lineCount) % BUFFER_LINES ] +
	//windows[activeWindow].currentLineSize) = c;

	int currentLine = (currentWindow->firstLine + currentWindow->lineCount) %BUFFER_LINES;
	int currentChar = currentWindow->currentLineSize;

	currentWindow->screenBuffer[currentLine][currentChar] = c;
	
	// Incremento contador de caracteres en linea
	currentWindow->currentLineSize++;

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
	if(windows[activeWindow].lineCount == (SCREEN_LINES -1)) {
		// Se llego alfinal de las lineas en pantalla, se debe subir una linea para que la ultima quede libre
		updateBuffer();
		refreshScreen();
	}else
		updateBuffer();
}

void getInput( char *inputBuffer, unsigned int buffer_size ){

	emptyBuffer();
	char c = 0;
	int ctrl = 0;
	int i = 0;

	while( c != '\n' && i < buffer_size ){

		while( ( c = getKey() ) == 0 ){
			_hlt();
		}	
	
		switch( c ){
			case 17:		//codigo ASCII asignado al make code del Ctrl
				ctrl = 1;
				break;
			
			case 18:		//codigo ASCII asignado al break code del Ctrl
				ctrl = 0;
				break;
			
			case '\b':
				if( i == 0 ){
					break;
				}
				i--;
				printChar( c, CHAR_COLOUR );
				break;
			
			case '1':
				if( ctrl && activeWindow != 0 ){
					//insert rutina de cambiar de programa here
					break;
				}
				inputBuffer[i++] = c;
				printChar( c, CHAR_COLOUR );
				break;
			
			case '2':
				if( ctrl && activeWindow != 1 ){
					//insert rutina de cambiar de programa here
					break;
				}
			
			default:
				inputBuffer[i++] = c;
				printChar( c, CHAR_COLOUR );	
		}
	}
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