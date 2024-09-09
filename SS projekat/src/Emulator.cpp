#include "../inc/Emulator.hpp"

int main(int argv, char* argc[]){
  if(argv != 2){
    std::cout << "Bad parametar syntax." << std::endl;
    return -1;
  }
  std::ifstream in(argc[1]);
  if(!in.is_open()){
    std::cout << "Error: File has not been opened." << std::endl;
    return -1;
  }
  Emulator e(in);
  std::thread terminal(&Emulator::Terminal, &e);
  std::thread timer(&Emulator::Timer, &e);
  e.Emulate();
  e.Print();
  terminal.join();
  timer.join();
  return 0;
}

Emulator::Emulator(std::ifstream& hex){
  for(int& r: reg)r = 0;
  reg[15] = 0x40000000;
  csr[0] = csr[1] = csr[2] = 0;
  end = false;
  std::string line;
  int separator;
  char* adresa;
  char value[5];
  while(getline(hex, line)){
    separator =  line.find(':');
    char* adresa = (char*)malloc(separator + 3);
    adresa[0] = value[0] = '0';
    adresa[1] = value[1] = 'x';
    line.copy(adresa + 2, separator - 1, 0);
    line.copy(value + 2, 2, separator + 2);
    memory[(unsigned int)strtol(adresa, NULL, 0)] = (unsigned int)strtol(value, NULL, 0);
  }
}

void Emulator::Emulate(){
  unsigned int instruction;
  int disp, temp;
  unsigned short code, regA, regB, regC;
  int i = 0;
  while (true)
  {
    instruction = readMemory(reg[15]);
    reg[15] += 4;
    code = (instruction & 0xFF000000) >> 24;
    regA = (instruction & 0x00F00000) >> 20;
    regB = (instruction & 0x000F0000) >> 16;
    regC = (instruction & 0x0000F000) >> 12;
    disp = instruction & 0x00000FFF;
    if(disp & 0x800)disp += 0xFFFFF000;
    switch (code)
    {
    case 0x00:
      end = true;
      return;
    case 0x10:
      irq = 0x4;
      break;
    case 0x20:
      reg[14] -= 4;
      writeMemory(reg[14], reg[15]);
      reg[15] = reg[regA] + reg[regB] + disp;
      break; 
    case 0x21:
      reg[14] -= 4;
      writeMemory(reg[14], reg[15]);
      reg[15] = readMemory(reg[regA] + reg[regB] + disp);
      break;
    case 0x30:
      reg[15] = reg[regA] + disp;
      break;
    case 0x31:
      if(reg[regB] == reg[regC])reg[15] = reg[regA] + disp;
      break;
    case 0x32:
      if(reg[regB] != reg[regC])reg[15] = reg[regA] + disp;
      break;
    case 0x33:
    if(reg[regB] > reg[regC])reg[15] = reg[regA] + disp;
      break;
    case 0x38:
      reg[15] = readMemory(reg[regA] + disp);
      break;
    case 0x39:
      if(reg[regB] == reg[regC])reg[15] = readMemory(reg[regA] + disp);
      break;
    case 0x3A:
      if(reg[regB] != reg[regC])reg[15] = readMemory(reg[regA] + disp);
      break;
    case 0x3B:
      if(reg[regB] > reg[regC])reg[15] = readMemory(reg[regA] + disp);
      break;
    case 0x40:
      temp = reg[regB];
      reg[regB] = reg[regC];
      reg[regC] = temp;
      break;
    case 0x50:
      reg[regA] = reg[regB] + reg[regC];
      break;
    case 0x51:
      reg[regA] = reg[regB] - reg[regC];
      break;
    case 0x52:
      reg[regA] = reg[regB] * reg[regC];
      break;
    case 0x53:
      reg[regA] = reg[regB] / reg[regC];
      break;
    case 0x60:
      reg[regA] = ~reg[regB];
      break;
    case 0x61:
      reg[regA] = reg[regB] & reg[regC];
      break;
    case 0x62:
      reg[regA] = reg[regB] | reg[regC];
      break;
    case 0x63:
      reg[regA] = reg[regB] ^ reg[regC];
      break;
    case 0x70:
      reg[regA] = reg[regB] << reg[regC];
      break;
    case 0x71:
      reg[regA] = reg[regB] >> reg[regC];
      break;
    case 0x80:
      writeMemory(reg[regA] + reg[regB] + disp, reg[regC]);
      break;
    case 0x82:
      writeMemory(readMemory(reg[regA] + reg[regB] + disp), reg[regC]);
      break;
    case 0x81:
      reg[regA] = reg[regA] + disp;
      writeMemory(reg[regA], reg[regC]);
      break;
    case 0x90:
      reg[regA] = csr[regB];
      break;
    case 0x91:
      reg[regA] = reg[regB] + disp;
      break;
    case 0x92:
      reg[regA] = readMemory(reg[regB] + reg[regC] + disp);
      break;
    case 0x93:
      reg[regA] = readMemory(reg[regB]);
      reg[regB] = reg[regB] + disp;
      break;
    case 0x94:
      csr[regA] = reg[regB];
      break;
    case 0x95:
      csr[regA] = csr[regB] | disp;
      break;
    case 0x96:
      csr[regA] = readMemory(reg[regB] + reg[regC] + disp);
      break;
    case 0x97:
      csr[regA] = readMemory(reg[regB]);
      reg[regB] = reg[regB] + disp;
      break;
    default:
      csr[2] = 1;
      end = true;
      exit(-12);
    }
    if(memory[0xFFFFFF00] != 0){
      std::cout << memory[0xFFFFFF00] << std::flush;
      memory[0xFFFFFF00] = 0;
    }
    reg[0] = 0;
    temp = irq & (~csr[0]);
    if(!temp)continue;
    reg[14] -= 4;
    writeMemory(reg[14], csr[0]);
    reg[14] -= 4;
    writeMemory(reg[14], reg[15]);
    if(temp & 0x4){
      csr[2] = 4;
      csr[0] = csr[0] | 0x7;
      irq &= 0x3;
    }
    else if(temp & 0x2){
      csr[2] = 3;
      csr[0] = csr[0] | 0x3;
      irq &= 0x5;
    }
    else{
      csr[2] = 2;
      csr[0] = csr[0] | 0x1;
      irq &= 0x6;
    }
    reg[15] = csr[1];
  }
}

