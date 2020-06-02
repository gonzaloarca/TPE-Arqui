#include <stdint.h>
#include <std_io.h>
#include <syscalls.h>

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

	puts( timeString );
}