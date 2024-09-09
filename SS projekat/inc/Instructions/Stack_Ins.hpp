#ifndef STACK_INS_HPP
#define STACK_INS_HPP
#include "Instruction.hpp"

enum STACK_Codes {
  PUSH_CODE = 129,
  POP_CODE = 147
};

class Stack_Ins: public Instruction {
  private:
  STACK_Codes code;
  int op;
public:
  Stack_Ins(STACK_Codes code, int op);
  virtual void Visit();
};
#endif