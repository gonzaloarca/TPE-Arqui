#include <std_io.h>
#include <std_num.h>
#include <stdarg.h>   //para tener cantidad variable de parámetros en funciones como printf y scanf

#define N 4096       //maximo tamaño de conversion para cuando printf reciba strings como parametros



int putchar(char c){
	return write( 1, &c, 1 );
}

int strcmp(char *str1, char *str2){
	int i = 0, answer;
	char flag = 1;
	while(flag != 0)
    {
        if (str1[i] != str2[i])
        {
           	answer = (str1[i] < str2[i])? -1 : 1;
           	flag = 0;
        }

        if (str1[i] == 0)
        {
            answer = 0;
            flag = 0;
        }
        i++;
    }

    return answer;
}

int puts(char *str){
    return write( 1, str, strlen(str) );
}

int strlen(char *str){
    int i = 0;
    for( ; str[i] != '\0' ; i++);
    return i;
}

int printf( const char* format, ...){
    if( format == 0 ){
        return 0;
    }

    va_list args;
    va_start( args, format );

    
    int i = 0;
    int count = 0;
    /*
    char *s;
    long d;
    unsigned long ud;
    double f;
    char c;
    */
    char numAux[32];


    while( format[i] ){
        if( format[i] != '%' ){
            if( format[i] != '\\' ){
                putchar( format[i++] );
                count++;
            } else {
                i++;
                switch( format[i] ){  //puede llegar a ser un caracter especial, como un enter o un tab
                    case 'n':
                        putchar('\n');
                        i++;
                        count++;
                        break;
                    case 't':
                        putchar('\t');
                        i++; 
                        count++;
                        break;
                    default:
                        putchar( '\\' );
                        count++;
                        break;
                }   
            }
     
        } else { // Encontre un %
            i++;
            switch( format[i] ){
                case 's': 
                    puts( va_arg( args, char *) );
                    i++;
                    break;          
                case 'd':
                    count += intToString( va_arg( args, int ), numAux );
                    puts( numAux );
                    i++;
                    break;
                case 'u':
                    count += intToString( (unsigned int) va_arg( args, unsigned int ), numAux );
                    puts( numAux );
                    i++;
                    break;
                case 'c':
                    putchar( va_arg( args, char ) );
                    i++;
                    count++;
                    break;
                case 'f': 
                    count += floatToString( va_arg( args, float ), numAux );
                    puts( numAux );
                    i++;
                    break;
                case 'g':   
                    count += floatToString( va_arg( args, double ), numAux );
                    puts( numAux );
                    i++;
                    break; 
                case 'l':
                    if( format[i+1] == 'u' ){   //unsigned long
                        count += intToString((unsigned long) va_arg( args, unsigned long ), numAux);
                        puts( numAux ); 
                        i += 2;
                        break;
                    }
                    if( format[i+1] == 'd' ){   //long
                        count += intToString( (long) va_arg( args, long ), numAux );
                        puts( numAux );
                        i += 2;
                        break;
                    } //si despues de la l no habia nada de interes, debo printear el % y lo que siga, lo cual cae en el caso default
                default:
                    putchar('%');
                    if( format[i] == '%' ){ //si me pasan %%, puedo ahorrarme las comparaciones que siguen si hago que directamente el siguiente % ni entre al switch salteandolo
                        i++;                
                    }
                    count++;
            }
        }
    }
    
    va_end(args);
    return count;

}
