#include "../../inc/Instructions/Misc_Ins.hpp"

Misc_Ins::Misc_Ins(MISC_Codes code){
  this->code = code;
}

void Misc_Ins::Visit(){
  if(code == HALT_CODE || code == INT_CODE)
    Instruction::writeInstruction(code, 0, 0, 0, 0);
  else if(code == IRET_CODE){
    Instruction::writeInstruction(POP_CSR_CODE, 0, 14, 0, 4);
    Instruction::writeInstruction(POP_GPR_CODE, 15, 14, 0, 8);
  }
  else
    Instruction::writeInstruction(POP_GPR_CODE, 15, 14, 0, 4);
}