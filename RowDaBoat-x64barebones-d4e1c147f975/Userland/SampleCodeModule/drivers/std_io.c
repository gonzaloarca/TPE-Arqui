#include <std_io.h>
#include <std_num.h>
#include <stdarg.h>   //para tener cantidad variable de parámetros en funciones como printf y scanf
#include <graphics_engine.h> //para llamar a getInput desde getchar hace falta

#define N 4096       //maximo tamaño de conversion para cuando printf reciba strings como parametros ?? hay que ver si ponemos limite a esto no
#define STDIN_MAXBUFFER 4096

static char stdinBuffer[STDIN_MAXBUFFER];
static int stdinFirstPos = 0;
static int stdinLastPos = 0;

int getchar(){  //devuelve chars casteados a int porque no hacemos uso de caracteres como EOF, ya que no hay un filesystem. Por ende no hay necesidad de operar con enteros en principio
    if( stdinLastPos == stdinFirstPos ){ //hay que ver que pasa si stdinLastPos > STDIN_BUFFERSIZE
        stdinFirstPos = 0;
        stdinLastPos = getInput( stdinBuffer, STDIN_MAXBUFFER );
    }

    return stdinBuffer[stdinFirstPos++];
}

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
                    putchar( (char) va_arg( args, int ) ); //str_arg.h promueve los chars a int
                    i++;
                    count++;
                    break;
                case 'f':   //al usar std_arg.h, cuando obtengo va_arg( args, float ), el float se promueve a double
                case 'g':   //son el mismo caso por lo tanto
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
