#ifndef _libasm64_h

#define _libasm64_h

#include <stdint.h>

//	Indica si se puede leer el teclado (puerto 64h)
unsigned int canReadKey();

//	Devuelve la lectura del teclado (puerto 60h)
unsigned int getScanCode();

//	Devuelve la temperatura absoluta en ºC del CPU 
int sys_getCPUTemp();

typedef struct{
    uint32_t secs;
    uint32_t mins;
    uint32_t hours;
}TimeFormat;

//	Regresa el tiempo actual en un puntero a estructura
void getTimeRTC(TimeFormat *time);

#endif
