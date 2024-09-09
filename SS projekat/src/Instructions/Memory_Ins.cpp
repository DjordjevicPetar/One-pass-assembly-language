#include "../../inc/Instructions/Memory_Ins.hpp"

Memory_Ins::Memory_Ins(MEMORY_Codes code, int dest, int literal, std::string symbol, int reg){
  this->code = code;
  this->dest = dest;
  this->literal = literal;
  this->symbol = symbol;
  this->reg = reg;
}

void Memory_Ins::Visit(){
  bool shortLit = literal < 2048 && literal > -2049;
  switch (code)
  {
  case LD_Imm:
    if(shortLit)
      Instruction::writeInstruction(145, dest, 0, 0, literal);
    else
      Instruction::writeInstruction(146, dest, 15, 0, literal);
    break;
  case LD_Dir:
    Instruction::writeInstruction(146, dest, 15, 0, symbol);
    break;
  case LD_IndImm:
    if(shortLit)
      Instruction::writeInstruction(146, dest, 0, 0, literal);
    else{
      Instruction::writeInstruction(146, dest, 15, 0, literal);
      Instruction::writeInstruction(146, dest, dest, 0, 0);
    }
    break;
  case LD_IndDir:
    Instruction::writeInstruction(146, dest, 15, 0, symbol);
    Instruction::writeInstruction(146, dest, dest, 0, 0);
    break;
  case LD_Reg:
    Instruction::writeInstruction(145, dest, reg, 0, 0);
    break;
  case LD_IndReg:
    Instruction::writeInstruction(146, dest, reg, 0, 0);
    break;
  case LD_RegImm:
    if(shortLit)
      Instruction::writeInstruction(146, dest, reg, 0, literal);
    else{
      Instruction::writeInstruction(146, dest, 15, 0, literal);
      Instruction::writeInstruction(146, dest, reg, dest, 0);
    }
    break;
  case LD_RegSym:
    Instruction::writeInstruction(146, dest, 15, 0, symbol);
    Instruction::writeInstruction(146, dest, reg, dest, 0);
    break;
  case ST_Dir:
    Instruction::writeInstruction(128, 15, 0, dest, symbol);
    break;
  case ST_IndImm:
    if(shortLit)
      Instruction::writeInstruction(128, 0, 0, dest, literal);
    else
      Instruction::writeInstruction(130, 15, 0, dest, literal);
    break;
  case ST_IndDir:
    Instruction::writeInstruction(130, 15, 0, dest, symbol);
    break;
  case ST_Reg:
    Instruction::writeInstruction(145, reg, dest, 0, 0);
    break;
  case ST_IndReg:
    Instruction::writeInstruction(128, reg, 0, dest, 0);
    break;
  case ST_RegSym:
    TNS::getInstance()->AddFix(Instruction::getOutput().tellp(), symbol);
    Instruction::writeInstruction(128, reg, 0, dest, 0);
    break;
  case ST_RegImm:
    if(!shortLit){
      std::cout << "In instruction with [reg + literal], literal cannot be more that 12 bits." << std::endl;
      exit(-35);
    }
    Instruction::writeInstruction(128, reg, 0, dest, literal);
  default:
    break;
  }
}

