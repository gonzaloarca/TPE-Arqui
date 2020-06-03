#ifndef _libasm64_h

#define _libasm64_h

//	Indica el fabricante del CPU
char *cpuVendor(char buffer[13]);

//	Indica la marca del CPU
char *cpuBrand(char buffer[48]);

//	Indica el modelo y la familia del CPU
void cpuModel(int buffer[2]);

//	Funciones para lectura del teclado
unsigned int canReadKey();

unsigned int getScanCode();

int sys_getCPUTemp();

#endif
