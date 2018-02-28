#pragma once
#ifndef opth
#define opth

extern int nodelistloc(int nl, char op[], int left, int right);

extern int nodetabnameloc(int nt, char name[]);

extern void DAG();

extern void nodetabindexloc(int nt, int index);

extern int can_in(int x);

extern int all_in(int nl);

#endif // !opth


