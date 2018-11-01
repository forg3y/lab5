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
    if(myOpcodeTable.RSposition(i.getOpcode()) != -1){
      cout << "In Switch- RTYPE, about to checkREAD1" << endl;
      checkForReadDependence(i.getRS());
    }
    if(myOpcodeTable.RTposition(i.getOpcode()) != -1){
      cout << "In Switch- RTYPE, about to checkREAD2" << endl;
      checkForReadDependence(i.getRT());
    }
    if(myOpcodeTable.RDposition(i.getOpcode()) != -1){
      cout << "In Switch- RTYPE, about to checkWRITE" << endl;
      checkForWriteDependence(i.getRD());
    }
    break;
  case ITYPE:
    if(myOpcodeTable.RSposition(i.getOpcode()) != -1){
      cout << "In Switch- ITYPE, about to checkREAD" << endl;
      checkForReadDependence(i.getRS());
    }
    if(myOpcodeTable.RTposition(i.getOpcode()) != -1){
      cout << "In Switch- ITYPE, about to checkWRITE" << endl;
      checkForWriteDependence(i.getRT());
    }
    break;
  case JTYPE:
    // do nothing, only immediate field
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
  // Find RegisterInfo() associated with reg
  RegisterInfo r = myCurrentState.at(reg);
  int instCount = myInstructions.size();

  // Check the RegisterInfo() things like last instruction, last access
  int lastI = r.lastInstructionToAccess;
  AccessType lastA = r.accessType;

  // if last instruction is -1, this is the first time accessing reg
  // so we update lastInstructionToAccess and AccessType, then exit
  if(lastA == A_UNDEFINED){
    cout << "READDEP-> In: lastA==A_UNDEFINED" << endl;    
    r.lastInstructionToAccess = instCount;
    r.accessType = READ;
    return;
  }

  // if the current access type is a read, and the previous access type was a read,
  // update lastInstructionToAcccess, then exit
  if (lastA == READ){
    cout << "READDEP-> In: lastA==READ" << endl;
    r.lastInstructionToAccess = instCount;
    return;
  }

  //Case: RAW
  // if the current access type is a read, and the previous access type was a write,
  // update lastInstructionAccess, update AccessType, create a new Dependence
  // and add to myDependences, then exit
  if(lastA == WRITE) {
    cout << "READDEP-> In: lastA==WRITE" << endl;
    r.lastInstructionToAccess = instCount;
    r.accessType = READ;
  // Create new dependence
    Dependence raw;
    raw.dependenceType = RAW;        
    raw.registerNumber = reg;
    raw.previousInstructionNumber = lastI;
    raw.currentInstructionNumber = instCount;
  // Add to myDependences
    myDependences.push_back(raw);
    return;
  }

}

void DependencyChecker::checkForWriteDependence(unsigned int reg)
  /* Determines if a write data dependence occurs when reg is written by the current
   * instruction.  If so, adds an entry to the list of dependences. Also updates 
   * the appropriate RegisterInfo entry regardless of dependence detection.
   */
{
  // Find RegisterInfo() associated with reg
  RegisterInfo r = myCurrentState.at(reg);
  int instCount = myInstructions.size();

  // Check the RegisterInfo() things like last instruction, last access
  int lastI = r.lastInstructionToAccess;
  AccessType lastA = r.accessType;

  // if last instruction is -1, this is the first time accessing reg
  // so we update lastInstructionToAccess and AccessType, then exit
  if(lastA == A_UNDEFINED){
    cout << "WRITEDEP-> In: lastA==A_UNDEFINED" << endl;
    r.lastInstructionToAccess = instCount;
    r.accessType = WRITE;
    return;
  }

  //Case: WAR
  // if the current access type is a write, and the previous access type was a read,
  // update lastInstructionAccess, update AccessType, create a new Dependence
  // and add to myDependences, then exit
  if(lastA == READ) {
    cout << "WRITEDEP-> In: lastA==READ" << endl;

    r.lastInstructionToAccess = instCount;
    r.accessType = WRITE;
  // Create new dependence
    Dependence war;
    war.dependenceType = WAR;        
    war.registerNumber = reg;
    war.previousInstructionNumber = lastI;
    war.currentInstructionNumber = instCount;
  // Add to myDependences
    myDependences.push_back(war);
    return;
  }

  //Case: WAW
  // if the current access type is a write, and the previous access type was a write,
  // update lastInstructionAccess, update AccessType, create a new Dependence
  // and add to myDependences, then exit
  if(lastA == WRITE) {
    r.lastInstructionToAccess = instCount;
    cout << "WRITEDEP-> In: lastA==WRITE" << endl;

    // no need to update accessType!
  
  // Create new dependence
    Dependence waw;
    waw.dependenceType = WAW;        
    waw.registerNumber = reg;
    waw.previousInstructionNumber = lastI;
    waw.currentInstructionNumber = instCount;
  // Add to myDependences
    myDependences.push_back(waw);
    return;
  }
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
