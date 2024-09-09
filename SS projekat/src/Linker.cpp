#include "../inc/Linker.hpp"

Linker* Linker::_singleton = nullptr;

Linker* Linker::getInstance(){
  if(_singleton == nullptr){
    _singleton = new Linker();
  }
  return _singleton;
}

Linker::Linker(){
  hex = false;
  relocatable = false;
  linkedFiles = std::vector<std::ifstream>();
}

int main(int argv, char* argc[]){
  Linker::getInstance()->Init(argv, argc);
  Linker::getInstance()->main();
  return 0;
}

int Linker::Init(int argv, char* argc[]){
  for (int i = 1; i < argv; i++){
    if(!strcmp(argc[i], "-hex"))hex = true;
    else if(!strcmp(argc[i], "-relocatable"))relocatable = true;
    else if(!strcmp(argc[i], "-o")){
      if(outFile.is_open()){
        std::cout << "Output file already given." << std::endl;
        return -4;
      }
      i++;
      if(hex)outFile = std::fstream(argc[i], std::fstream::out);
      else if(relocatable)outFile = std::fstream(argc[i], std::fstream::binary | std::fstream::out);
      if(!outFile.is_open()){
        std::cout << "Output file cannot be opened." << std::endl;
        return -3;
      }
    }
    else if(strncmp(argc[i], "-place=", 7) == 0){
      int pos = strchr(argc[i], '@') - argc[i];
      char* sekcija = (char*)malloc(pos - 6);
      char* adresa = (char*)malloc(strlen(argc[i]) - pos);
      strncpy(sekcija, argc[i] + 7, pos - 7);
      strcpy(adresa, argc[i] + pos + 1);
      if(pos < 0 || strlen(sekcija) == 0 || strlen(adresa) == 0){
        std::cout << "Wrong syntax of -place option." << std::endl;
        return -5;
      }
      place[sekcija] = (int)strtol(adresa, NULL, 0);
    }
    else{
      for (int j = i; j < argv; j++)
      {
        linkedFiles.push_back(std::ifstream(argc[j]));
        if(!linkedFiles[linkedFiles.size() - 1].is_open()){
          std::cout << "File that is tried to be linked, by the name of " << argc[j] << " does not exist." << std::endl;
          return -6;
        }
      }
      break;
    }
  }

  if(!hex && !relocatable){
    std::cout << "Either hex or relocatable must be selected as an option." << std::endl;
    return -1;
  }

  if(hex && relocatable){
    std::cout << "You cannot select both hex and relocatable options in a single command." << std::endl;
    return -2;
  }

  if(!outFile.is_open()){
    if(hex)outFile = std::fstream("ln.o", std::fstream::binary | std::fstream::out);
    else outFile = std::fstream("ln.hex", std::fstream::binary | std::fstream::out);
    if(!outFile.is_open()){
      std::cout << "Output file cannot be opened." << std::endl;
      return -3;
    }
  }
  return 0;
}

void Linker::main(){
  readRelevantInfo();
  if(hex)Hex();
  else Rel();
}

void Linker::Hex(){
  SymbolTable::getInstance()->writeHex(hexOut, place, linkedFiles);
  SymbolTable::getInstance()->ResolveRelocations(hexOut);
  char hexDigits[17] = "0123456789ABCDEF";
  char code[9];
  for (auto it: hexOut)
  {
    sprintf(code, "%X", it.first);
    outFile << code << " : " << hexDigits[(it.second & 0xF0) >> 0x4] << hexDigits[it.second & 0xF] << std::endl;
  }
}

void Linker::Rel(){
  int out = 0;
  outFile.write((char*)(&out), sizeof(out));
  SymbolTable::getInstance()->writeCode(outFile, linkedFiles);
  int cur = outFile.tellp();
  outFile.seekp(0);
  outFile.write(reinterpret_cast<char*>(&cur), sizeof(cur));
  outFile.seekp(cur);
  SymbolTable::getInstance()->WriteBinary(outFile);
}

void Linker::readRelevantInfo(){
  //readSections
  int pos;
  int section_count;
  std::vector<int> sectionCounts = std::vector<int>();
  for (int i = 0; i < linkedFiles.size(); i++)
  {
    linkedFiles[i].read((char*)(&pos), sizeof(pos));
    linkedFiles[i].seekg(pos);
    linkedFiles[i].read((char*)(&section_count), sizeof(section_count));
    sectionCounts.push_back(section_count);
    SymbolTable::getInstance()->readSections(linkedFiles[i], i, section_count);
  }
  
  //readRelocations
  for (int i = 0; i < linkedFiles.size(); i++)
    BackPatching::getInstance()->readRelocations(linkedFiles[i], i, sectionCounts[i]);

  //readSymbols
  for (int i = 0; i < linkedFiles.size(); i++)
    SymbolTable::getInstance()->readSymbols(linkedFiles[i], i);
}