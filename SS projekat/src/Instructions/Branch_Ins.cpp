#include "../../inc/Instructions/Branch_Ins.hpp"

Branch_Ins::Branch_Ins(BRANCH_Codes code, int A, int B, int C, std::string jumpPointSymbol, int jumpPointLiteral){
  int symbolPos = SymbolTable::getInstance()->branchSearch(jumpPointSymbol);
  bool shortLiteral = (jumpPointSymbol == "" && jumpPointLiteral < 2048 && jumpPointLiteral > -2049) || symbolPos != 0;
  this->code = code;
  switch (code)
  {
  case CALL_CODE:
    if(shortLiteral)this->code = CALL_SHORT_CODE;
    break;
  case JMP_CODE:
    if(shortLiteral)this->code = JMP_SHORT_CODE;
    break;
  case BEQ_CODE:
    if(shortLiteral)this->code = BEQ_SHORT_CODE;
    break;
  case BNE_CODE:
    if(shortLiteral)this->code = BNE_SHORT_CODE;
    break;
  case BGT_CODE:
    if(shortLiteral)this->code = BGT_SHORT_CODE;
    break;
  }
  this->A = A;
  this->B = B;
  this->C = C;
  this->jumpPointSymbol = jumpPointSymbol;
  this->jumpPointLiteral = jumpPointLiteral;
}

void Branch_Ins::Visit(){
  if(jumpPointSymbol != "")Instruction::writeInstruction(code, A, B, C, jumpPointSymbol, true);
  else Instruction::writeInstruction(code, A, B, C, jumpPointLiteral);
}