void Emulator::Print()
{
  std::cout << "---------------------------------------" << std::endl;
  std::cout << "Emulated processor executed halt instruction" << std::endl;
  std::cout << "Emulated processor state:" << std::endl;
  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      std::cout << "r" << i * 4 + j << "=" << std::hex << reg[i * 4 + j] << "     ";
    }
    std::cout << std::endl;
  }
  std::cout << "status=" << std::hex << csr[0] << std::endl;
  std::cout << "handler=" << std::hex << csr[1] << std::endl;
  std::cout << "cause=" << std::hex << csr[2] << std::endl;
  std::cout << "irq=" << std::hex << irq << std::endl;
}

void Emulator::writeMemory(unsigned int address, unsigned int value){
  memory[address] = value & 0xFF;
  memory[address + 1] = (value & 0xFF00) >> 8;
  memory[address + 2] = (value & 0xFF0000) >> 16;
  memory[address + 3] = (value & 0xFF000000) >> 24;
}
unsigned int Emulator::readMemory(unsigned int address){
  unsigned int value = memory[address + 3] << 24;
  value += memory[address + 2] << 16;
  value += memory[address + 1] << 8;
  value += memory[address];
  return value;
}

void Emulator::Terminal(){
  char term_in;
  static struct termios oldt, newt;
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON);          
  tcsetattr( STDIN_FILENO, TCSANOW, &newt);
  while (!end)
  {
    term_in = getchar();
    writeMemory(0xFFFFFF04, (int)term_in);
    irq |= 0x2;
  }
  tcsetattr( STDIN_FILENO, TCSANOW, &oldt);
}

void Emulator::Timer(){
  double time[] = {0.5, 1, 1.5, 2, 5, 10, 30, 60};
  while(!end){
    unsigned int timeIndex = readMemory(0xFFFFFF10);
    if(timeIndex > 7){
      std::cout << "This number is not mapped for timer" << std::endl;
      exit(-51);
    }
    usleep(time[timeIndex] * 1000000);
    irq |= 0x1;
  }
}