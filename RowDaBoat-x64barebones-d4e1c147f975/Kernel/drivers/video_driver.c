#include "video_driver.h"
#include <stdint.h>
#include "font.h"

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
struct vbe_mode_info_structure * screen_info = (struct vbe_mode_info_structure *) 0x5C00;
// Busco ahora dentro de la estructura la variable framebuffer que es donde se empieza a escribir
// La resolucion de vesa es:
int WIDTH = 1024;
int HEIGHT = 768;

//Tamaño de la fuente
int FONT_HEIGHT = 14;
int FONT_WIDTH = 8;

//Instancia de array 3D de pantalla (taria bueno ver como hacer pa tenerla afuera)
// static uint32_t (*screen)[10][3] = (screen_info->framebuffer);

// char * getPixelDataByPosition(int x, int y) {
// 	return screen[x][y];
// }

int writePixel(Pixel p) {
	// char rojo = 255, verde = 255, azul = 255;

	// char * pos = getPixelDataByPosition(0,0);			// equivalente a &(screen_info).framebuffer
	// pos[0] = azul;
	// pos[1] = verde;
	// pos[2] = rojo;			// 3 variables que indican el color por pixel
	return 0;
}

int drawChar( char c, Point2D pos, int rgb ){
	char (*screen)[screen_info->width][3] = (char (*)[(screen_info->width)][3]) ((uint64_t)screen_info->framebuffer);
	const unsigned char * letra= getCharMap(c);
	int x_init = pos.x;
	int y_init = pos.y;
	char r = (rgb >> 16) & 0xFF;
	char g = (rgb >> 8) & 0xFF;
	char b = rgb & 0xFF;

	for( int i = 0; i < FONT_HEIGHT; i++ ){
		int row = letra[i];
		for( int j = FONT_WIDTH; row != 0; j-- ){
			int aux = row & 1;
			if( aux == 1 ){
				screen[y_init + i][x_init + j][0] = b;
				screen[y_init + i][x_init + j][1] = g;
				screen[y_init + i][x_init + j][2] = r;
			}
			row = row >> 1;
		}
	}	

	return 0;
}