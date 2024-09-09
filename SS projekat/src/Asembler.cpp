#include "../inc/Asembler.hpp"

Asembler* Asembler::_singleton = nullptr;

Asembler::Asembler(int argc, char* argv[]){
    if(argc == 2){
    in = fopen(argv[1], "r");
    outBin = std::fstream("as.o", std::fstream::in | std::fstream::out | std::fstream::binary | std::fstream::trunc);
    outTxt = std::ofstream("as.txt");
    if(in == NULL || !outBin.is_open() || !outTxt.is_open()){
      printf("This file is not present\n");
      exit(-1);
    }
  }
  else if(argc == 4 && strcmp(argv[1], "-o") == 0){
    in = fopen(argv[3], "r");
    outBin = std::fstream(argv[2], std::fstream::in | std::fstream::out | std::fstream::binary | std::fstream::trunc);
    outTxt = std::ofstream("as.txt");
    if(in == NULL || !outBin.is_open() || !outTxt.is_open()){
      printf("This file is not present\n");
      exit(-1);
    }
  }
  else {
    printf("Bad format\n");
    exit(-2);
  }
}

int Asembler::Init(int argc, char* argv[]){
  _singleton = new Asembler(argc, argv);

  yyin = _singleton->in;
  return 0;
}

Asembler* Asembler::getInstance(){
  return _singleton;
}

std::fstream& Asembler::getOutBin(){
  return outBin;
}

std::ofstream& Asembler::getOutTxt(){
  return outTxt;
}

int main(int argc, char* argv[]){
  Asembler::Init(argc, argv);
  int out = 0;
  Asembler::getInstance()->getOutBin().write(reinterpret_cast<char*>(&out), sizeof(out));
  Instruction::setOutput(Asembler::getInstance()->getOutBin());
  yyparse();
  int cur = Asembler::getInstance()->getOutBin().tellp();
  Asembler::getInstance()->getOutBin().seekp(0);
  Asembler::getInstance()->getOutBin().write(reinterpret_cast<char*>(&cur), sizeof(cur));
  Asembler::getInstance()->getOutBin().seekp(cur);
  SymbolTable::getInstance()->WriteTxt(Asembler::getInstance()->getOutTxt());
  SymbolTable::getInstance()->WriteBinary(Asembler::getInstance()->getOutBin());
  for(int i = 0; i < SymbolTable::getInstance()->getLastSection() + 1; i++){
    SymbolTable::getInstance()->WriteSectionCode(i, Asembler::getInstance()->getOutBin(), Asembler::getInstance()->getOutTxt());
  }
  Asembler::getInstance()->getOutBin().close();
  Asembler::getInstance()->getOutTxt().close();
}