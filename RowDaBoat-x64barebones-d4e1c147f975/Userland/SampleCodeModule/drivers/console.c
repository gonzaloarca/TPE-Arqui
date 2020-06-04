#include <console.h>
#include <std_io.h>
#include <syscalls.h>

#define WINDOWS 2

static StackFrame mainFrame;
static Module modules[WINDOWS];
static unsigned int activeModule = 0;
static unsigned int numberOfModules = 0;

//	Funcion de assembler que guarda el stack frame
void markFrame(StackFrame *mainFrame);
//	Funcion de assembler que setea el frame de nuevo
//	y comienza a ejecutar el programa
void setFrame(StackFrame *mainFrame, void (*program)());

void setMainFrame(){
	markFrame(&mainFrame);
	//printf("RBP = %x\n", mainFrame.rbp);
	//printf("RSP = %x\n", mainFrame.rsp);
}

int initModule(void (*program)(), char prompt[MAX_PROMPT], char delimiter)
{
	//	Si ya no tengo mas ventanas, no puedo agregar nada
	if (numberOfModules >= WINDOWS)
		return 0;

	//	Busco la direccion del nuevo modulo en la lista
	Module *newModule = &(modules[numberOfModules]);

	//	Copio los parametros al modulo
	newModule->program = program;
	newModule->delimiter = delimiter;
	for (int i = 0; prompt[i] != 0 && i < MAX_PROMPT; i++)
		newModule->prompt[i] = prompt[i];

	//	Le asigno una ventana
	newModule->windowID = numberOfModules;
	numberOfModules++;

	return 1;
}

void startFirstProgram(){
	if (numberOfModules == 0)
		fprintf(1, "NO HAY PROGRAMA PARA EJECUTAR\n");
	else
	{
		setFrame(&mainFrame, modules[activeModule].program);
	}
}

static int switchWindow( unsigned int window ){
	//	Llamo a la syscall para cambiar de ventana graficamente
	int success = changeWindow(window);
	if(success)
	{
		activeModule = window;
		setFrame(&mainFrame, modules[activeModule].program);
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
