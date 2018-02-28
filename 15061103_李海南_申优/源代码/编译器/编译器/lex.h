#pragma once
#include "Global_Var.h"

#ifndef lex
#define lex
extern void setup();
extern char h2l(char c);
extern void sytosym(enum Symbol sy, char sym[]);
extern void nextch();
extern void insymbol();
#endif // !lex

