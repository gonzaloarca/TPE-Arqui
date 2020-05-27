#ifndef _libasm64_h

#define _libasm64_h

//	Funcion que ya venia
char *cpuVendor(char *result);

//	Estructura y funcion para obtener la hora del RTC
//	NOTA: puede ser que devuelva UTC, por lo que esta adelantado 3 horas
typedef struct TimeFormat
{
	unsigned int seconds;
	unsigned int minutes;
	unsigned int hours;
} TimeFormat;

TimeFormat *getTime(TimeFormat *time);

//	Funciones para lectura del teclado
unsigned int canReadKey();

unsigned int getScanCode();

#endif