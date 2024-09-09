#ifndef CSR_INS_HPP
#define CSR_INS_HPP
#include "Instruction.hpp"

enum CSR_Codes {
  CSRRD_CODE = 144,
  CSRWR_CODE = 148
};

class CSR_Ins: public Instruction{
private:
  CSR_Codes code;
  int A, B;
public:
  CSR_Ins(CSR_Codes code, int A, int B);
  virtual void Visit();
};
#endif