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

int fputc(char c, int fd){
    if(fd != 1 && fd != 2){  //solo tenemos implementados stdout y stderr
        return -1;
    }

    return write( fd, &c, 1 );
}

int putchar(char c){
	return fputc(c, 1);
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

int fputs(char *str, int fd){
    if(fd != 1 && fd != 2){  //solo tenemos implementados stdout y stderr
        return -1;
    }
    return write(fd, str, strlen(str));
}

int puts(char *str){
    return fputs(str, 1);
}

int vfprintf(int fd, char* format, va_list arg){
    if(format == 0 || (fd != 1 && fd != 2 )){   //solo tenemos implementados stdout y stderr
        return -1;
    }
 
    int i = 0;
    int count = 0;

    char numAux[32];

    while(format[i]){
        if(format[i] != '%'){
            if(format[i] != '\\'){
                fputc(format[i++], fd);
                count++;
            } else {
                i++;
                switch(format[i]){  //puede llegar a ser un caracter especial, como un enter o un tab
                    case 'n':
                        fputc('\n', fd);
                        i++;
                        count++;
                        break;
                    case 't':
                        fputc('\t', fd);
                        i++; 
                        count++;
                        break;
                    default:
                        fputc('\\', fd);
                        count++;
                        break;
                }   
            }
     
        } else { // Encontre un %
            i++;
            switch(format[i]){
                case 's': 
                    fputs(va_arg(arg, char *), fd);
                    i++;
                    break;
                case 'p':   //puntero
                    count += pointerToHexString(va_arg(arg, void *), numAux);
                    fputs(numAux, fd);
                    i++;
                    break;
                case 'x':
                    count += intToHexString((unsigned int) va_arg(arg, unsigned int), numAux);
                    fputs(numAux, fd);
                    i++;
                    break;
                case 'd':
                    count += intToString(va_arg(arg, int), numAux);
                    fputs(numAux, fd);
                    i++;
                    break;
                case 'u':
                    count += intToString((unsigned int) va_arg(arg, unsigned int), numAux);
                    fputs(numAux, fd);
                    i++;
                    break;
                case 'c':
                    fputc((char) va_arg(arg, int), fd); //str_arg.h promueve los chars a int
                    i++;
                    count++;
                    break;
                case 'f':   //al usar std_arg.h, cuando obtengo va_arg( arg, float ), el float se promueve a double
                case 'g':   //son el mismo caso por lo tanto
                    count += floatToString(va_arg(arg, double), numAux);
                    fputs(numAux, fd);
                    i++;
                    break; 
                case 'l':
                    if( format[i+1] == 'u' ){   //unsigned long
                        count += intToString((unsigned long) va_arg(arg, unsigned long), numAux);
                        fputs(numAux, fd); 
                        i += 2;
                        break;
                    }
                    if( format[i+1] == 'd' ){   //long
                        count += intToString((long) va_arg(arg, long), numAux);
                        fputs(numAux, fd);
                        i += 2;
                        break;
                    } 
                    if( format[i+1] == 'x' ){   //unsigned long en hexa
                        count += intToHexString((unsigned long) va_arg(arg, unsigned long), numAux);
                        fputs(numAux, fd);
                        i += 2;
                        break;
                    } 
                    //si despues de la l no habia nada de interes, debo printear el % y lo que siga, lo cual cae en el caso default
                default:
                    fputc('%', fd);
                    if( format[i] == '%' ){ //si me pasan %%, puedo ahorrarme las comparaciones que siguen si hago que directamente el siguiente % ni entre al switch salteandolo
                        i++;                
                    }
                    count++;
            }
        }
    }

    return count;
}

int fprintf( int fd, char* format, ...){
    va_list arg;
    va_start(arg, format);

    int ret = vfprintf(fd, format, arg);

    va_end(arg);

    return ret;
}


int printf(char* format, ...){
    va_list arg;
    va_start(arg, format);

    int ret = vfprintf(1, format, arg);

    va_end(arg);
    
    return ret;    
}

