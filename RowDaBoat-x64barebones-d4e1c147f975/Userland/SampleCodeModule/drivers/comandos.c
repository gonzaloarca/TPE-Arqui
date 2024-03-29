#include <comandos.h>


// Constantes utilizadas por el comando help
#define CLEAR_MSG		"\tLimpia la pantalla."
#define CPUINFO_MSG 	"\tMuestra informacion de interes sobre el cpu."
#define CPUTEMP_MSG		"\tMuestra la temperatura del cpu."
#define INFOREG_MSG 	"\tMuestra el estado de los registros del ultimo\n guardado(los guardados se realizan presionando F1)."
#define PRINTMEM_MSG 	"\tMuestra los 32 bytes siguientes a partir de la\n direccion de memoria(en hexadecimal) recibida por parametro."
#define PRINTTIME_MSG 	"\tMuestra la hora actual del dispositivo."
#define EXCP_0_MSG		"\tComando para verificar la rutina de\n excepcion de division por cero."
#define EXCP_6_MSG		"\tComando para verificar la rutina de\n excepcion de operacion invalida(Undefined Instruction)."

#define TECLA_F1		"\tEjecuta el guardado de los registros, para que sean\n impresos con inforeg."
#define TECLA_F2		"\tEjecuta el borrado total de la linea actual."
#define CAMBIO			"\tEjecuta el cambio de proceso."

void help(){
	puts("---------Informacion sobre comandos disponibles----------------\n");
	changeWindowColor(0xffd300);
	printf("%s\t", "clear");
	changeWindowColor(0xffffff);
	printf("%s\n", CLEAR_MSG);

	changeWindowColor(0xffd300);
	printf("%s  ", "cpuinfo");
	changeWindowColor(0xffffff);
	printf("%s\n", CPUINFO_MSG);

	changeWindowColor(0xffd300);
	printf("%s  ", "cputemp");
	changeWindowColor(0xffffff);
	printf("%s\n", CPUTEMP_MSG);

	changeWindowColor(0xffd300);
	printf("%s  ", "inforeg");
	changeWindowColor(0xffffff);
	printf("%s\n", INFOREG_MSG);

	changeWindowColor(0xffd300);
	printf("%s ", "printmem");
	changeWindowColor(0xffffff);
	printf("%s\n", PRINTMEM_MSG);

	changeWindowColor(0xffd300);
	printf("%s", "printtime");
	changeWindowColor(0xffffff);
	printf("%s\n", PRINTTIME_MSG);

	changeWindowColor(0xffd300);
	printf("%s", "executeZeroException");
	changeWindowColor(0xffffff);
	printf("%s\n", EXCP_0_MSG);

	changeWindowColor(0xffd300);
	printf("%s", "executeUIException  ");
	changeWindowColor(0xffffff);
	printf("%s\n", EXCP_6_MSG);

	putchar('\n');

	printf("---------Informacion sobre teclas especiales disponibles-------\n");
	changeWindowColor(0x4ad5f2);
	printf("%s", "F1\t");
	changeWindowColor(0xffffff);
	printf("%s\n", TECLA_F1);

	changeWindowColor(0x4ad5f2);
	printf("%s", "F2\t");
	changeWindowColor(0xffffff);
	printf("%s\n", TECLA_F2);

	changeWindowColor(0x4ad5f2);
	printf("%s", "F3\t");
	changeWindowColor(0xffffff);
	printf("%s\n", CAMBIO);

	putchar('\n');
}

void printTime(){       //se podria modularizar en una funcion para obtener el tiempo correcto, que abarcaria hasta el while, y otra para generar el string formateado del tiempo, despues juega printString
    TimeFormat time;
	getTime( &time );
	
	//	Hora local Argentina
	if (time.hours >= 3) time.hours -= 3;
	else time.hours = 24 + time.hours - 3;

	char timeString[9];

    timeString[2] = ':';
	timeString[5] = ':';
	timeString[8] = 0;

    int data[3] = { time.hours, time.mins, time.secs };
    int j = 0; 
	for( int i = 0; i < 3; i++ ){
		if( data[i] < 10 ){
			timeString[j++] = '0';
		} else {
			timeString[j++] = (data[i] / 10) + '0';
		}
		timeString[j++] = (data[i] % 10) + '0';
		j++;
	}

	puts( timeString );
}

