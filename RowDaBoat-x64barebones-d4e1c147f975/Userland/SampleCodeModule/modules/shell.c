#include <console.h>
#include <std_io.h>
#include <comandos.h>
#define INPUT_BUFFER_SIZE 100

char inputBuffer[INPUT_BUFFER_SIZE];
int indexBuffer;
static void parse();
static PromptData prData = { "$> ", '\n' };


void runShell(){
    
    while(1){
        indexBuffer = getInput( inputBuffer, INPUT_BUFFER_SIZE, prData.symbol, prData.delimiter );
        parse();
    }
}

static void parse(){
    inputBuffer[indexBuffer] = 0;         // Reemplazo con 0 el final para utilizar strcmp
    
    if(strcmp( inputBuffer, "help\n") == 0)
        help();
    else if(strcmp( inputBuffer, "inforeg\n" ) == 0){
	   printInforeg();
    }
    else if(strcmp( inputBuffer, "clear\n" ) == 0){
        clrScreen();
    }
    else if(strcmp( inputBuffer, "printtime\n" ) == 0){
        printTime();
        putchar('\n');
    }
    else if(strcmp( "printmem", inputBuffer ) == 0){           // solo valido que lo q este al principio del input valide con el comando,
                                                                // el resto va a ser el argumento
        if( inputBuffer[8] == ' ' && inputBuffer[9] != '\n'){       // se ingreso un parametro
            char address[INPUT_BUFFER_SIZE- 10] = {0};              // maximo tamanio posible del argumento("printmem " son 9 caracteres y tampoco cuento el \n)
            int i = 0;
            for(; inputBuffer[9 + i] != '\n' && i < indexBuffer -1; i++)   // en inputBuffer[indexBuffer] esta el \n
                address[i] = inputBuffer[9 + i];

            address[i] = 0;
            printmem(address);
        }else
            fprintf(2, "-printmem: Falta ingresar la direccion como parametro.\n");
    }
    else if(strcmp( inputBuffer, "cpuinfo\n") == 0){
        printCPUInfo();
    }
    else if(strcmp( inputBuffer, "cputemp\n") == 0)
        // printCPUtemp();
        return;
    else
        fprintf(2, "Comando no reconocido, ejecuta help para recibir informacion.\n");
}