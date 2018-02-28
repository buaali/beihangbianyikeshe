#pragma once
#include "Global_Var.h"
#ifndef gra
#define gra


extern int loc(char idl[]);
extern void testsemicolon();
extern enum types testi_ch();
extern void enter(char id[], enum objecttyp objtyp, enum types type, int isfun);

extern void entervariable(char temp[], enum types type);




extern void enter_arr_variable(char temp[],enum types type);

extern int enterconst(enum types type);

extern void enterarray(enum types type, long size);

extern void enterid(char name[], enum objecttyp objtyp, enum types type, int adr);

extern void enterids();

extern void parameterlist(enum types type);

extern void constdec(enum types type);



extern void v_a_fdec(char temp[], enum types type);



extern void statement();

extern void returnstatement();

extern void printstatement();

extern void scanfstatement();

extern void linestatement();

extern void switchstatement();

extern void fact_sheet(char var[], char label[]);



extern void dowhilestatement();

extern void factor();

extern void term();

extern void simpleexpression();



extern void assignment(struct Tab tabi);

extern void selector(struct Tab tabi);


extern void codition();

extern void ifstatement();

extern void block();

#endif // !gra


