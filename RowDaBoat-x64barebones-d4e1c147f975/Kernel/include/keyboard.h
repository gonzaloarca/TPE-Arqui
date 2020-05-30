#ifndef _KEYBOARD_H

#define _KEYBOARD_H

//	Ver esta libreria para las funciones asembler de lectura
//	de los puertos 60h y 64h
#include <libasm64.h>
#include <video_driver.h>

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

//  Levanta caracter del buffer del teclado y lo devuelve codificado en ASCII
char sys_getKey();

//  Vacía el buffer
void sys_emptyBuffer();

#endif