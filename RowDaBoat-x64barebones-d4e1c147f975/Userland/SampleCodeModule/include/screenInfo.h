#ifndef _SCREEN_INFO_H
#define _SCREEN_INFO_H

// Cantidad de pantallas
#define N 1

// Resolución de la pantalla (WIDTH x HEIGHT)
#define TOTAL_WIDTH 1024

// Resolucion de cada ventana
#define HEIGHT 768
#define WIDTH (TOTAL_WIDTH/N)		// Las pantallas se distribuyen horizontalmente

// Tamaño de la fuente
#define FONT_HEIGHT 14
#define FONT_WIDTH  8

// Ancho del renglon
#define LINE_MARGIN  1
#define LINE_HEIGHT (FONT_HEIGHT + 2*LINE_MARGIN)

// Máxima cantidad de renglones que pueden aparecer en cada ventana
#define SCREEN_LINES (HEIGHT/LINE_HEIGHT)
#define BUFFER_LINES (SCREEN_LINES +1)

// Máxima cantidad de caracteres que puede entrar en un renglon de una ventana
#define MAX_LINE_CHARS (WIDTH/FONT_WIDTH)

// Color de fondo y letra
#define BACKGROUND_COLOR 0		// negro
#define CHAR_COLOR 0xFFFFFF		// blanco	// that's racist


#endif