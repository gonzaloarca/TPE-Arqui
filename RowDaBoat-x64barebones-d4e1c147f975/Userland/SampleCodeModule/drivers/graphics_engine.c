#include <graphics_engine.h>
#include <std_io.h>
#include <syscalls.h>

//	NOTA: CAMBIARLE EL NOMBRE A ESTA LIBRERIA

static int activeWindow = 0;

int switchWindow( unsigned int window ){
	int success = changeWindow(window);
	if(success){
		activeWindow = window;
		return 1;
	}
	return 0;
}

int getInput( char *inputBuffer, unsigned int buffer_size ){

	emptyBuffer();
	char c = 0;
	int ctrl = 0;
	int i = 0;

	puts("$> ");

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
