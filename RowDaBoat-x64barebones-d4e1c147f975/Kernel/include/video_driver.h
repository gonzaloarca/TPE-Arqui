#ifndef VIDEO_DRIVER_H_
#define VIDEO_DRIVER_H_

typedef struct{
	int x;
	int y;
} Point2D;

typedef struct{
	int rgb;
	Point2D pos;
} Pixel;




int writePixel(int x, int y, int rgb);

int drawChar(char c, int x, int y, int rgb);

int printChar( char c, int rgb );

int printString( char * str, unsigned int count, int rgb );

int printNullString( char * s, int rgb );

void newline();

void refreshScreen();

int printInt( int num, int rgb);

#endif