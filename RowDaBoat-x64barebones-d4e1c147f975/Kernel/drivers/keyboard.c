#include <keyboard.h>

#define LEFT_SHIFT 42
#define RIGHT_SHIFT 54
#define LSHIFT_RELEASED (LEFT_SHIFT+0x80)
#define RSHIFT_RELEASED (RIGHT_SHIFT+0x80) 

//	Tengo que guardarme si el shift se encuentra presionado
static int lshift = 0;
static int rshift = 0;

const char asccode[58][2] = { {0,0}, {0,0}, {'1', '!'}, {'2', '@'}, {'3', '#'},{'4', '$'},
						{'5','%'},{'6','^'},{'7','&'},{'8','*'},{'9','('},{'0',')'},
						{'-','_'},{'=','+'},{'\b', '\b'},{'\t','\t'},{'q','Q'}, 
						{'w','W'}, {'e','E'},{'r','R'},{'t','T'},{'y','Y'},{'u','U'},
						{'i','I'},{'o','O'},{'p','P'},{'[','{'},{']','}'},
					  	{'\n','\n'},{0,0},{'a','A'},{'s','S'},{'d','D'},{'f','F'},
					  	{'g','G'},{'h','H'},{'j','J'},{'k','K'},{'l','L'}, 
					  	{';',':'},{'\'', '\"'},{'`','~'},{0,0},{'\\','|'},
					  	{'z','Z'},{'x','X'},{'c','C'},{'v','V'},{'b','B'},
					  	{'n','N'},{'m','M'}, {',', '<'},{'.','>'},{'/','?'},
					  	{0,0},{0,0},{0,0},{' ',' '}};

char checkKey(int code);

void keyboard_handler()
{
	//	Si no habia nada para leer, no hago nada
	if (canReadKey() == 0) return;

	int code = getScanCode();
	//	Verifico que key es el codigo
	char c = checkKey(code);

	if (c != 0) printChar(c, 0xFFFFFF);
}

char checkKey(int code)
{
	//	Seteo el estado de los shift
	switch(code)
	{
		case LEFT_SHIFT: 
			lshift = 1; return 0;
		case RIGHT_SHIFT: 
			rshift = 1; return 0;
		case LSHIFT_RELEASED:
			lshift = 0; return 0;
		case RSHIFT_RELEASED:
			rshift = 0; return 0;
	}

	if (code > 0x80) return 0;	//	No me interesan los exit codes

	//	Convierto el codigo a char fijandome el estado de los shifts
	if (lshift + rshift >= 1)
		return scanCodetoChar(code, 1);

	return scanCodetoChar(code, 0);
}

char scanCodetoChar (unsigned int scan_code, unsigned int shift)
{
	if ((shift != 0 && shift != 1) || scan_code >= 58) 
		return 0;

	return asccode[scan_code][shift];
}

/*
void keyboardPolling(){
	int key, shift = 0;

	//	Espero a poder leer una tecla
	while(canReadKey() == 0);

	//	Me guardo la tecla que presiono
	key = getScanCode();

	//	Veo si fue un shift
	if (key == 42 || key == 54) {

		int aux;
		shift = 1;

		//	Tengo que leer la siguiente tecla
		while(canReadKey() == 0);
		key = getScanCode();

		//	Tengo que esperar a que la suelte o suelte el shift
		do {
			while(canReadKey() == 0);
			aux = getScanCode(); 
			//	Si solto el shift, no hago nada
			if (aux == 42 + 0x80 || aux == 54 + 0x80)
				return;
		} while(aux != key + 0x80);

	} else {	
		//	Espero a que suelte la tecla
		do {
			while(canReadKey() == 0);
		} while(getScanCode() != key + 0x80);
		//	Solo suelta la tecla si el teclado devuelve ScanCode + 80h
	}

	
	//	Imprimir el scancode directo:
	//char string[20];
	//print(numtostr(key,string));
	

	//	Imprimo la tecla
	putc(scanCodetoChar(key, shift));

}
*/
