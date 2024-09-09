#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP
#include "../Asembler.hpp"
#include <fstream>

class Instruction{
private:
  static std::fstream* outBin;
public:
  Instruction();
  static void setOutput(std::fstream& outFile);
  static std::fstream& getOutput();
  virtual void Visit() = 0;
  void writeInstruction(int code, int ra, int rb, int rc, std::string symbol, bool branch=false);
  void writeInstruction(int code, int ra, int rb, int rc, int literal);
  int writeSymbol(std::string symbol, bool branch = false);
  int writeLiteral(int data);
};
#endif