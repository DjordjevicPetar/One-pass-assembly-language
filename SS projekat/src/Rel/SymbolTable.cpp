#include "../../inc/Rel/SymbolTable.hpp"

SymbolTable* SymbolTable::singleton_ = nullptr;

SymbolTable::SymbolTable(){
  symbols = std::vector<SymbolRecord>();
  sections = std::vector<SectionRecord>();
}

SymbolTable* SymbolTable::getInstance()
{
  if(singleton_ == nullptr){
    singleton_ = new SymbolTable();
  }
  return singleton_;
}

SymbolTable::SectionRecord::SectionRecord(std::string name, int address){
  this->name = name;
  this->startPosition = address;
}

SymbolTable::SectionRecord::SectionRecord(std::string name, int startPosition, int size, int fileId){
  this->name = name;
  this->startPosition = startPosition;
  this->size = size;
  this->fileId = fileId;
}

SymbolTable::SymbolRecord::SymbolRecord(bool glob, std::string name){
  this->glob = glob;
  this->name = name;
  this->defined = false;
  this->rel = 0;
  this->section = -1;
}

int SymbolTable::findSymbol(std::string name){
  for (int i = 0; i < symbols.size(); i++)if(symbols[i].name.compare(name) == 0)return i;
  return -1;
}

void SymbolTable::DeclareSection(std::string section, int address) 
{
   for (int i = 0; i < sections.size(); i++)if(sections[i].name.compare(section) == 0){
    std::cout << "Already defined handlersection" << std::endl;
    exit(-1);
  }
  SectionRecord sec(section, address);
  sections.push_back(sec);
}

void SymbolTable::EndSection(int address){

  // Error: Something written in assembly before section declaration
  if(sections.size() == 0 && address > 4){
    std::cout << "Only directives can be written before section declaration" << std::endl;
    exit(-1);
  }

  //First section is being declared
  if(sections.size() == 0)return;

  //Section End
  sections[sections.size() - 1].size = address - sections[sections.size() - 1].startPosition;
}

void SymbolTable::DeclareSymbol(bool glob, std::string name){
  if(findSymbol(name) != -1){
    symbols[findSymbol(name)].glob |= glob;
    return;
  }

  //First declaration
  SymbolRecord s(glob, name);
  symbols.push_back(s);
}

void SymbolTable::DefineSymbol(std::string name, int address, bool equ){

  //Symbol already defined
  if(symbols[findSymbol(name)].defined){
    std::cout << "Already defined symbol" << std::endl;
    exit(-1);
  }

  symbols[findSymbol(name)].section = sections.size() - 1;
  symbols[findSymbol(name)].defined = true;
  symbols[findSymbol(name)].equ = equ;
  symbols[findSymbol(name)].rel = address - ((equ)?0:sections[sections.size() - 1].startPosition);

}

int SymbolTable::getLastSection(){
  return sections.size() - 1;
}

int SymbolTable::getSectionsStart(int section){
  if(section == -1)return 0;
  return sections[section].startPosition;
}

int SymbolTable::branchSearch(std::string name){
  int i = findSymbol(name);
  if(i == -1 || symbols[i].section != sections.size() - 1 || !symbols[i].defined)return 0;
  return symbols[i].rel + sections[symbols[i].section].startPosition;
}

bool SymbolTable::TrySolve(std::string newSymbol, std::string firstSymbol, std::string secondSymbol, int section){
  if(!symbols[findSymbol(firstSymbol)].defined || !symbols[findSymbol(secondSymbol)].defined)return false;
  if(symbols[findSymbol(firstSymbol)].section != symbols[findSymbol(secondSymbol)].section)return false;
  int pos = findSymbol(newSymbol);
  symbols[pos].defined = true;
  symbols[pos].equ = true;
  symbols[pos].rel = symbols[findSymbol(firstSymbol)].rel - symbols[findSymbol(secondSymbol)].rel;
  symbols[pos].section = section;
  return true;
}

std::pair<std::string, int> SymbolTable::getRelocInfo(std::string symbol){
  SymbolRecord& record = symbols[findSymbol(symbol)];
  if(record.glob)return std::make_pair(record.name, 0);
  else if(!record.equ) return std::make_pair(sections[record.section].name, record.rel);
  else return std::make_pair("", record.rel);
}

