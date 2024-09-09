#include "../../inc/Rel/LiteralPool.hpp"
LiteralPool* LiteralPool::_singleton = nullptr;

LiteralPool::Record::Record(int literal){
  this->literal = literal;
  this->symbol = "";
}

LiteralPool::Record::Record(std::string symbol){
  this->literal = 0;
  this->symbol = symbol;
}

LiteralPool* LiteralPool::getInstance(){
  if(_singleton == nullptr)_singleton = new LiteralPool();
  return _singleton;
}

LiteralPool::LiteralPool(){

}

void LiteralPool::AddLiteral(int number){
  for(int i = 0; i < PoolRecords.size(); i++){
    if(PoolRecords[i].literal == number)return;
  }
  Record newRecord(number);
  PoolRecords.push_back(newRecord);
}

void LiteralPool::AddSymbol(std::string symbol){
  for(int i = 0; i < PoolRecords.size(); i++){
    if(PoolRecords[i].symbol == symbol)return;
  }
  Record newRecord(symbol);
  PoolRecords.push_back(newRecord);
}

void LiteralPool::EmptyPool(std::fstream& binaryFile){
  for(int i = 0; i < PoolRecords.size(); i++){
    BackPatching::getInstance()->Resolve(PoolRecords[i].literal, PoolRecords[i].symbol, binaryFile);
    binaryFile.write(reinterpret_cast<char*>(&PoolRecords[i].literal), sizeof(PoolRecords[i].literal));
  }
  PoolRecords.clear();
};