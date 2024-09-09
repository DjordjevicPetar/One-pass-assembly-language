#ifndef MEMORY_INS_HPP
#define MEMORY_INS_HPP
#include "Instruction.hpp"
#include "../Rel/TNS.hpp"

enum MEMORY_Codes {
  LD_Imm,
  LD_Dir,
  LD_IndImm,
  LD_IndDir,
  LD_Reg,
  LD_IndReg,
  LD_RegImm,
  LD_RegSym,
  ST_Dir,
  ST_IndImm,
  ST_IndDir,
  ST_Reg,
  ST_IndReg,
  ST_RegImm,
  ST_RegSym
};

class Memory_Ins: public Instruction {
  private:
  MEMORY_Codes code;
  int reg, literal, dest;
  std::string symbol;
public:
  Memory_Ins(MEMORY_Codes code, int dest, int literal, std::string symbol, int reg);
  virtual void Visit();
};
#endif