int SymbolTable::getConstant(std::string name){
  int pos = findSymbol(name);
  if(pos == -1 || !symbols[pos].equ){
    std::cout << "Undefined constant" << std::endl;
    exit(-37);
  }
  return symbols[pos].rel;
}

void SymbolTable::WriteTxt(std::ofstream& txtFile){
  txtFile << "----------Tabela sekcija-------------" << std::endl;
  txtFile << "Id:  Name:   Start:    Size:" << std::endl;
  for (int i = 0; i < sections.size(); i++)
    txtFile << i << ":" << sections[i].name << " " << sections[i].startPosition << " " << sections[i].size << std::endl;

  for (int i = 0; i < sections.size(); i++)BackPatching::getInstance()->PrintTxt(txtFile, i);

  txtFile << "----------Tabela simbola-------------" << std::endl;
  txtFile << "Id:  Name:   Value:    Section:   Defined:   Global:   Equ:" << std::endl;
  for (int i = 0; i < symbols.size(); i++)
    txtFile << i << ":" << symbols[i].name << " " << symbols[i].rel << " " << symbols[i].section <<
      " " << symbols[i].defined << " " << symbols[i].glob << " " << symbols[i].equ << std::endl;
};

void SymbolTable::WriteBinary(std::fstream& binaryFile){
  int out = sections.size();
  binaryFile.write(reinterpret_cast<char*>(&out), sizeof(out));
  for (int i = 0; i < sections.size(); i++)
  {
    out = sections[i].name.size();
    binaryFile.write(reinterpret_cast<char*>(&out), sizeof(out));
    binaryFile << sections[i].name.c_str();
    out = sections[i].startPosition;
    binaryFile.write(reinterpret_cast<char*>(&out), sizeof(out));
    out = sections[i].size;
    binaryFile.write(reinterpret_cast<char*>(&out), sizeof(out));
  }

  for (int i = 0; i < sections.size(); i++)BackPatching::getInstance()->PrintBinary(binaryFile, i);

  out = symbols.size();
  int get_back = binaryFile.tellp();
  int count = 0;
  binaryFile.write(reinterpret_cast<char*>(&out), sizeof(out));
  for (int i = 0; i < symbols.size(); i++)
  {
    if(!symbols[i].glob)continue;
    count++;
    out = symbols[i].name.size();
    binaryFile.write(reinterpret_cast<char*>(&out), sizeof(out));
    binaryFile << symbols[i].name.c_str();
    out = symbols[i].rel;
    binaryFile.write(reinterpret_cast<char*>(&out), sizeof(out));
    out = ((symbols[i].section == -1)?0:symbols[i].section);
    binaryFile.write(reinterpret_cast<char*>(&out), sizeof(out));
    out = symbols[i].defined;
    binaryFile.write(reinterpret_cast<char*>(&out), sizeof(out));
    out = symbols[i].equ;
    binaryFile.write(reinterpret_cast<char*>(&out), sizeof(out));
  }
  out = binaryFile.tellp();
  binaryFile.seekp(get_back);
  binaryFile.write(reinterpret_cast<char*>(&count), sizeof(count));
  binaryFile.seekp(out);
  binaryFile.flush();
};

void SymbolTable::WriteSectionCode(int section, std::fstream& binaryFile, std::ofstream& txtFile){
  txtFile << "----------Kod sekcija broj " << section << "-------------" << std::endl;
  char hex[17] = "0123456789abcdef";
  unsigned char read;
  binaryFile.seekg(sections[section].startPosition);
  for (int i = 0; i < sections[section].size; i++)
  {
    if(i % 8 == 0){
      if(i != 0) txtFile << '\n';
      char hex[4];
      sprintf(hex, "%x", i);
      txtFile << hex << ':';
    }
    binaryFile >> read;
    txtFile << hex[read >> 4];
    txtFile << hex[read & 15];
    txtFile << ' ';
  }
  txtFile << "\n";
  
}

