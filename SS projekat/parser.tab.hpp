/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef YY_YY_PARSER_TAB_HPP_INCLUDED
# define YY_YY_PARSER_TAB_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    NUMBER = 258,
    STRING = 259,
    REGISTER = 260,
    STATUS = 261,
    HANDLER = 262,
    CAUSE = 263,
    LABEL = 264,
    END_DIR = 265,
    GLOBAL_DIR = 266,
    EXTERN_DIR = 267,
    SECTION_DIR = 268,
    WORD_DIR = 269,
    SKIP_DIR = 270,
    ASCII_DIR = 271,
    EQU_DIR = 272,
    HALT = 273,
    INT = 274,
    IRET = 275,
    CALL = 276,
    RET = 277,
    JMP = 278,
    BEQ = 279,
    BNE = 280,
    BGT = 281,
    PUSH = 282,
    POP = 283,
    XCHG = 284,
    ADD = 285,
    SUB = 286,
    MUL = 287,
    DIV = 288,
    NOT = 289,
    AND = 290,
    OR = 291,
    XOR = 292,
    SHL = 293,
    SHR = 294,
    LD = 295,
    ST = 296,
    CSRRD = 297,
    CSRWR = 298,
    SYMBOL = 299,
    DOLLAR = 300,
    PERCENT = 301,
    LEFT_BRACE = 302,
    RIGHT_BRACE = 303,
    PLUS = 304,
    MINUS = 305,
    COLON = 306,
    EOL = 307
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 25 "misc/parser.ypp"

  int num;
  char* str;
  void* symbolList;
  void* instruction;

#line 117 "parser.tab.hpp"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSER_TAB_HPP_INCLUDED  */
