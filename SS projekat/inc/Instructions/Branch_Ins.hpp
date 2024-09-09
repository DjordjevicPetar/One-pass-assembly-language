#ifndef BRANCH_INS_HPP
#define BRANCH_INS_HPP
#include "Instruction.hpp"

enum BRANCH_Codes {
  CALL_SHORT_CODE = 32,
  CALL_CODE = 33,
  JMP_SHORT_CODE = 48,
  BEQ_SHORT_CODE = 49,
  BNE_SHORT_CODE = 50,
  BGT_SHORT_CODE = 51,
  JMP_CODE = 56,
  BEQ_CODE = 57,
  BNE_CODE = 58,
  BGT_CODE = 59
};

class Branch_Ins: public Instruction{
private:
  BRANCH_Codes code;
  int A, B, C;
  std::string jumpPointSymbol;
  int jumpPointLiteral;
public:
  Branch_Ins(BRANCH_Codes code, int A, int B, int C, std::string jumpPointSymbol, int jumpPointLiteral);
  virtual void Visit();
};
#endif