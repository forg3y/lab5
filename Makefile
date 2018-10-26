# makefile
# use to compile and test the assembler ASM for the STACK computer and
# its various components

DEBUG_FLAG= -DDEBUG -g -Wall
CFLAGS=-DDEBUG -g -Wall

.SUFFIXES: .cpp .o

.cpp.o:
	g++ $(CFLAGS) -c $<


DCHECKER: Depend.o DependencyChecker.o Instruction.o OpcodeTable.o RegisterTable.o ASMParser.o
	g++ -o DCHECKER Depend.o DependencyChecker.o OpcodeTable.o ASMParser.o RegisterTable.o Instruction.o

DependencyChecker.o: DependencyChecker.h OpcodeTable.h RegisterTable.h Instruction.h 

ASMParser.o: ASMParser.h OpcodeTable.h RegisterTable.h Instruction.h 

Instruction.o: OpcodeTable.h RegisterTable.h Instruction.h 

OpcodeTable.o: OpcodeTable.h 

RegisterTable.o: RegisterTable.h  

clean:
	/bin/rm -f DCHECKER *.o core


