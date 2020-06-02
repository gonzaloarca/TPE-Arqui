#ifndef _libasm64_h

#define _libasm64_h

//	Indica el fabricante del CPU
char *cpuVendor(char buffer[12]);

//	Indica la marca del CPU
char *cpuBrand(char buffer[48]);

//	Regresa el tiempo actual en un puntero a estructura
//void getTimeRTC(TimeFormat *time);

//	Funciones para lectura del teclado
unsigned int canReadKey();

unsigned int getScanCode();

int sys_getCPUTemp();

#endif
