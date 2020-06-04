#ifndef STD_NUM_H

#define STD_NUM_H

#define DOUBLE_SIZE 16
//	Digitos de precision para el double
#define PRECISION 4	

#define MAX_INT_DIG 16

//	Convierte number a un string guardandolo en buffer
//	Devuelve su longitud
int floatToString(double number, char buffer[DOUBLE_SIZE]);

//	Convierte un integer a String
//	Devuelve la longitud del string creado
int intToString( long int num, char buffer[MAX_INT_DIG] );

//	Convierte un integer a String en formato hexadecimal
//	Devuelve la longitud del string creado
int intToHexString(unsigned long int num, char buffer[MAX_INT_DIG + 1]);

#endif