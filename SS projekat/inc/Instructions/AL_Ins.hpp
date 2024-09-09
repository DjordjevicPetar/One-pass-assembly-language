#ifndef AL_INS_HPP
#define AL_INS_HPP
#include "Instruction.hpp"

enum AL_Codes {
  XCHG_CODE = 64,
  ADD_CODE = 80,
  SUB_CODE = 81,
  MUL_CODE = 82,
  DIV_CODE = 83,
  NOT_CODE = 96,
  AND_CODE = 97,
  OR_CODE = 98,
  XOR_CODE = 99,
  SHL_CODE = 112,
  SHR_CODE = 113
};

class AL_Ins: public Instruction{
private:
  AL_Codes code;
  int A, B, C;
public:
  AL_Ins(AL_Codes code, int A, int B, int C);
  virtual void Visit();
};
#endif