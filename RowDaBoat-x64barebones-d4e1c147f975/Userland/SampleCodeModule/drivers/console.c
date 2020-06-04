#include <console.h>
#include <std_io.h>
#include <syscalls.h>

static int activeWindow = 0;

static int switchWindow( unsigned int window ){
	int success = changeWindow(window);
	if(success){
		activeWindow = window;
		return 1;
	}
	return 0;
}

int getInput( char *inputBuffer, unsigned long int buffer_size )
{
	emptyBuffer();
	int ctrl = 0, size = 0, keyboard_size, i;
	char c, keyboard[buffer_size];

	puts("$> ");

	while (size < buffer_size)	// Podria ser while(1) pero dejamos esta condicion por las dudas
	{
		//	Leo el buffer de teclado. Si estaba vacio, llamo a hlt
		//	para esperar a una interrupcion del teclado
		while ( (keyboard_size = read(keyboard, buffer_size)) == 0 )
			_hlt();

		for (i = 0; i < keyboard_size; i++)
		{
			c = keyboard[i];

			switch( c ){
				case 17:		//codigo ASCII asignado al make code del Ctrl
					ctrl = 1;
					break;
				
				case 18:		//codigo ASCII asignado al break code del Ctrl
					ctrl = 0;
					break;

				case '\n':
					inputBuffer[size++] = c;
					putchar(c);
					return size;
				
				case '\b':
					if( size == 0 ){
						break;
					}
					size--;
					putchar(c);
					break;
				
				default:
					if( ctrl && c == '1' && activeWindow != 0 ){
						switchWindow(0);
						break;
					}
					if( ctrl && c == '2' && activeWindow != 1 ){
						switchWindow(1);
						break;
					}
					// Solo lo guardo si tengo espacio en el buffer
					// Si no hay espacio, hay que esperar a las teclas especiales
					if (size < buffer_size - 1)
					{
						inputBuffer[size++] = c;
						putchar( c );
					}	
			}
		}
	}

	return size;
}
