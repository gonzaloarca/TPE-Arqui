#include <stdint.h>
#include <std_io.h>
typedef struct{
    uint32_t secs;
    uint32_t mins;
    uint32_t hours;
}TimeFormat;

void getTime(TimeFormat *time); //llama a syscall que te rellena la estructura de TimeFormat

void printTime(){       //se podria modularizar en una funcion para obtener el tiempo correcto, que abarcaria hasta el while, y otra para generar el string formateado del tiempo, despues juega printString
    TimeFormat time;
	getTime( &time );
	
	char timeString[9];

    timeString[2] = ':';
	timeString[5] = ':';
	timeString[8] = 0;

    int data[3] = { time.hours, time.mins, time.secs };
    int j = 0; 
	for( int i = 0; i < 3; i++ ){
		if( data[i] < 10 ){
			timeString[j++] = '0';
		} else {
			timeString[j++] = (data[i] / 10) + '0';
		}
		timeString[j++] = (data[i] % 10) + '0';
		j++;
	}

	puts( timeString, 0xFFFFFF );
}