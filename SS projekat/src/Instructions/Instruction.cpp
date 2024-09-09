#include "../../inc/Instructions/Instruction.hpp"

std::fstream* Instruction::outBin = nullptr;

Instruction::Instruction(){

}

void Instruction::setOutput(std::fstream& outFile){
  outBin = &outFile;
}

std::fstream& Instruction::getOutput(){
  return *outBin;
}

void Instruction::writeInstruction(int code, int ra, int rb, int rc, std::string symbol, bool branch){
  int out = code & 0xFF;
  out = (out << 4) + (ra & 0xF);
  out = (out << 4) + (rb & 0xF);
  out = (out << 4) + (rc & 0xF);
  out = (out << 12) + (writeSymbol(symbol, branch && SymbolTable::getInstance()->branchSearch(symbol) != 0) & 0xFFF);
  outBin->write(reinterpret_cast<char*>(&out), sizeof(out));
}

void Instruction::writeInstruction(int code, int ra, int rb, int rc, int literal){
  int out = code & 0xFF;
  out = (out << 4) + (ra & 0xF);
  out = (out << 4) + (rb & 0xF);
  out = (out << 4) + (rc & 0xF);
  out = (out << 12) + (writeLiteral(literal) & 0xFFF);
  outBin->write(reinterpret_cast<char*>(&out), sizeof(out));
}

int Instruction::writeSymbol(std::string symbol, bool branch){
  if(branch){
    int symbolPos = SymbolTable::getInstance()->branchSearch(symbol);
    return symbolPos - outBin->tellp() - 4;
  }
  LiteralPool::getInstance()->AddSymbol(symbol);
  BackPatching::getInstance()->AddPatch((int)outBin->tellp(), symbol, 0);
  return 0;
}

int Instruction::writeLiteral(int data){
  if(!(data < 2048 && data > -2049)){
    LiteralPool::getInstance()->AddLiteral(data);
    BackPatching::getInstance()->AddPatch((int)outBin->tellp(), "", data);
    return 0;
  }
  return data;
}