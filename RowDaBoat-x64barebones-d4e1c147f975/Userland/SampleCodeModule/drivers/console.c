#include <console.h>
#include <std_io.h>
#include <syscalls.h>
#include <frame.h>

#define WINDOWS 2
#define MB	0x100000

//	Lista de modulos/programas
static Module modules[WINDOWS];
static unsigned int activeModule = 0;
static unsigned int numberOfModules = 0;

//	Reservo espacio para los stack frames
static char reserve[WINDOWS][MB];

int initModule(void (*program)(), char prompt[MAX_PROMPT], char delimiter)
{
	int i;

	//	Si ya no tengo mas ventanas, no puedo agregar nada
	if (numberOfModules >= WINDOWS)
		return 0;

	//	Busco la direccion del nuevo modulo en la lista
	Module *newModule = &(modules[numberOfModules]);

	//	Copio los parametros al modulo
	newModule->program = program;
	newModule->delimiter = delimiter;
	for (i = 0; prompt[i] != 0 && i < MAX_PROMPT; i++)
		newModule->prompt[i] = prompt[i];
	prompt[i] = '0';

	//	Voy a asignarle espacio para el stack frame:
	// 	Tengo que moverme al final de la memoria guardada ya que
	//	el stack comienza en direcciones alta y va disminuyendo
	uint64_t *last_address = (uint64_t*) (reserve[numberOfModules+1] - 8);

	//	Y luego pongo la entrada a mi funcion
	*last_address = (uint64_t) program;
	//	rsp al inicio de mi programa
	newModule->stackFrame.rsp = last_address;
	//	El rbp lo inicializo en 0
	newModule->stackFrame.rbp = 0;
	
	numberOfModules++;

	return 1;
}

void startFirstProgram(){
	if (numberOfModules == 0)
		fprintf(1, "NO HAY PROGRAMA PARA EJECUTAR\n");
	else
	{
		setFrame(&(modules[0].stackFrame));
	}
}

static int switchWindow( unsigned int window ){
	//	Llamo a la syscall para cambiar de ventana graficamente
	int success = changeWindow(window);
	if(success)
	{
		//	Tengo que guardar el stack frame actual
		getFrame(&(modules[activeModule].stackFrame));
		//	Cambio el modulo activado
		activeModule = window;
		//	Seteo el stack frame
		setFrame(&(modules[activeModule].stackFrame));
	}
	return 0;
}

int getInput( char *inputBuffer, unsigned long int buffer_size )
{
	emptyBuffer();
	int ctrl = 0, size = 0, keyboard_size, i;
	char c, keyboard[buffer_size],
		delimiter = modules[activeModule].delimiter;

	puts(modules[activeModule].prompt);

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

				case '\b':
					if( size == 0 ){
						break;
					}
					size--;
					putchar(c);
					break;
				
				case '\n':
					if (delimiter != '\n') break;

				default:
					if( ctrl && c == '1' && activeModule != 0 ){
						switchWindow(0);
						break;
					}
					if( ctrl && c == '2' && activeModule != 1 ){
						switchWindow(1);
						break;
					}
					// Solo lo guardo si tengo espacio en el buffer
					// Si no hay espacio, hay que esperar a las teclas especiales
					if (c == delimiter || size < buffer_size - 1)
					{
						inputBuffer[size++] = c;
						putchar(c);
						if (c == delimiter) {
							return size;
						}
					}	
			}
		}
	}

	return size;
}
