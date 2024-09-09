Parser = misc/parser.ypp
Lexer = misc/lexer.l
Create_FLAGS = -Iinc -I. -Ifl

SOURCES = $(wildcard *.cpp) $(wildcard src/Operands/*cpp) $(wildcard src/Rel/*cpp) $(wildcard *.c) $(wildcard src/Instructions/*cpp)
SRC_LINKER = $(wildcard src/Rel/*cpp)

parser.tab.hpp parser.tab.cpp: $(Parser)
	bison -d -t $(Parser)

lex.yy.c: $(Lexer) parser.tab.hpp
	flex $(Lexer)

create: parser.tab.hpp parser.tab.cpp lex.yy.c
	g++ ${Create_FLAGS} ${SOURCES} src/Asembler.cpp -o as.out
	g++ ${Create_FLAGS} ${SRC_LINKER} src/Linker.cpp -o linker.out
	g++ -pthread src/Emulator.cpp -o emulator.out

clean:
	rm -f as.out
	rm -f linker.out
	rm -f parser.tab.cpp parser.tab.hpp
	rm -f lex.yy.c