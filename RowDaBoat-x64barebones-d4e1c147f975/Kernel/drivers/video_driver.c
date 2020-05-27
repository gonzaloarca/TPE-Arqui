#include "video_driver.h"
#include <stdint.h>
#include "font.h"
#include "lib.h"

//Resolución de la pantalla (WIDTH x HEIGHT)
#define WIDTH 1024
#define HEIGHT 768

//Tamaño de la fuente
#define FONT_HEIGHT 14
#define FONT_WIDTH  8

//Ancho del renglon
#define LINE_MARGIN  1
#define LINE_WIDTH (FONT_HEIGHT + 2 * LINE_MARGIN)

//Máxima cantidad de renglones que pueden aparecer en pantalla
#define SCREEN_LINES  (HEIGHT/LINE_WIDTH)
#define BUFFER_LINES (SCREEN_LINES +1)


//Máxima cantidad de caracteres que puede entrar en un renglon
#define MAX_LINE_CHARS (WIDTH/FONT_WIDTH)

struct vbe_mode_info_structure {
	uint16_t attributes;		// deprecated, only bit 7 should be of interest to you, and it indicates the mode supports a linear frame buffer.
	uint8_t window_a;			// deprecated
	uint8_t window_b;			// deprecated
	uint16_t granularity;		// deprecated; used while calculating bank numbers
	uint16_t window_size;
	uint16_t segment_a;
	uint16_t segment_b;
	uint32_t win_func_ptr;		// deprecated; used to switch banks from protected mode without returning to real mode
	uint16_t pitch;			// number of bytes per horizontal line
	uint16_t width;			// width in pixels
	uint16_t height;			// height in pixels
	uint8_t w_char;			// unused...
	uint8_t y_char;			// ...
	uint8_t planes;
	uint8_t bpp;			// bits per pixel in this mode
	uint8_t banks;			// deprecated; total number of banks in this mode
	uint8_t memory_model;
	uint8_t bank_size;		// deprecated; size of a bank, almost always 64 KB but may be 16 KB...
	uint8_t image_pages;
	uint8_t reserved0;

	uint8_t red_mask;
	uint8_t red_position;
	uint8_t green_mask;
	uint8_t green_position;
	uint8_t blue_mask;
	uint8_t blue_position;
	uint8_t reserved_mask;
	uint8_t reserved_position;
	uint8_t direct_color_attributes;

	uint32_t framebuffer;		// physical address of the linear frame buffer; write here to draw to the screen
	uint32_t off_screen_mem_off;
	uint16_t off_screen_mem_size;	// size of memory in the framebuffer but not being displayed on the screen
	uint8_t reserved1[206];
} __attribute__ ((packed));


// Busco VBEModeInfoBlock que es donde se encuentra la estructura
struct vbe_mode_info_structure * screenInfo = (struct vbe_mode_info_structure *) 0x5C00;
// Busco ahora dentro de la estructura la variable framebuffer que es donde se empieza a escribir
// La resolucion de vesa es:

//Ultimas posiciones de la pantalla
int xLast = 0;
int yLast = 0;

//Buffer de la pantalla para permitir scrolling
char screenBuffer[BUFFER_LINES][MAX_LINE_CHARS] = {0}; // necesito una linea mas para tener la anterior

//Cantidad de caracteres en la linea actual
static int currentLineSize = 0;

//Cantidad de lineas escritas actualmente
static int lineCount = 0;

//Linea en la cual arranca la pantalla
static int firstLine = 0;

//Instancia de array 3D de pantalla (taria bueno ver como hacer pa tenerla afuera)
// static uint32_t (*screen)[10][3] = (screenInfo->framebuffer);

// char * getPixelDataByPosition(int x, int y) {
// 	return screen[x][y];
// }

int writePixel( int x, int y, int rgb ) {
	char (*screen)[screenInfo->width][3] = (char (*)[(screenInfo->width)][3]) ((uint64_t)screenInfo->framebuffer);
	screen[y][x][0] = rgb & 0xFF;
	screen[y][x][1] = (rgb >> 8) & 0xFF;
	screen[y][x][2] = (rgb >> 16) & 0xFF;
	return 0;
}

