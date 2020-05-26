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
#define MAX_LINES 5
//(HEIGHT/LINE_WIDTH)

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
char screenBuffer[(MAX_LINES) - 1][WIDTH] = {0}; //WIDTH + 1 para que los strings puedan ser null terminated

//Buffer de la linea actual que se escribe en pantalla
static char currentLine[WIDTH];

//Cantidad de caracteres en la linea actual
static int currentLineSize = 0;

//Cantidad de lineas escritas actualmente
static int lineCount = 0;

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
	static int cont = 0;
	if( letra == 0 ){
		cont++;
		if( cont == 5 ){
			return 0;
		}
		letra = getCharMap(32);		//debe printear vacio
	}
	for( int i = 0; i < FONT_HEIGHT; i++ ){
		int row = letra[i];
		for( int j = FONT_WIDTH - 1; j >= 0; j-- ){
			int aux = row & 1;
			if( aux == 1 ){
				writePixel( x + j, y + i, rgb);
			}
			else {
				writePixel( x + j, y + i, 0xFF0000 ); //negro por ahora
			}
			row = row >> 1;
		}
	}	

	return 0;
}

// int eraseChar( int x, int y ){
// 	for( int i = 0; i < FONT_HEIGHT; i++ ){
// 		for( int j = 0; j < FONT_WIDTH; j++ ){
// 			writePixel( x, y, 0 ); //en negro por ahora, hay que ver si vamos a permitir colores de ventana
// 		}
// 	}

// 	return 0;
// }



// int printChar( char c, int rgb ){ old printChar
// 	Point2D pos = { xLast, yLast };

// 	if( xLast + 8 >= WIDTH ){
// 		newline();
// 		pos.x = xLast;
// 		pos.y = yLast;	//si no entra el caracter entero en la linea, debo printearlo directamente en la sig
// 	} else {
// 		xLast += 8; //ta mal esto, no es siempre 8
// 	}

// 	drawChar( c, pos, rgb );
// 	return 0;
// }



static void updateBuffer(){

	xLast = 0;
	int screenBuffIdx = lineCount; //defino aca el indice en donde debo agregar el string al buffer porque si esta lleno el buffer, se va a poner en lineCount - 2

	if( lineCount != MAX_LINES ){ //solo debo incrementar la posicion de yLast y de las lineas si no estoy parado en la ultima linea
		lineCount++;
		yLast = lineCount * LINE_WIDTH; //lineCount podria reemplazarse con yLast / LINE_WIDTH, pero hay que ver que otros usos le vamos a dar a yLast
	}

	if( lineCount == MAX_LINES ){ //debo mover todo el vector 1 lugar para la izquierda si se lleno el buffer
		memcpy( (void *) screenBuffer, (void *) screenBuffer[1], (MAX_LINES - 1)*(WIDTH) );
		lineCount--;
		yLast = lineCount * LINE_WIDTH;
	}
	
	memcpy( (void *) screenBuffer[screenBuffIdx], (void *) currentLine, WIDTH );
	memset( currentLine, 0, WIDTH );
	currentLineSize = 0;
}

static void refreshChar( int lineNumber, int charIndex ){
	drawChar( screenBuffer[lineNumber][charIndex], charIndex * FONT_WIDTH, lineNumber * (LINE_WIDTH) + LINE_MARGIN, 0xFFFFFF ); //blanco pa testear
}																			//le sumo el margen para que quede centrado 

static void refreshLine( int lineNumber ){
	for( int i = 0; i < WIDTH; i++ ){
		refreshChar( lineNumber, i );
	}	
}

void refreshScreen(){
	for( int i = 0; i < lineCount; i++ ){
		refreshLine( i );
	}
}

int printChar( char c, int rgb ){
	if( currentLineSize == WIDTH - 1 ){
		newline();+
	}

	*(currentLine + currentLineSize) = c;
	currentLineSize++;
	xLast += FONT_WIDTH;
	
	//refreshChar( lineCount, (xLast - 1) / FONT_WIDTH );

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
	updateBuffer();
	refreshScreen();
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

