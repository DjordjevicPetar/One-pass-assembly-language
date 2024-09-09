#include "../../inc/Instructions/Stack_Ins.hpp"

Stack_Ins::Stack_Ins(STACK_Codes code, int op){
  this->code = code;
  this->op = op;
}

void Stack_Ins::Visit(){
  if(code == PUSH_CODE)
    Instruction::writeInstruction(code, 14, 0, op, -4);
  else
    Instruction::writeInstruction(code, op, 14, 0, 4);
}