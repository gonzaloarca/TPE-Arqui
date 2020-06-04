#ifndef _COMANDOS_H
#define _COMANDOS_H

typedef struct memType {
	uint64_t mem1;
	uint64_t mem2;
	uint64_t mem3;
	uint64_t mem4;
} memType;

//	Programa que imprime la hora actual
void printTime();

//	Programa que imprime informacion del CPU
void printCPUInfo();

// Programa que imprime el valor de 4 direcciones de memoria a partir de la indicada por parametro
void printmem(char* address);

// Programa que muestra informacion sobre posibles usos de la terminal
void help();

#endif