#ifndef LITERAL_POOL_HPP
#define LITERAL_POOL_HPP
#include <vector>
#include <string>
#include <fstream>
#include "BackPatching.hpp"

class LiteralPool{
private:
  int curSection;
  struct Record{
    int literal;
    std::string symbol;
    Record(int literal);
    Record(std::string symbol);
  };
  std::vector<Record> PoolRecords;
  LiteralPool();
  static LiteralPool* _singleton;
public:
  static LiteralPool* getInstance();
  void AddLiteral(int number);
  void AddSymbol(std::string symbol);
  void EmptyPool(std::fstream& binaryFile);
};
#endif