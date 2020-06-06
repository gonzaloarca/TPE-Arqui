#include <process_manager.h>

#define WINDOWS 2
#define MB	0x100000

//	Lista de modulos/programas
static Module modules[WINDOWS];
static int windowMap[WINDOWS] = {0}; //mapa que recibe el pid de un programa y devuelve el indice de la ventana que le corresponde
static unsigned int activeModule = 0;	
static unsigned int numberOfModules = 0;

//	Reservo espacio para los stack frames
static char reserve[WINDOWS][MB];

int sys_initModule(void (*program)())
{
	//	Si ya no tengo mas ventanas, no puedo agregar nada
	if (numberOfModules >= WINDOWS)
		return 1;

	//	Busco la direccion del nuevo modulo en la lista
	Module *newModule = &(modules[numberOfModules]);

	//	Le asigno al modulo el puntero al programa recibido
	newModule->program = program;

	//	Voy a asignarle espacio para el stack frame:
	// 	Tengo que moverme al final de la memoria guardada ya que
	//	el stack comienza en direcciones alta y va disminuyendo
	uint64_t *last_address = (uint64_t*) (reserve[numberOfModules+1] - 8);

	//	Y luego pongo la entrada a mi programa
	*last_address = (uint64_t) modules[numberOfModules].program;
	//	rsp al inicio de mi programa
	newModule->stackFrame.rsp = last_address;
	//	El rbp lo inicializo en 0
	newModule->stackFrame.rbp = 0;
	//	Le asigno el numero de process ID segun la cantidad de procesos corriendo hasta el momento
	newModule->pid = numberOfModules;
	//	Se le asigna al mapa de pid-window la ventana que ocupara el programa (tomamos la funcion identidad por convencion)
	windowMap[newModule->pid] = newModule->pid;

	numberOfModules++;

	return 0;
}

int sys_switchProcess(){
	if( numberOfModules <= 1 ){
		return 1;
	}
	int nextPid = (activeModule + 1) % numberOfModules;
	//	Llamo a la syscall para cambiar de ventana graficamente
	int success = sys_changeWindow( windowMap[nextPid] );
	if(success)
	{
		//	Tengo que guardar el stack frame actual
		getBackup(&(modules[activeModule].stackFrame), &(modules[activeModule].backup));
		//	Cambio el modulo activado
		activeModule = nextPid;
		//	Seteo el stack frame
		setBackup(&(modules[activeModule].stackFrame), &(modules[activeModule].backup));
	}
	return 0;
}

void sys_runFirstProcess(){
	if (numberOfModules == 0)
		return;

	else
	{
		//	Empieza el primer programa
		setBackup(&(modules[0].stackFrame), &(modules[0].backup));
	}
}

void recoverModule(){

	// Busco la direccion donde arranca el stack del modulo actual
	uint64_t *last_address = (uint64_t*) (reserve[activeModule+1] - 8);

	// Obtengo la direccion al modulo actual
	Module *newModule = &(modules[activeModule]);

	//	Y luego pongo la entrada al programa del modulo actual al inicio de su stack
	*last_address = (uint64_t) modules[activeModule].program;
	//	Ubico rsp al inicio del programa
	newModule->stackFrame.rsp = last_address;
	//	Reinicio rbp
	newModule->stackFrame.rbp = 0;

	//	Seteo el stack frame para que se realize el cambio
	setBackup(&(modules[activeModule].stackFrame), &(modules[activeModule].backup));
}