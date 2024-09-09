#ifndef SYMBOL_TABLE_HPP
#define SYMBOL_TABLE_HPP
#include <string>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <fstream>
#include "BackPatching.hpp"

class SymbolTable {

private:
  struct SectionRecord{
    std::string name;
    int startPosition;
    int size;
    int fileId;
    SectionRecord(std::string name, int address);
    SectionRecord(std::string name, int startPosition, int size, int fileId);
  };

  struct SymbolRecord{
    int rel, section;
    bool glob, defined, equ;
    std::string name;
    SymbolRecord(bool glob, std::string name);
  };

  static SymbolTable* singleton_;

  std::vector<SectionRecord> sections;
  std::vector<SymbolRecord> symbols;
  SymbolTable();
  int findSymbol(std::string name);
public:
  static SymbolTable* getInstance();
  void DeclareSection(std::string section, int address);
  void EndSection(int address);
  void DeclareSymbol(bool glob, std::string name);
  void DefineSymbol(std::string name, int address, bool equ=false);
  int getLastSection();
  int getSectionsStart(int section);
  bool TrySolve(std::string, std::string, std::string, int);
  std::pair<std::string, int> getRelocInfo(std::string symbol);
  int getConstant(std::string);
  void WriteTxt(std::ofstream& txtFile);
  void WriteBinary(std::fstream& binaryFile);
  int branchSearch(std::string name);
  void WriteSectionCode(int section, std::fstream& binaryFile, std::ofstream& txtFile);
  void readSections(std::ifstream& stream, int id, int section_count);
  void writeHex(std::unordered_map<int, unsigned char>& hex, std::unordered_map<std::string, int>& place, std::vector<std::ifstream>& linkedFiles);
  void writeCode(std::fstream& binaryFile, std::vector<std::ifstream>& linkedFiles);
  void readSymbols(std::ifstream& stream, int id);
  int findSectionByFile(int section, int id);
  void ResolveRelocations(std::unordered_map<int, unsigned char>& hex);
};
#endif