void printCPUInfo()
{
	CpuInfo info;

	cpuVendor(info.vendor);
	cpuBrand(info.brand);
	cpuModel(&(info.family));

	printf("Vendor: %s\n", info.vendor);
	printf("FamilyID: %d\n", info.family);
	printf("Model: %d\n", info.model);
	printf("Brand: %s\n", info.brand);
}

static int pow(int potencia, int base){
	int ans = 1;
	for(int i = 0 ; i < potencia ; i ++)
		ans = ans * base;

	return ans;
}

void printmem(char* address){
	memType memory;

	uint64_t decimalAddress = 0;
	int length = strlen(address);
	int flag = 1;		// asumo en un principio que esta correctamente escrito

	// reviso el address este escrito correctamente
	for(int i = length ; i > 0 && flag; i--){
		switch(address[i-1]){
			case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
				decimalAddress += (address[i-1] - '0') * pow(length-i, 16);
				break;
			case 'a': case 'A':
				decimalAddress += 10 * pow(length-i, 16);
				break;
			case 'b': case 'B':
				decimalAddress += 11 * pow(length-i, 16);
				break;
			case 'c': case 'C':
				decimalAddress += 12 * pow(length-i, 16);
				break;
			case 'd': case 'D':
				decimalAddress += 13 * pow(length-i, 16);
				break;
			case 'e': case 'E':
				decimalAddress += 14 * pow(length-i, 16);
				break;
			case 'f': case 'F':
				decimalAddress += 15 * pow(length-i, 16);
				break;

			default:
			// Encontre un caracter que no cumple con la escritura hexadecimal de un numero positivo
				flag = 0;
		}
	}

	if(flag){
		getMemory(&memory, decimalAddress);

		changeWindowColor(0xa4de02);
		printf("%p", decimalAddress);
		changeWindowColor(0xffffff);
		printf(": ");
		for(int i = 0 ; i < 8 ; i++){
			printf("%x ", 0xFF & memory.mem1);
			memory.mem1 = memory.mem1 >> 8;
		}
		putchar('\n');
		changeWindowColor(0xa4de02);
		printf("%p", decimalAddress+8);
		changeWindowColor(0xffffff);
		printf(": ");
		for(int i = 0 ; i < 8 ; i++){
			printf("%x ", 0xFF & memory.mem2);
			memory.mem2 = memory.mem2 >> 8;
		}
		putchar('\n');
		changeWindowColor(0xa4de02);
		printf("%p", decimalAddress+16);
		changeWindowColor(0xffffff);
		printf(": ");
		for(int i = 0 ; i < 8 ; i++){
			printf("%x ", 0xFF & memory.mem3);
			memory.mem3 = memory.mem3 >> 8;
		}
		putchar('\n');
		changeWindowColor(0xa4de02);
		printf("%p", decimalAddress+24);
		changeWindowColor(0xffffff);
		printf(": ");
		for(int i = 0 ; i < 8 ; i++){
			printf("%x ", 0xFF & memory.mem4);
			memory.mem4 = memory.mem4 >> 8;
		}
		putchar('\n');
	}
	else
		fprintf(2, "-printmem:Direccion invalida.\n");
}

void printInforeg(){
	RegistersType* reg = getRegisters();
	printf("RAX: %ld\tRBX: %ld\tRCX: %ld\nRDX: %ld\tRBP: %ld\tRDI: %ld\nRSI: %ld\tRSP: %ld\tR8:  %ld\nR9:  %ld\tR10: %ld\tR11: %ld\nR12: %ld\tR13: %ld\tR14: %ld\nR15: %ld\tRIP: %ld\n",
		reg->rax, reg->rbx, reg->rcx,
		 reg->rdx, reg->rbp, reg->rdi,
		  reg->rsi, reg->rsp, reg->r8,
		   reg->r9, reg->r10, reg->r11,
		    reg->r12, reg->r13, reg->r14, reg->r15, reg->rip);
}

void printCPUTemp(){
	int temp = getCPUTemp();
	changeWindowColor(0xff4500);
	printf("Temperatura actual del CPU: ");
	changeWindowColor(0xffffff);
	printf("%d C\n", temp);
}
