#ifndef VIDEO_DRIVER_H_
#define VIDEO_DRIVER_H_

int sys_writePixel( int x, int y, int rgb );

void drawChar( char c, int x, int y, int rgb, int backgroundColour );

#endif