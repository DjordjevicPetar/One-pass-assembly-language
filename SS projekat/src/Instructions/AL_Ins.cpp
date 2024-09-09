#include "../../inc/Instructions/AL_Ins.hpp"

AL_Ins::AL_Ins(AL_Codes code, int A, int B, int C){
  this->code = code;
  this->A = A;
  this->B = B;
  this->C = C;
}
void AL_Ins::Visit(){
  Instruction::writeInstruction(code, A, B, C, 0);
}