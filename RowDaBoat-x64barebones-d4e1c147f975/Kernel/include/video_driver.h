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




int writePixel(Pixel p);

int drawChar(char c, Point2D pos, int rgb);

#endif