%{
  #include <iostream>
  #include "../parser.tab.hpp"
  #include <stdio.h>
  int line = 1;
%}

%option noyywrap

%%

#.* {} //Comment

["].*["] {
  yytext[strlen(yytext) - 1] = '\0';
  yylval.str = strdup(yytext + 1);
  return STRING;
}
[0-9]+ {
  yylval.num = atoi(yytext);
  return NUMBER;
}

0x[0-9a-fA-F]+ {
  yylval.num = (int)strtol(yytext, NULL, 0);
  return NUMBER;
}

\%r(1[0-5]|[0-9]) {
  yylval.num = atoi(yytext + 2);
  return REGISTER;
}
\%sp { 
  yylval.num = 14;
  return REGISTER;
}
\%pc {
  yylval.num = 15;
  return REGISTER;
}//Registers

\%status return STATUS;
\%handler return HANDLER;
\%cause return CAUSE;

[a-zA-Z_][a-zA-Z0-9_]*[:] {
  yytext[strlen(yytext) - 1] = '\0';
  yylval.str = yytext;
  return LABEL;
}

".end" return END_DIR;
".global" return GLOBAL_DIR;
".extern" return EXTERN_DIR;
".section" return SECTION_DIR;
".word" return WORD_DIR;
".skip" return SKIP_DIR;
".ascii" return ASCII_DIR;
".equ" return EQU_DIR;

halt return HALT;
int return INT;
iret return IRET;
call return CALL;
ret return RET;
jmp return JMP;
beq return BEQ;
bne return BNE;
bgt return BGT;
push return PUSH;
pop return POP;
xchg return XCHG;
add return ADD;
sub return SUB;
mul return MUL;
div return DIV;
not return NOT;
and return AND;
or return OR;
xor return XOR;
shl return SHL;
shr return SHR;
ld return LD;
st return ST;
csrrd return CSRRD;
csrwr return CSRWR;

[a-zA-Z_][a-zA-Z_0-9]* {
  yylval.str = strdup(yytext);
  return SYMBOL;
}

"$" return DOLLAR;
"%" return PERCENT;
"[" return LEFT_BRACE;
"]" return RIGHT_BRACE;
"+" return PLUS;
"-" return MINUS;
"," return COLON;
" " {}

\n {line++; return EOL;}

. {
  printf("Greska u liniji %d: %s", line, yytext);
  return END_DIR;
  }

%%