int drawChar( char c, int x, int y, int rgb ){	//dibujar un caracter dado su esquina izq superior y un color
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
				writePixel( x + j, y + i, 0 ); //negro por ahora
			}
			row = row >> 1;
		}
	}	

	return 0;
}

static void updateBuffer(){
	xLast = 0;
	lineCount++;					// Se agrego una linea en pantalla

	if( lineCount == SCREEN_LINES ){
		// Ahora la pantalla va a arrancar en una posicion mas alta en el buffer
		firstLine = (firstLine + 1) % BUFFER_LINES;
		lineCount--;
		int lastLine = (firstLine + SCREEN_LINES -1) % BUFFER_LINES; // Linea que ahora estara alfinal de la pantalla vacia

		// Solo borro caracteres que quedaron pendientes, no toda la linea
		for(int i = 0; i < MAX_LINE_CHARS && screenBuffer[lastLine][i] != 0 ; i++ ){
			screenBuffer[lastLine][i] = 0;
		}
	}

	yLast = lineCount * LINE_WIDTH;
	currentLineSize = 0;
}

static void refreshLine( int lineNumber ){
	// recordar que el updateBuffer me movio el firstline una posicion

	int i = 0;																// indice para iterar sobre la linea
	int currentLineIndex = (firstLine + lineNumber) % BUFFER_LINES;			// indice de la linea en el buffer que debe ser impresa en lineNumber
	int previousLineIndex;													// indice de la linea en el buffer que se encuentra antes del ciclo escrita en pantalla

	if( firstLine == 0 && lineNumber == 0 )
		previousLineIndex = BUFFER_LINES -1;								
	else
		previousLineIndex = (firstLine + lineNumber -1) % BUFFER_LINES;		// Ya me asegure que el indice sera positivo

	// En este primer ciclo sobreescribire los chars que no comparten la linea que se encuentra en pantalla y la que se debe escribir
	// Finaliza una vez terminada la linea o una de las lineas
	for( ; i < MAX_LINE_CHARS && screenBuffer[previousLineIndex][i] != 0 && screenBuffer[currentLineIndex][i] != 0 ; i++ ){
		if(screenBuffer[previousLineIndex][i] != screenBuffer[currentLineIndex][i]){
			drawChar( screenBuffer[currentLineIndex][i], i * FONT_WIDTH, lineNumber * (LINE_WIDTH) + LINE_MARGIN, 0xFFFFFF);
		}
	}

	// En caso de que la nueva linea sea mas corta que la anterior, se deben borrar en pantalla las letras que quedaron
	while( screenBuffer[previousLineIndex][i] != 0 && i < MAX_LINE_CHARS) {
		drawChar(0, i * FONT_WIDTH, lineNumber * (LINE_WIDTH) + LINE_MARGIN, 0xFFFFFF);
		i++;
	}

	// En caso de que la nueva linea sea mas larga que la anterior, se deben agregar en pantalla estas letras sobrantes
	while( screenBuffer[currentLineIndex][i] != 0 && i < MAX_LINE_CHARS ) {
		drawChar( screenBuffer[currentLineIndex][i], i * FONT_WIDTH, lineNumber * (LINE_WIDTH) + LINE_MARGIN, 0xFFFFFF);
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
		newline();
		return 0;
	}

	if( currentLineSize == MAX_LINE_CHARS ){
		// Se llego al tope de caracteres que entran en una linea, se debe pasar a la siguiente
		newline();
	}

	// Escribo en pantalla el nuevo caracter en la linea y posicion actual, y luego incremento la posicion para el proximo caracter
	drawChar( c, currentLineSize * FONT_WIDTH, lineCount * (LINE_WIDTH) + LINE_MARGIN, 0xFFFFFF);
	*(screenBuffer[ (firstLine + lineCount) % BUFFER_LINES] + currentLineSize) = c;
	currentLineSize++;
	

	xLast += FONT_WIDTH;

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

void newline(){
	if(lineCount == SCREEN_LINES -1) {
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

