#include <std_num.h>

int floatToString(double number, char buffer[DOUBLE_SIZE])
{
	int length = 0;

	if (number < 0)
	{
		buffer[length++] = '-';
		number *= -1;
	}

	//	Obtengo la parte entera y la agrego al buffer
	long int integer = (long int) number;
	length += intToString(integer, &(buffer[length]));

	//	Obtengo la parte decimal hasta PRECISION digitos
	//	en forma de entero para reutilizar intToString
	number -= integer;
	for (int i = 0; i < PRECISION+1; i++)
		number *= 10;
	integer = (long int) number;
	//	Hago el redondeo
	if (integer % 10 >= 5) 
		integer += 10;
	integer /= 10;

	if (integer != 0)	// Veo que la parte decimal no sea cero
	{
		buffer[length++] = '.';	//	Agrego el punto
		length += intToString(integer, &(buffer[length]));
	}

	buffer[length] = 0;

	return length;
}

int intToString( long int num, char * str ){

    if( num == 0 ){
        str[0] = '0';
        str[1] = 0;
        return 1;
    }

    int dig = 0;
    int aux = num;

    while( aux != 0 ){ //cuento digitos para saber desde donde arrancar a meter los caracteres
        aux /= 10;
        dig++;
    }
    
    if( num < 0 ){
        dig++;
        str[0] = '-';
        num *= -1;      //porque num % n da negativo si num < 0
    }

    int i = dig;
    str[i--] = 0;

    while( num != 0 ){
        str[i--] = (num % 10) + '0';
        num /= 10;
    }

    return dig;
}
