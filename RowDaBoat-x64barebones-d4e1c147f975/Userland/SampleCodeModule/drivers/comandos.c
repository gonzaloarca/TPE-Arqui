#include <stdint.h>
#include <std_io.h>
#include <syscalls.h>
#include <comandos.h>


#define INFOREG_MSG "inforeg\tMuestra el estado de los registros del ultimo\n guardado."
#define PRINTTIME_MSG "inforeg\tMuestra la hora actual."
#define PRINTMEM_MSG "printmem\tMuestra los 32 bytes siguientes a partir de la\n direccion de memoria(en hexadecimal) recibida por parametro."
#define PRINTCPUINFO_MSG "printCPUInfo\tMuestra informacion de interes sobre el cpu."

void help(){
	printf("Informacion sobre comandos disponibles:\n%s\n%s\n%s\n%s\n", INFOREG_MSG, PRINTTIME_MSG, PRINTMEM_MSG, PRINTCPUINFO_MSG);
}

void printTime(){       //se podria modularizar en una funcion para obtener el tiempo correcto, que abarcaria hasta el while, y otra para generar el string formateado del tiempo, despues juega printString
    TimeFormat time;
	getTime( &time );
	
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
	getCPUInfo(&info);

	printf("Vendor: %s\n", info.vendor);
	printf("FamilyID: %d\n", info.family);
	printf("Model: %d\n", info.model);
	printf("Brand: %s\n", info.brand);
}

void getMemoryASM(memType* answer, uint64_t address);

static int pow(int potencia, int base){
	int ans = 1;
	for(int i = 0 ; i < potencia ; i ++)
		ans = ans * base;

	return ans;
}

static int getMemory(memType* answer, char* address){
	// Se encarga de validar que la direccion este escrita en hexa, retorna 1 en exito 0 sino
	uint64_t decimalAddress = 0;
	int length = strlen(address);
	int flag = 1;		// asumo en un principio que esta correctamente escrito

	for(int i = length ; i > 0 && flag ; i--){
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
	if(flag)
		getMemoryASM(answer, decimalAddress);

	return flag;
}

void printmem(char* address){
	memType memory;

	uint64_t decimalAddress = 0;
	int length = strlen(address);
	int flag = 1;		// asumo en un principio que esta correctamente escrito

	for(int i = length ; i > 0 && flag; i--){
		switch(address[i-1]){
			case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
				decimalAddress += (address[i-1] - '0') * pow(length-i, 16);
				break;
			case 'a':
				decimalAddress += 10 * pow(length-i, 16);
				break;
			case 'b':
				decimalAddress += 11 * pow(length-i, 16);
				break;
			case 'c':
				decimalAddress += 12 * pow(length-i, 16);
				break;
			case 'd':
				decimalAddress += 13 * pow(length-i, 16);
				break;
			case 'e':
				decimalAddress += 14 * pow(length-i, 16);
				break;
			case 'f':
				decimalAddress += 15 * pow(length-i, 16);
				break;

			default:
			// Encontre un caracter que no cumple con la escritura hexadecimal de un numero positivo
				flag = 0;
		}
	}
	if(flag){
		getMemory(&memory, address);

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
		fprintf(2, "Direccion invalida.\n");
}