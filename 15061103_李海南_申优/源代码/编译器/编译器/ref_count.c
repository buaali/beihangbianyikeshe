#include "Global_Var.h"
#include "asm.h"
#include "err.h"
#include "ref_count.h"
#include <string.h>
int refloc(int j,char name[]) {
	int i;
	for (i = 0; i < ftab[j].rt && i<8; i++) {
		if (strcmp(ftab[j].ref_tab[i].name, name)==0) {
			return i;
		}
	}
	return -1;
}
void enter_ref(int j,char name[]) {
	int i= 0;
	for (i = 0; i < ftab[j].rt; i++) {
		if (strcmp(ftab[j].ref_tab[i].name, name)==0) {
			ftab[j].ref_tab[i].num++;
			return;
		}
	}
	if (name[0] == '$') {
		strcpy(ftab[j].ref_tab[ftab[j].rt].name, name);
		ftab[j].ref_tab[ftab[j].rt++].num = 1;
	}
}
void ref_count() {
	int i,j,k;
	for (i = 0; i < midcodeiter; i++) {
		if (strcmp(midcodelist[i].op, "func") != 0) {
			continue;
		}
		j = funcloc(midcodelist[i].var3);
		if (j != -1) {
			if (j != f - 1) {
				for (k = ftab[j].ref + 1 + ftab[j].paranum; k < ftab[j + 1].ref; k++) {
					if (tab[k].type != arrays) {
						strcpy(ftab[j].ref_tab[ftab[j].rt].name, tab[k].name);
						ftab[j].ref_tab[ftab[j].rt++].num = 1;
					}
				}
			}
			else {
				for (k = ftab[j].ref + 1 + ftab[j].paranum; k < t; k++) {
					if (tab[k].type != arrays) {
						strcpy(ftab[j].ref_tab[ftab[j].rt].name, tab[k].name);
						ftab[j].ref_tab[ftab[j].rt++].num = 1;
					}
				}
			}
		}
		else {
			err(0);
		}
		while (strcmp(midcodelist[i].op, "end") != 0) {
			enter_ref(j, midcodelist[i].var1);
			enter_ref(j, midcodelist[i].var2);
			enter_ref(j, midcodelist[i].var3);
			i++;
		}
		Sort(j);
	}
}
void Sort(int fun) {
	int i, j;
	struct Ref_Tab temp;
	for (i = 0; i < ftab[fun].rt; i++) {
		for (j = i; j < ftab[fun].rt; j++) {
			if (ftab[fun].ref_tab[j].num > ftab[fun].ref_tab[i].num) {
				temp = ftab[fun].ref_tab[i];
				ftab[fun].ref_tab[i] = ftab[fun].ref_tab[j];
				ftab[fun].ref_tab[j] = temp;
			}
		}
	}
}
