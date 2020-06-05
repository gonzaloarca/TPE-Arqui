#include <registers.h>

static RegistersType reg;

void saveRegistersASM(RegistersType *reg);

void saveRegisters(){
	saveRegistersASM(&reg);
}

RegistersType *sys_getRegisters(){
	return &reg;
}