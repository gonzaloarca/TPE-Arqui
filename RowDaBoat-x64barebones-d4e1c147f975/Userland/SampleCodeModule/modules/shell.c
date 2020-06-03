#include <graphics_engine.h>
#include <std_io.h>
#include <comandos.h>
#define INPUT_BUFFER_SIZE 100

typedef struct registersType {
    uint64_t rbx;
    uint64_t rcx;
    uint64_t rdx;
    uint64_t rsi;
    uint64_t rbp;
    uint64_t rsp;
    uint64_t r8;
    uint64_t r9;
    uint64_t r10;
    uint64_t r11;
    uint64_t r12;
    uint64_t r13;
    uint64_t r14;
    uint64_t r15;
    uint64_t rip;
} registersType;

char inputBuffer[INPUT_BUFFER_SIZE];
int indexBuffer;
registersType reg;
uint64_t rax, rdi;

static void parse();

uint64_t getRAX();
uint64_t getRDI();

registersType* getRegisters();

void runShell(){
    while(1){
        rax = getRAX();
        rdi = getRDI();
		getRegisters(&reg);
        indexBuffer = getInput( inputBuffer, INPUT_BUFFER_SIZE );
        parse();
    }
}

static void parse(){
    inputBuffer[indexBuffer] = 0;         // Reemplazo el \n con 0 para utilizar strcmp
    
    if(strcmp( inputBuffer, "inforeg\n" ) == 0){
	   printf("RAX: %ld\tRBX: %ld\tRCX: %ld\nRDX: %ld\tRSI: %ld\tRDI: %ld\nRBP: %ld\tRSP: %ld\tR8:  %ld\nR9:  %ld\tR10: %ld\tR11: %ld\nR12: %ld\tR13: %ld\tR14: %ld\nR15: %ld\tRIP: %ld\n",
		rax, reg.rbx, reg.rcx,
		 reg.rdx, reg.rsi, rdi,
		  reg.rbp, reg.rsp, reg.r8,
		   reg.r9, reg.r10, reg.r11,
		    reg.r12, reg.r13, reg.r14, reg.r15, reg.rip);
    }
    else if(strcmp( inputBuffer, "printTime\n" ) == 0){
        printTime();
        putchar('\n');
    }
    else if(strcmp( inputBuffer, "printmem\n") == 0){
        // printmem();
        putchar('\n');
    }
}
