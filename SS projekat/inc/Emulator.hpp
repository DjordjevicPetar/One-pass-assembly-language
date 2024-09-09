#include <unordered_map>
#include <fstream>
#include <iostream>
#include <thread>
#include <termios.h>
#include <unistd.h>
class Emulator{
private:
  std::unordered_map<unsigned int, unsigned char> memory;
  int reg[16];
  int csr[3];
  bool end;
  unsigned short irq;
  void writeMemory(unsigned int, unsigned int);
  unsigned int readMemory(unsigned int);
public:
  Emulator(std::ifstream& hex);
  void Emulate();
  void Print();
  void Terminal();
  void Timer();
};