#include "../../inc/Rel/TNS.hpp"

TNS* TNS::_singleton = nullptr;

TNS::TNS(){

}

TNS* TNS::getInstance(){
  if(_singleton == nullptr)_singleton = new TNS();
  return _singleton;
};

void TNS::AddEntry(std::string newSymbol, std::string firstSymbol, std::string secondSymbol, int section){
  ExpressionEntry entry;
  entry.newSymbol = newSymbol;
  entry.firstSymbol = firstSymbol;
  entry.secondSymbol = secondSymbol;
  entry.section = section;
  symbols.push_back(entry);
};

void TNS::AddFix(int address, std::string name){
  Fix newFix;
  newFix.address = address;
  newFix.symbol = name;
  fixes.push_back(newFix);
}

void TNS::Resolve(std::fstream& outFile){
  int solved = 1;
  while(solved != 0){
    solved = 0;
    for (std::list<ExpressionEntry>::iterator i = symbols.begin(); i != symbols.end();)
    {
      if(SymbolTable::getInstance()->TrySolve(i->newSymbol, i->firstSymbol, i->secondSymbol, i->section)){
        solved++;
        std::list<ExpressionEntry>::iterator temp = i;
        temp++;
        symbols.erase(i);
        i = temp;
      }
      else{
        i++;
      }
    }
  }
  if(symbols.size() > 0){
    std::cout << "Unsolvable equ symbols!!!" << std::endl;
    exit(-50);
  }
  int out;
  unsigned int instruction;
  int where;
  int cur;
  cur = outFile.tellg();
  for (auto i = fixes.begin(); i != fixes.end(); i++)
  {
    out = SymbolTable::getInstance()->getConstant(i->symbol);
    if(out > 2046 || out < -2047){
      std::cout << "Constant cannot fit into 12 bits" << std::endl;
      exit(-45);
    }
    outFile.seekg(i->address);
    outFile.read((char*)&instruction, sizeof(instruction));
    instruction = instruction + (out & 0xFFF);
    outFile.seekg(i->address);
    outFile.write((char*)&instruction, sizeof(instruction));
  }
  outFile.seekg(cur);
}