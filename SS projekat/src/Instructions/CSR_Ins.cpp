#include "../../inc/Instructions/CSR_Ins.hpp"

CSR_Ins::CSR_Ins(CSR_Codes code, int A, int B){
  this->code = code;
  this->A = A;
  this->B = B;
}
void CSR_Ins::Visit(){
  Instruction::writeInstruction(code, A, B, 0, 0);
}