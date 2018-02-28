#pragma once
#include <stdlib.h>
#include <stdio.h>

#ifndef g_var
#define g_var
char line[1000];
char ch;
char id[100];
char idl[100];
char nowvar[200];
FILE *in, *out, *lex_result, *gra_result, *sem_result,*midcode,*aim_code,*mid_opt_file;
int facerrflag;
int linenum;
int ll;
int cc;
int lc;
int sx;
int sleng;
int tn;
int tabnum[100];
int t, a, f,sign,dx,prt,prb;
long inum;
int errpos;
int skipflag;
int retflag;
int funtypflag;
#define nkw 14
#define alng 100
#define llng 121 
#define emax 322
#define emin -292 
#define kmax 10
#define tmax 10000
#define bmax 30
#define amax 30
#define c2max 20
#define csmax 30
#define cmax 800
#define lmax 7
#define smax 60000
#define ermax 58
#define omax 63
#define xmax 2147483647
#define nmax 2147483647
#define lineleng 132
#define linelimit 200
#define stacksize 1450
#define Maxcode 1000
int midcodeiter ;
int labelcnt ;
int varcnt ;
struct Midcode
{
	char op[100] ;
	char var1[100];
	char var2[100];
	char var3[100];
}midcodelist[Maxcode],midcodeopt[Maxcode],midtemp,pushlist[Maxcode];

enum Symbol {
	intsy,charsy,
	intcon, charcon, stringcon,
	plus, minus, times, idiv,
	eql, neq, gtr, geq, lss, leq,
	lparent, rparent, lbrack, rbrack, lbrace, rbrace, comma, semicolon,
	colon, becomes, constsy, voidsy, mainsy,
	ident, ifsy, switchsy, casesy, whilesy,
	dosy, printsy, returnsy, scanfsy
}sy;
enum objecttyp { constant, variable, typel, funktion };
enum types { notyp, ints, chars, arrays };
char stab[smax];
int display[lmax];
struct Tab
{
	char name[100];
	int paranum;
	enum objecttyp obj;
	enum types type;
	int ref;
	int normal;
	int adr;
	int link;
};
struct Tab tab[tmax];
struct Ref_Tab
{
	char name[100];
	int num;
};
struct funtab
{
	char name[100];
	int paranum;
	int offset;
	int rt;
	struct Ref_Tab ref_tab[100];
	int ref;
	int size;
	enum types type;
}ftab[bmax];
struct ATab
{
	enum types eltype;
	int elref, low, high, elsize, size;
}atab[amax];
enum Class{ INT,CHAR}cla;
char sym[15];
char key[nkw][20]; //保留字集合
enum Symbol ksy[nkw]; //保留字对应的sym集合}
enum Symbol sps[200]; //special symbols } {index类型为char}
#endif // !g_var

