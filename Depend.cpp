#include "ASMParser.h"
#include "DependencyChecker.h"
#include <iostream>
#include <stdlib.h>

using namespace std;

int main(int argc, char *argv[])
{
  DependencyChecker checker;
  ASMParser *parser;

  if(argc != 2){
    cerr << "Need to enter input file name" << endl;
    exit(1);
  }

  parser = new ASMParser(argv[1]);

  if(parser->isFormatCorrect() == false){
    cerr << "Format of input file is incorrect " << endl;
    exit(1);
  }

  Instruction i;

  i = parser->getNextInstruction();
  while( i.getOpcode() != UNDEFINED){
    //    cout << i.getAssembly() << endl;
    checker.addInstruction(i);
    i = parser->getNextInstruction();
  }
  checker.printDependences();
  
  delete parser;
}
