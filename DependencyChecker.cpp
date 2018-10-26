#include "DependencyChecker.h"

DependencyChecker::DependencyChecker(int numRegisters)
/* Creates RegisterInfo entries for each of the 32 registers and creates lists for
 * dependencies and instructions.
 */
{
  RegisterInfo r;

  // Create entries for all registers
  for(int i = 0; i < numRegisters; i++){
    myCurrentState.insert(make_pair(i, r));
  }
}

void DependencyChecker::addInstruction(Instruction i)
/* Adds an instruction to the list of instructions and checks to see if that 
 * instruction results in any new data dependencies.  If new data dependencies
 * are created with the addition of this instruction, appropriate entries
 * are added to the list of dependences.
 */
{
  InstType iType = i.getInstType();

  switch(iType){
  case RTYPE:
    for each (register r in Instruction i) {

    break;
  case ITYPE:
    // Your code here
    break;
  case JTYPE:
    // Your code here
    break;
  default:
    // do nothing
    break;
  }

  myInstructions.push_back(i);

}

void DependencyChecker::checkForReadDependence(unsigned int reg)
  /* Determines if a read data dependence occurs when reg is read by the current
   * instruction.  If so, adds an entry to the list of dependences. Also updates
   * the appropriate RegisterInfo entry regardless of dependence detection.
   */
{
  // Case: RAW
  // Check the RegisterInfo() stuff
  // if -1, first time accessing reg and need to update lastinstaccessed and
  //  AccessType, don't update dependency list 
  // if previous AccessType is read, AND current is a read, update
  //  lastinstaccessed, do nothing else
  // if previous AccessType is read, AND current is a write, create new
  //  Dependence, update lastinstaccessed and AccessType, update myDependency  

}

void DependencyChecker::checkForWriteDependence(unsigned int reg)
  /* Determines if a write data dependence occurs when reg is written by the current
   * instruction.  If so, adds an entry to the list of dependences. Also updates 
   * the appropriate RegisterInfo entry regardless of dependence detection.
   */
{
  
  // Cases: WAW and WAR
  // Check the RegisterInfo stuff
  // if -1, first access, update lastinstaccessed and AccessType
  // if previous AccessType is write, AND current is write, create new
  //  Dependence, update lastinstaccessed (can leave AccessType alone),
  //  and update myDependency list
  // if previous AccessType is write, AND current is read, create new 
  //  Dependence, update lastinst and AccessType, update myDependency list

}


void DependencyChecker::printDependences()
  /* Prints out the sequence of instructions followed by the sequence of data
   * dependencies.
   */ 
{
  // First, print all instructions
  list<Instruction>::iterator liter;
  int i = 0;
  cout << "INSTRUCTIONS:" << endl;
  for(liter = myInstructions.begin(); liter != myInstructions.end(); liter++){
    cout << i << ": " << (*liter).getAssembly() << endl;
    i++;
  }

  // Second, print all dependences
  list<Dependence>::iterator diter;
  cout << "DEPENDENCES: \nType Register (FirstInstr#, SecondInstr#) " << endl;
  for(diter = myDependences.begin(); diter != myDependences.end(); diter++){
    switch( (*diter).dependenceType){
    case RAW:
      cout << "RAW \t";
      break;
    case WAR:
      cout << "WAR \t";
      break;
    case WAW:
      cout << "WAW \t";
      break;
    default:
      break;
    }

    cout << "$" << (*diter).registerNumber << " \t";
    cout << "(" << (*diter).previousInstructionNumber << ", ";
    cout << (*diter).currentInstructionNumber << ")" << endl;
  }


}
