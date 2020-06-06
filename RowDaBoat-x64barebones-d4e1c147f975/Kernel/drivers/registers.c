#include <registers.h>
#include <window_manager.h>

static RegistersType reg;

void saveRegistersASM(RegistersType *reg);

void saveRegisters(){
	saveRegistersASM(&reg);
}

RegistersType *sys_getRegisters(){
	return &reg;
}