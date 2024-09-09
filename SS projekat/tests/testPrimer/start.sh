./as.out -o handler.o tests/testPrimer/handler.s
./as.out -o main.o tests/testPrimer/main.s
./linker.out -hex \
 -place=my_code_main@0x40000000 \
 -place=my_code_handler@0xC0000000 \
 -o program.hex handler.o main.o
./emulator.out program.hex