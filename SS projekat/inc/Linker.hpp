#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <string.h>
#include <unordered_map>
#include "../inc/Rel/BackPatching.hpp"
#include "../inc/Rel/SymbolTable.hpp"

class Linker
{
private:
  static Linker* _singleton;
  bool hex, relocatable;
  std::fstream outFile;
  std::vector<std::ifstream> linkedFiles;
  std::unordered_map<std::string, int> place;
  std::unordered_map<int, unsigned char> hexOut;
  Linker();
public:
  static Linker* getInstance();
  int Init(int argc, char* argv[]);
  void main();
  void Hex();
  void Rel();
  void readRelevantInfo();
};