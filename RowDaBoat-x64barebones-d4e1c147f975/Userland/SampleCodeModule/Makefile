include ../Makefile.inc

MODULE=0000-sampleCodeModule.bin
SOURCES=$(wildcard [^_]*.c)
SOURCES_MODULES=$(wildcard modules/*.c)
SOURCES_ASM=$(wildcard asm/*.asm)

OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)

all: $(MODULE)

$(MODULE): $(SOURCES) $(SOURCES_ASM) $(SOURCES_MODULES)
	$(ASM) $(ASMFLAGS) $(SOURCES_ASM) -o $(OBJECTS_ASM)
	$(GCC) $(GCCFLAGS) -I./include -T sampleCodeModule.ld _loader.c $(SOURCES) $(SOURCES_MODULES) $(OBJECTS_ASM) -o ../$(MODULE)

clean:
	rm -rf *.o

.PHONY: all clean print
