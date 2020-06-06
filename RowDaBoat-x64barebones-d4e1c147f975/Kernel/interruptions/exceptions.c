#define ZERO_EXCEPTION_ID 0
#define OPCODE_ECEPTION_ID 6

static void zero_division(){
	// se imprimen registros
	// recoverModule();
	return;
}

static void opcode(){
	// se imprimen registros
	return;
}


// excpNumber indica la excepcion a la que se llamo
// Este dispatcher se encargara de ejecutar la rutina de tratamiento correspondiente a la excepcion
void exceptionDispatcher(int excpNumber) {
	switch(excpNumber)
	{
		case ZERO_EXCEPTION_ID:
			zero_division();
			break;
		case OPCODE_ECEPTION_ID:
			opcode();
			break;
	}
}