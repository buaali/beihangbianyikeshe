#pragma once
#ifndef mid2asm
#define mid2asm
extern int isnum(char var[]);
extern int addrloc(char name[]);
extern int funcloc(char name[]);
extern void instack(char var[], int size);

extern void inaddr(int type, int tmi, int t, int glob);
extern void midcodetoasm();

extern void callop();

extern void GOTOop();

extern void BZop();


extern void labop();

extern void plusop();

extern void minuop();

extern void mulop();

extern void divop();


extern void grtop();

extern void geqop();

extern void lssop();

extern void leqop();

extern void eqlop();

extern void neqop();

extern void assop();

extern void sassop();

extern void getaop();

extern void scfop();

extern void prtop();

extern void fpaop();

extern void retop();

extern void paraop();

extern void funcop();
#endif // !mid2asm


