#ifndef BACK_PATCHING_HPP
#define BACK_PATCHING_HPP
#include <string>
#include <list>
#include <fstream>
#include "SymbolTable.hpp"

class BackPatching {

private:
  struct Record{
    int address;
    std::string name;
    int literal;
    Record(int address, std::string name, int literal);
  };

  struct Relocation{
    int address;
    std::string name;
    int addEnd;
    int section;
    int fileId;
    Relocation(int address, std::string name, int section, int addEnd = 0);
  };

  std::list<Record>* patchingList;
  std::list<Relocation>* relTable;
  static BackPatching* singleton_;

  BackPatching();
public:
  static BackPatching* getInstance();
  void AddPatch(int address, std::string name, int literal);
  void AddAssemblyRelocation(int address, std::string symbol);
  void Resolve(int literal, std::string, std::fstream& binFile);
  void fixLocalSymbols(std::fstream& binFile);
  void PrintTxt(std::ofstream& txtFile, int section);
  void PrintBinary(std::fstream& binFile, int section);
  void readRelocations(std::ifstream& binFile, int fileId, int section_count);
  void ResolveSymbol(std::unordered_map<int, unsigned char>& hex, std::string name, int fileId, int patch, bool global);
};
#endif