#ifndef TNS_HPP
#define TNS_HPP
#include <string>
#include <list>
#include "SymbolTable.hpp"

class TNS
{
private:
  struct ExpressionEntry
  {
    std::string newSymbol;
    std::string firstSymbol;
    std::string secondSymbol;
    int section;
  };
  struct Fix
  {
    int address;
    std::string symbol;
  };
  static TNS* _singleton;
  std::list<ExpressionEntry> symbols;
  std::list<Fix> fixes;
  TNS();
public:
  static TNS* getInstance();
  void AddEntry(std::string, std::string, std::string, int);
  void AddFix(int address, std::string symbol);
  void Resolve(std::fstream& outFile);
};
#endif