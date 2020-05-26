#include <stdint.h>
#include <string.h>
#include <lib.h>
#include <moduleLoader.h>
#include <naiveConsole.h>
#include "video_driver.h"

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000;

static void * const sampleCodeModuleAddress = (void*)0x400000;
static void * const sampleDataModuleAddress = (void*)0x500000;

typedef int (*EntryPoint)();


void clearBSS(void * bssAddress, uint64_t bssSize)
{
	memset(bssAddress, 0, bssSize);
}

void * getStackBase()
{
	return (void*)(
		(uint64_t)&endOfKernel
		+ PageSize * 8				//The size of the stack itself, 32KiB
		- sizeof(uint64_t)			//Begin at the top of the stack
	);
}

void * initializeKernelBinary()
{
	char buffer[10];

	ncPrint("[x64BareBones]");
	ncNewline();

	ncPrint("CPU Vendor:");
	ncPrint(cpuVendor(buffer));
	ncNewline();

	ncPrint("[Loading modules]");
	ncNewline();
	void * moduleAddresses[] = {
		sampleCodeModuleAddress,
		sampleDataModuleAddress
	};

	loadModules(&endOfKernelBinary, moduleAddresses);
	ncPrint("[Done]");
	ncNewline();
	ncNewline();

	ncPrint("[Initializing kernel's binary]");
	ncNewline();

	clearBSS(&bss, &endOfKernel - &bss);

	ncPrint("  text: 0x");
	ncPrintHex((uint64_t)&text);
	ncNewline();
	ncPrint("  rodata: 0x");
	ncPrintHex((uint64_t)&rodata);
	ncNewline();
	ncPrint("  data: 0x");
	ncPrintHex((uint64_t)&data);
	ncNewline();
	ncPrint("  bss: 0x");
	ncPrintHex((uint64_t)&bss);
	ncNewline();

	ncPrint("[Done]");
	ncNewline();
	ncNewline();
	return getStackBase();
}

int main()
{	

	printNullString( "el beto", 0xFFFFFF );
	newline();
	printNullString( "el beto", 0xFFFFFF );
	newline();
	printInt( 41287, 0xFFFFFF );
	newline();
	printInt( -69420, 0xFFFFFF );
	newline();
	printNullString( "A", 0xFFFFFF );
	newline();
	printNullString( "B", 0xFFFFFF );
	newline();
	printNullString( "C", 0xFFFFFF );
	newline();
	printNullString( "D", 0xFFFFFF );
	newline();
	printNullString( "E", 0xFFFFFF );
	newline();
	printNullString( "F", 0xFFFFFF );
	newline();
	printNullString( "G", 0xFFFFFF );
	newline();
	printNullString( "H", 0xFFFFFF );
	newline();
	printNullString( "I", 0xFFFFFF );
	newline();
	printNullString( "J", 0xFFFFFF );
	newline();
	printNullString( "K", 0xFFFFFF );
	newline();
	printNullString( "L", 0xFFFFFF );
	newline();
	printNullString( "A", 0xFFFFFF );
	newline();
	printNullString( "B", 0xFFFFFF );
	newline();
	printNullString( "C", 0xFFFFFF );
	newline();
	printNullString( "D", 0xFFFFFF );
	newline();
	printNullString( "E", 0xFFFFFF );
	newline();
	printNullString( "F", 0xFFFFFF );
	newline();
	printNullString( "G", 0xFFFFFF );
	newline();
	printNullString( "H", 0xFFFFFF );
	newline();
	printNullString( "I", 0xFFFFFF );
	newline();
	printNullString( "J", 0xFFFFFF );
	newline();
	printNullString( "K", 0xFFFFFF );
	newline();
	printNullString( "L", 0xFFFFFF );
	newline();
	printNullString( "A", 0xFFFFFF );
	newline();
	printNullString( "B", 0xFFFFFF );
	newline();
	printNullString( "C", 0xFFFFFF );
	newline();
	printNullString( "D", 0xFFFFFF );
	newline();
	printNullString( "E", 0xFFFFFF );
	newline();
	printNullString( "F", 0xFFFFFF );
	newline();
	printNullString( "G", 0xFFFFFF );
	newline();
	printNullString( "H", 0xFFFFFF );
	newline();
	printNullString( "I", 0xFFFFFF );
	newline();
	printNullString( "J", 0xFFFFFF );
	newline();
	printNullString( "K", 0xFFFFFF );
	newline();
	printNullString( "L", 0xFFFFFF );
	newline();
	printNullString( "A", 0xFFFFFF );
	newline();
	printNullString( "B", 0xFFFFFF );
	newline();
	printNullString( "C", 0xFFFFFF );
	newline();
	printNullString( "D", 0xFFFFFF );
	newline();
	printNullString( "ED", 0xFFFFFF );
	newline();
	printNullString( "FEEEE", 0xFFFFFF );
	newline();
	printNullString( "G", 0xFFFFFF );
	newline();
	printNullString( "H", 0xFFFFFF );
	newline();
	printNullString( "I", 0xFFFFFF );
	newline();
	printNullString( "J", 0xFFFFFF );
	newline();
	printNullString( "K", 0xFFFFFF );
	newline();
	printNullString( "L", 0xFFFFFF );
	newline();

	return 0;
}
