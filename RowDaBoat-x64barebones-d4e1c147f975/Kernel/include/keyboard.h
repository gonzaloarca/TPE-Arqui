#ifndef _KEYBOARD_H
#define _KEYBOARD_H

//	Ver esta libreria para las funciones asembler de lectura
//	de los puertos 60h y 64h
#include <libasm64.h>
#include <stdint.h>
#include <registers.h>
#include <process_manager.h>

#define LEFT_SHIFT 42
#define RIGHT_SHIFT 54
#define CAPS_LOCK 58
#define LSHIFT_RELEASED (LEFT_SHIFT+0x80)
#define RSHIFT_RELEASED (RIGHT_SHIFT+0x80)
#define F1 59
#define F2 60
#define F3 61
#define BUFFER_SIZE 200

//	Funcion que se queda esperando a que toquen una tecla 
//	(o su combinacion con shift)
void keyboardPolling();

//	Convierte el Scan Code de una tecla a su Char correspondiente
//	"shift" indica si se presiono como combinacion shift+tecla
//	shift es 0 o 1 unicamente
char scanCodetoChar (unsigned int scan_code, unsigned int shift);

//	Esta funcion es la encargada de ver las interrupciones por teclado
//	En este caso, imprime en pantalla el caracter typeado
void keyboard_handler();

//  Vacía el buffer
void sys_emptyBuffer();

//	La syscall read() lee el buffer de teclado actual
//	Si hay teclas en el buffer de teclado, las guarda en out_buffer
//	y devuelve la cantidad de letras guardada
//	Si el buffer de teclado está vacío, no guarda nada y devuelve 0
uint64_t sys_read(char* out_buffer, unsigned long int count);

#endif
