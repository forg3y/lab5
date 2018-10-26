#ifndef __DEPENDENCYCHECKER_H__
#define __DEPENDENCYCHECKER_H__

#include <iostream>
#include <map>
#include <list>

using namespace std;

#include "Instruction.h"
#include "OpcodeTable.h"

/*
 * The DependencyChecker class finds all of the data dependencies (RAW, WAR, WAW)
 * involved in a sequence of instructions.
 */


// The different types of possible data dependencies. 
enum DependenceType{
  RAW,                 // Read After Write
  WAR,                 // Write After Read
  WAW,                 // Write After Write
  D_UNDEFINED
};

// The different ways of accessing data
enum AccessType{
  READ,
  WRITE,
  A_UNDEFINED
};

/* RegisterInfo is used to process dependencies.  It stores information about the
 * last instruction to access a given register and whether that access was a 
 * read or write (needed to determine the type of dependence.
 */
struct RegisterInfo{
  int lastInstructionToAccess;
  AccessType accessType;
  
  // Constructor sets last instruction to -1 and access type to undefined
  RegisterInfo(){
    lastInstructionToAccess = -1;
    accessType = A_UNDEFINED;
  };
};

/* The Dependence struct keeps track of the involved instructions, the register
 * that the dependence involves, and what type of dependence.
 */
struct Dependence {
  DependenceType dependenceType;        
  unsigned int registerNumber;
  int previousInstructionNumber;         // first instruction to occur
  int currentInstructionNumber;          // second instruction to occur  
};


/* This class keeps track of a sequence of instructions and determines data
 * dependencies that occur between the instructions due to register usage.  Instructions
 * are numbered and those numbers are used to keep track of which instructions
 * are used in a dependence.
 */ 
class DependencyChecker {
 public:

  /* Creates RegisterInfo entries for each of the 32 registers and creates lists for
   * dependencies and instructions.
   */
  DependencyChecker(int numRegisters = 32);
  
  /* Adds an instruction to the list of instructions and checks to see if that 
   * instruction results in any new data dependencies.  If new data dependencies
   * are created with the addition of this instruction, appropriate entries
   * are added to the list of dependences.
   */
  void addInstruction(Instruction i);

  /* Prints out the sequence of instructions followed by the sequence of data
   * dependencies.
   */ 
  void printDependences();

 private:
  /* Determines if a read data dependence occurs when reg is read by the current
   * instruction.  If so, adds an entry to the list of dependences. Also updates
   * the appropriate RegisterInfo entry regardless of dependence detection.
   */
  void checkForReadDependence(unsigned int reg);

  /* Determines if a write data dependence occurs when reg is written by the current
   * instruction.  If so, adds an entry to the list of dependences. Also updates 
   * the appropriate RegisterInfo entry regardless of dependence detection.
   */
  void checkForWriteDependence(unsigned int reg);

  map<unsigned int, RegisterInfo> myCurrentState;
  list<Dependence> myDependences;
  list<Instruction> myInstructions;
  OpcodeTable myOpcodeTable;
};


#endif
