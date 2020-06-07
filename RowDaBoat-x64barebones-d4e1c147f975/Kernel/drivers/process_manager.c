#include <process_manager.h>
#include <window_manager.h>

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

	//	Pongo la entrada a mi programa
	newModule->backup.rip = (uint64_t) modules[numberOfModules].program;
	//	rsp al inicio del stack frame
	newModule->backup.rsp = (uint64_t) last_address;

	//	Le asigno el numero de process ID segun la cantidad de procesos corriendo hasta el momento
	newModule->pid = numberOfModules;
	//	Se le asigna al mapa de pid-window la ventana que ocupara el programa (tomamos la funcion identidad por convencion)
	windowMap[newModule->pid] = newModule->pid;

	numberOfModules++;

	return 0;
}

int switchProcess(){
	if( numberOfModules <= 1 ){
		return 1;
	}
	int nextPid = (activeModule + 1) % numberOfModules;
	//	Llamo a la syscall para cambiar de ventana graficamente
	int success = sys_changeWindow( windowMap[nextPid] );
	if(success)
	{
		//	Tengo que guardar el stack frame actual
		getBackupINT(&(modules[activeModule].backup));
		//	Cambio el modulo activado
		activeModule = nextPid;
		//	Seteo el stack frame
		setBackupINT(&(modules[activeModule].backup));
	}
	return 0;
}

void sys_runFirstProcess(){
	if (numberOfModules == 0)
		return;

	else
	{
		//	Empieza el primer programa
		startRunning(modules[activeModule].backup.rip, modules[activeModule].backup.rsp);
	}
}

void recoverModule(){

	// Busco la direccion donde arranca el stack del modulo actual
	uint64_t *last_address = (uint64_t*) (reserve[activeModule+1] - 8);

	// Obtengo la direccion al modulo actual
	Module *newModule = &(modules[activeModule]);

	//	Pongo la entrada a mi programa
	newModule->backup.rip = (uint64_t) modules[activeModule].program;
	//	rsp al inicio del stack frame
	newModule->backup.rsp = (uint64_t) last_address;

	//	El programa de antes de la interrupcion se reinicia
	startRunningEXC(newModule->backup.rip, newModule->backup.rsp);

}
