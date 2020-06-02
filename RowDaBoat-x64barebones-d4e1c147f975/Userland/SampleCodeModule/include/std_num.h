#ifndef STD_NUM_H

#define STD_NUM_H

#define DOUBLE_SIZE 16
//	Digitos de precision para el double
#define PRECISION 4	

//	Convierte number a un string guardandolo en buffer
//	Devuelve su longitud
int floatToString(double number, char buffer[DOUBLE_SIZE]);

//	Convierte un integer a String
//	Devuelve la longitud del buffer
int intToString( long int num, char * str );

#endif