#ifndef _SCREEN_INFO_H
#define _SCREEN_INFO_H

// Cantidad de pantallas
#define N 2

// Tamanio y color del margen entre ventanas
#define WINDOW_MARGIN 4
#define WINDOW_MARGIN_COLOUR 0xFFFF0

// Resolución de la pantalla (WIDTH x HEIGHT)
#define TOTAL_WIDTH 1024
#define TOTAL_HEIGHT 768

// Resolucion de cada ventana
#define WINDOW_HEIGHT 768		// Le saco el margen de arriba y de abajo
#define WINDOW_WIDTH (TOTAL_WIDTH/N)				// Las pantallas se distribuyen horizontalmente

// Tamaño de la fuente default
#define FONT_HEIGHT 14
#define FONT_WIDTH  8

// Ancho del renglon
#define LINE_MARGIN  1
#define LINE_HEIGHT (FONT_HEIGHT + 2*LINE_MARGIN)

// Máxima cantidad de renglones que pueden aparecer en cada ventana
#define SCREEN_LINES ((TOTAL_HEIGHT - 2*WINDOW_MARGIN)/LINE_HEIGHT)
#define BUFFER_LINES (SCREEN_LINES + 1)

// Máxima cantidad de caracteres que puede entrar en un renglon de una ventana
#define MAX_LINE_CHARS ((WINDOW_WIDTH - 2*WINDOW_MARGIN)/FONT_WIDTH)		// Le saco los margenes de los costados

// Color de fondo y letra
#define BACKGROUND_COLOUR 0		// negro
#define CHAR_COLOUR 0xFFFFFF		// blanco	// that's racist


#endif
