#ifndef MISC_INS_HPP
#define MISC_INS_HPP
#include "Instruction.hpp"

enum MISC_Codes {
  HALT_CODE = 0,
  INT_CODE = 16,
  IRET_CODE,
  RET_CODE,
  POP_GPR_CODE = 147,
  POP_CSR_CODE = 150
};

class Misc_Ins: public Instruction {
  private:
  MISC_Codes code;
public:
  Misc_Ins(MISC_Codes code);
  virtual void Visit();
};
#endif