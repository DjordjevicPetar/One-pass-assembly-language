#include "../../inc/Rel/BackPatching.hpp"
#include <iostream>

BackPatching* BackPatching::singleton_ = nullptr;

BackPatching* BackPatching::getInstance(){
  if(singleton_ == nullptr){
    singleton_ = new BackPatching();
  }
  return singleton_;
}

BackPatching::BackPatching(){
  patchingList = new std::list<Record>();
  relTable = new std::list<Relocation>();
}

void BackPatching::AddPatch(int address, std::string name, int literal){
  Record patch(address, name, literal);
  patchingList->push_back(patch);
}

BackPatching::Record::Record(int address, std::string name, int literal){
  this->address = address;
  this->name = name;
  this->literal = literal;
}

BackPatching::Relocation::Relocation(int address, std::string name, int section, int addEnd){
  this->name = name;
  this->section = section;
  this->address = address;
  this->addEnd = addEnd;
}

void BackPatching::Resolve(int literal, std::string symbol, std::fstream& binFile){
  int cur = binFile.tellp(), rel;
  int out;
  for (std::list<Record>::iterator i = patchingList->begin(); i != patchingList->end();)
  {
    if((i->literal == literal && symbol == "") || (i->name == symbol && symbol != "")){
      rel = cur - i->address - 4;
      if(!(rel < 2048 && rel > -2049)){
        printf("ERROR!! Section too long\n");
      }
      binFile.seekp(i->address);
      binFile.read((char*)(&out), sizeof(out));
      binFile.seekg(i->address);
      out += (rel & 0xFFF);
      binFile.write((char*)(&out), sizeof(out));
      binFile.seekp(cur);
      binFile.seekg(cur);
      std::list<Record>::iterator temp = i;
      temp++;
      patchingList->erase(i);
      i = temp;
    }
    else{
      i++;
    }
  }
  if(symbol != "")AddAssemblyRelocation(cur, symbol);
};

void BackPatching::AddAssemblyRelocation(int address, std::string symbol){
  int section = SymbolTable::getInstance()->getLastSection();
  address -= SymbolTable::getInstance()->getSectionsStart(section);
  Relocation rel(address, symbol, section);
  relTable->push_back(rel);
}

void BackPatching::fixLocalSymbols(std::fstream& binFile){
  int cur = binFile.tellp();
  for(auto rel = relTable->begin(); rel != relTable->end();){
    auto temp = SymbolTable::getInstance()->getRelocInfo(rel->name);
    rel->name = temp.first;
    rel->addEnd = temp.second;
    if(rel->name != "")rel++;
    else{
      binFile.seekp(rel->address + SymbolTable::getInstance()->getSectionsStart(rel->section));
      binFile.write((char*)(&rel->addEnd), sizeof(rel->addEnd));
      auto temp = rel;
      temp++;
      relTable->erase(rel);
      rel = temp;
    }
  }
  binFile.seekp(cur);
}

void BackPatching::PrintTxt(std::ofstream& txtFile, int section){
  txtFile << "----------Tabela relokacija-------------" << std::endl;
  txtFile << "----------Sekcija broj " << section << "-------------" << std::endl;
  txtFile << "RelAddress:  Name:  Addend:" << std::endl;
  for(std::list<Relocation>::iterator i = relTable->begin(); i != relTable->end(); i++)
  {
    if(i->section != section)continue;
    txtFile << i->address << " " << i->name << " " << i->addEnd << std::endl;
  }
}

void BackPatching::PrintBinary(std::fstream& binFile, int section){
  int pos = binFile.tellp();
  int recordNumber = 0;
  binFile.write((char*)&recordNumber, sizeof(recordNumber));
  for(std::list<Relocation>::iterator i = relTable->begin(); i != relTable->end(); i++)
  {
    if(i->section != section)continue;
    recordNumber++;
    binFile.write((char*)&i->address, sizeof(i->address));
    int out = i->name.size();
    binFile.write((char*)&out, sizeof(out));
    binFile << i->name.c_str();
    binFile.write((char*)&i->addEnd, sizeof(i->addEnd));
  }
  int cur = binFile.tellp();
  binFile.seekp(pos);
  binFile.write((char*)&recordNumber, sizeof(recordNumber));
  binFile.seekp(cur);
}

void BackPatching::readRelocations(std::ifstream& binFile, int fileId, int section_count){
  int relocation_count;
  std::string name;
  int address, name_size, addEnd;
  for (int i = 0; i < section_count; i++)
  {
    binFile.read((char*)&relocation_count, sizeof(relocation_count));
    for (int j = 0; j < relocation_count; j++)
    {
      binFile.read((char*)(&address), sizeof(address));
      binFile.read((char*)(&name_size), sizeof(name_size));
      char* raw_string = (char*)malloc(name_size + 1);
      binFile.read(raw_string, name_size);
      raw_string[name_size] = '\0';
      name = std::string(raw_string);
      binFile.read((char*)(&addEnd), sizeof(addEnd));
      Relocation relocation = Relocation(address, name, SymbolTable::getInstance()->findSectionByFile(i, fileId), addEnd);
      relocation.fileId = fileId;
      relTable->push_back(relocation);
    }
  }
  
}

void BackPatching::ResolveSymbol(std::unordered_map<int, unsigned char>& hex, std::string name, int fileId, int patch, bool global){
  for (std::list<Relocation>::iterator i = relTable->begin(); i != relTable->end();)
  {
    if(name == i->name && (global || fileId == i->fileId)){
      i->address += SymbolTable::getInstance()->getSectionsStart(i->section);
      patch += i->addEnd;
      hex[i->address + 3] = (char)(patch >> 24);
      hex[i->address + 2] = (char)(patch >> 16);
      hex[i->address + 1] = (char)(patch >> 8);
      hex[i->address + 0] = (char)(patch >> 0);
      patch -= i->addEnd;
      std::list<Relocation>::iterator temp = i;
      temp++;
      relTable->erase(i);
      i = temp;
    }
    else{
      i++;
    }
  }
}