#include <console.h>

int getInput( char *inputBuffer, unsigned long int buffer_size, char *promptSymbol, char promptDelimiter )
{
	emptyBuffer();
	int size = 0, keyboard_size, i;
	char c, keyboard[buffer_size];

	puts(promptSymbol);

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
				case '\b':
					if( size == 0 ){
						break;
					}
					size--;
					putchar(c);
					break;
				
				case '\t':
					for( int k = 0; k < 4 && size + k < buffer_size -1; k++ ){	// tab = 4 espacios, se agregan solo si hay espacio en el buffer
						inputBuffer[size++] = ' '	;
						putchar(' ');
					}
					break;

				case 19:		//codigo ASCII asginado al make code de la tecla F2
					// Borro la linea actual
					while(size>0){
						putchar('\b');
						size--;
					}
					break;

				case '\n':
					if (promptDelimiter != '\n')
						break;	// para que no haga saltos de linea en la calculadora
					// si no es entra en el siguiente caso

				default:
					if (c == promptDelimiter || size < buffer_size - 1)
					{
						inputBuffer[size++] = c;
						putchar(c);
						if (c == promptDelimiter) {
							return size;
						}
					}	
			}
		}
	}

	return size;
}

char waitInput()
{
	char c;

	emptyBuffer();

	while ( read(&c, 1) == 0 )
			_hlt();

	return c;
}
