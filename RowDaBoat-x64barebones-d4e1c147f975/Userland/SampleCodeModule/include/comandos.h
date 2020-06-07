#ifndef _COMANDOS_H
#define _COMANDOS_H

#include <stdint.h>
#include <std_io.h>
#include <syscalls.h>

// Estructura utilizada para imprimir los valores de 32 bytes de memoria
typedef struct memType {
	uint64_t mem1;
	uint64_t mem2;
	uint64_t mem3;
	uint64_t mem4;
} memType;

// Estructura utilizada para manejar una copia de los registros realizada en algun momento
typedef struct RegistersType {
	uint64_t rax;
    uint64_t rbx;
    uint64_t rcx;
    uint64_t rdx;
    uint64_t rbp;
    uint64_t rdi;
    uint64_t rsi;
    uint64_t r8;
    uint64_t r9;
    uint64_t r10;
    uint64_t r11;
    uint64_t r12;
    uint64_t r13;
    uint64_t r14;
    uint64_t r15;
    uint64_t rsp;
    uint64_t rip;
} RegistersType;

//	Programa que imprime la hora actual
void printTime();

//	Programa que imprime informacion del CPU
void printCPUInfo();

//  Programa que imprime la temperatura actual del CPU
void printCPUTemp();

// Programa que imprime el valor de 4 direcciones de memoria a partir de la indicada por parametro
void printmem(char* address);

// Programa que muestra informacion sobre los posibles usos de la terminal
void help();

// Programa que imprime los valores de los registros que se tenian la ultima vez que se presiono f1
void printInforeg();

// Retorna estructura con los valores correspondientes a los 32 bytes siguientes a address
void getMemoryASM(memType* answer, uint64_t address);

//  Funcion que retorna una estructura con los valores de los registros de la ultima vez que se presiono la tecla especial F1
RegistersType* getRegisters();

// Programa para ejecutar excepcion de dividir por cero
void executeZeroException();

// Programa para ejecutar excepcion de instruccion indefinida
void executeUIException();

#endif