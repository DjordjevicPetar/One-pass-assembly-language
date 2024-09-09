#ifndef ASEMBLER_HPP
#define ASEMBLER_HPP
#include "../parser.tab.hpp"
#include "Rel/LiteralPool.hpp"
#include "Rel/BackPatching.hpp"
#include "Instructions/Instruction.hpp"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
extern FILE* yyin;
extern int yyparse();

class Asembler {
private:
  FILE *in;
  std::fstream outBin;
  std::ofstream outTxt;
  static Asembler* _singleton;
  Asembler(int argc, char* argv[]);
public:
  static int Init(int argc, char* argv[]);
  static Asembler* getInstance();
  std::fstream& getOutBin();
  std::ofstream& getOutTxt();
};

#endif