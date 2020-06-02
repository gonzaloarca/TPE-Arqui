#include <graphics_engine.h>
#include <std_io.h>

int read( unsigned int fd, char *buffer, unsigned long count ); //funcion de ASM
void emptyBuffer();	//funcion de ASM
char getKey();	//funcion de ASM
void _hlt();
int changeWindow(unsigned int window);

static int activeWindow = 0;

int switchWindow( unsigned int window ){
	int success = changeWindow(window);
	if(success){
		activeWindow = window;
		return 1;
	}
	return 0;
}

int printString(  unsigned int count, char * str, int rgb ){
	write(count, str, rgb);

	return 0;
}

int printNullString( char * s, int rgb ){
	while( *s != 0 ){
		putchar( *s);
		s++;
	}

	return 0;
}

void newLine(){
	write( 1,"\n", 0);
}

int getInput( char *inputBuffer, unsigned int buffer_size ){

	emptyBuffer();
	char c = 0;
	int ctrl = 0;
	int i = 0;

	while( c != '\n' && i < buffer_size ){

		while( ( c = getKey() ) == 0 ){
			_hlt();
		}	
	
		switch( c ){
			case 17:		//codigo ASCII asignado al make code del Ctrl
				ctrl = 1;
				break;
			
			case 18:		//codigo ASCII asignado al break code del Ctrl
				ctrl = 0;
				break;
			
			case '\b':
				if( i == 0 ){
					break;
				}
				i--;
				putchar( c);
				break;
			
			case '1':
				if( ctrl && activeWindow != 0 ){
					switchWindow(0);
					break;
				}
				inputBuffer[i++] = c;
				putchar( c);
				break;
			
			case '2':
				if( ctrl && activeWindow != 1 ){
					switchWindow(1);
					break;
				}
			
			default:
				inputBuffer[i++] = c;
				putchar( c );	
		}
	}
	return i;
}

static void numToString( int num, char * str ){

	if( num == 0 ){
		str[0] = '0';
		str[1] = 0;
		return;
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
		num *= -1;		//porque num % n da negativo si num < 0
	}

	int i = dig;
	str[i--] = 0;

	while( num != 0 ){
		str[i--] = (num % 10) + '0';
		num /= 10;
	}
}

int printInt( int num, int rgb ){
	char buffer[32];
	numToString( num, buffer );
	printNullString(buffer, rgb);

	return 0;
}