void SymbolTable::readSections(std::ifstream& stream, int fileId, int section_count){
  std::string name;
  int startPosition, size, name_size;
  for (int i = 0; i < section_count; i++)
  {
    stream.read((char*)(&name_size), sizeof(name_size));
    char* raw_string = (char*)malloc(name_size + 1);
    stream.read(raw_string, name_size);
    raw_string[name_size] = '\0';
    name = std::string(raw_string);
    stream.read((char*)(&startPosition), sizeof(startPosition));
    stream.read((char*)(&size), sizeof(size));
    SectionRecord section = SectionRecord(name, startPosition, size, fileId);
    auto insertPos = sections.end();
    for (auto i = sections.begin(); i != sections.end(); i++)
    {
      if(i->name == name){
        insertPos = i;
      }
      else if(insertPos != sections.end()){
        break;
      }
    }
    if(insertPos != sections.end())insertPos++;
    sections.insert(insertPos, section);
  }
}

void SymbolTable::readSymbols(std::ifstream& stream, int id){
  int symbol_count;
  std::string name;
  int rel, section, name_size, defined, equ;
  stream.read((char*)(&symbol_count), sizeof(symbol_count));
  for (int i = 0; i < symbol_count; i++)
  {
    stream.read((char*)(&name_size), sizeof(name_size));
    char* raw_string = (char*)malloc(name_size + 1);
    stream.read(raw_string, name_size);
    raw_string[name_size] = '\0';
    name = std::string(raw_string);
    stream.read((char*)(&rel), sizeof(rel));
    stream.read((char*)(&section), sizeof(section));
    stream.read((char*)(&defined), sizeof(defined));
    stream.read((char*)(&equ), sizeof(equ));
    section = findSectionByFile(section, id);
    if(!defined)continue;
    if(findSymbol(name) != -1){
      std::cout << "Error: Multiple global variable " << std::endl;
      exit(-10);
    }
    SymbolRecord s(true, name);
    s.defined = true;
    s.rel = rel;
    s.section = section;
    s.equ = equ;
    symbols.push_back(s);
  }
}

void SymbolTable::ResolveRelocations(std::unordered_map<int, unsigned char>& hex){
  for (int i = 0; i < symbols.size(); i++)
    BackPatching::getInstance()->ResolveSymbol(hex, symbols[i].name, -1,
      symbols[i].rel + ((symbols[i].equ)?0:getSectionsStart(symbols[i].section)), true);
  for (int i = 0; i < sections.size(); i++)
    BackPatching::getInstance()->ResolveSymbol(hex, sections[i].name, sections[i].fileId, getSectionsStart(i), false);
}

void SymbolTable::writeHex(std::unordered_map<int, unsigned char>& hex, std::unordered_map<std::string, int>& place, std::vector<std::ifstream>& linkedFiles){
  int pos = 0;
  char tmp;
  int cur;
  for (int i = 0; i < sections.size(); i++)
  {
    if(place.find(sections[i].name) != place.end()){
      pos = place[sections[i].name];
      place.erase(place.find(sections[i].name));
    }
    cur = linkedFiles[sections[i].fileId].tellg();
    linkedFiles[sections[i].fileId].seekg(sections[i].startPosition);
    for (int j = 0; j < sections[i].size; j++)
    {
      if(hex.find(pos) != hex.end()){
        std::cout << "Impossible location for -place" << std::endl;
        exit(-11);
      }
      linkedFiles[sections[i].fileId].read(&tmp, sizeof(tmp));
      hex[pos++] = tmp;
    }
    linkedFiles[sections[i].fileId].seekg(cur);
    sections[i].startPosition = pos - sections[i].size;
  }
  
}

void SymbolTable::writeCode(std::fstream& binaryFile, std::vector<std::ifstream>& linkedFiles){
  char tmp;
  int cur;
  int pos;
  for (int i = 0; i < sections.size(); i++)
  {
    cur = linkedFiles[sections[i].fileId].tellg();
    linkedFiles[sections[i].fileId].seekg(sections[i].startPosition);
    pos = binaryFile.tellg();
    for (int j = 0; j < sections[i].size; j++)
    {
      linkedFiles[sections[i].fileId].read(&tmp, sizeof(tmp));
      binaryFile.write(&tmp, sizeof(tmp));
    }
    linkedFiles[sections[i].fileId].seekg(cur);
    sections[i].startPosition = pos;
  }
}

int SymbolTable::findSectionByFile(int section, int id){
  for (int i = 0; i < sections.size(); i++)
  {
    if(sections[i].fileId == id)section--;
    if(section == -1)return i;
  }
  return -1;
}