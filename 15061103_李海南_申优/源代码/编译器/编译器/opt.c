#include "asm.h"
#include "err.h"
#include "Global_Var.h"
#include "gra.h"
#include "lex.h"
#include "midcode.h"
#include <string.h>
#include "opt.h"

struct Nodetab
{
	char name[100];
	int index;
}nodetab[100];
struct Nodelist
{
	char op[100];
	int is_leaf;
	int in_order;
	int pi;
	int left_index;
	int right_index;
	int parent_index[100];
}*nodelist[100] = { NULL };
int nodelistloc(int nl, char op[],int left, int right) {
	int i = 0;
	for (i = 1; i <= nl; i++) {
		if (nodelist[i] == NULL) {
			printf("´íÀ²");
		}
		else {
			if (strcmp(op, nodelist[i]->op) == 0 && left == nodelist[i]->left_index
				&& right == nodelist[i]->right_index) {
				return i;
			}
		}
	}
	return -1;
}
int nodetabnameloc(int nt,char name[]) {
	int i = 0;
	for (i = 0; i < nt; i++) {
		if (strcmp(name, nodetab[i].name) == 0) {
			return i;
		}
	}
	return nt;
}
void DAG() {
	struct Nodelist *head, *child1,*child2,*p;
	int left, right,first;
	char var3[100],temp[100];
	int mi,optmi = 0,nt,nl,i,j,k,mnt = 0;
	int order[100],od;
	for (mi = 0; mi < midcodeiter; mi++) {
		memset(nodetab, '\0', 100 * sizeof(struct Nodetab));
		for (i = 0; i < 100; i++) {
			if (nodelist[i] != NULL) {
				nodelist[i] = NULL;
				//free(nodelist[i]);
			}
		}
		nt = 0;
		nl = 0;
		od = 0;
		if (strcmp(midcodelist[mi].op, "=") == 0 || strcmp(midcodelist[mi].op, "minu") == 0 ||
			strcmp(midcodelist[mi].op, "plus") == 0 || strcmp(midcodelist[mi].op, "mul") == 0 ||
			strcmp(midcodelist[mi].op, "div") == 0) {
			while (strcmp(midcodelist[mi].op, "=") == 0 || strcmp(midcodelist[mi].op, "minu") == 0 ||
				strcmp(midcodelist[mi].op, "plus") == 0 || strcmp(midcodelist[mi].op, "mul") == 0 ||
				strcmp(midcodelist[mi].op, "div") == 0) {
				midtemp = midcodelist[mi];
				//z=x
				if (strcmp(midcodelist[mi].op, "=") == 0) {
					i = nodetabnameloc(nt, midtemp.var1);
					if (i == nt) {
						//		if (!isnum(midtemp.var1)) {
						child1 = (struct Nodelist*)malloc(sizeof(struct Nodelist));
						strcpy(child1->op, "");
						child1->pi = 0;
						child1->is_leaf = 1;
						child1->in_order = 0;
						child1->left_index = 0;
						child1->right_index = 0;
						memset(child1->parent_index,0,400);
						nodelist[++nl] = child1;
						nodetab[nt].index = nl;
						strcpy(nodetab[nt].name, midtemp.var1);
						left = nl;
						nt++;
						//		}
					}
					else {
						left = nodetab[i].index;
						child1 = nodelist[left];
					}
					k = nodetabnameloc(nt, midtemp.var3);
					if (k == nt) {
						strcpy(nodetab[nt].name, midtemp.var3);
						nodetab[nt].index = left;
						nt++;
					}
					else {
						if (nodelist[nodetab[k].index]->is_leaf == 1 && !isnum(nodetab[k].name) && nodetab[k].index!=left) {
							strcpy(midcodeopt[optmi].var1, nodetab[k].name);
							strcpy(temp, nodetab[k].name);
							strcpy(nodetab[k].name, "$");
							strcat(nodetab[k].name, temp);
							strcpy(midcodeopt[optmi].op, "=");
							strcpy(midcodeopt[optmi].var3, nodetab[k].name);
							fprintf(mid_opt_file, "%s\t%s\t%s\t%s\n", midcodeopt[optmi].op, midcodeopt[optmi].var1, midcodeopt[optmi].var2, midcodeopt[optmi].var3);

							optmi++;
							strcpy(nodetab[nt].name, midtemp.var3);
							nodetab[nt].index = left;
							nt++;
						}
						else {
							nodetab[k].index = left;
						}
					}
				}
				//z=x op y
				if (strcmp(midcodelist[mi].op, "minu") == 0 ||
					strcmp(midcodelist[mi].op, "plus") == 0 || strcmp(midcodelist[mi].op, "mul") == 0 ||
					strcmp(midcodelist[mi].op, "div") == 0)
				{
					i = nodetabnameloc(nt, midtemp.var1);
					if (i == nt) {
						//		if (!isnum(midtemp.var1)) {
						child1 = (struct Nodelist*)malloc(sizeof(struct Nodelist));
						strcpy(child1->op, "");
						child1->in_order = 0;
						child1->pi = 0;
						child1->is_leaf = 1;
						child1->left_index = 0;
						child1->right_index = 0;
						memset(child1->parent_index,'\0',100);
						nodelist[++nl] = child1;
						nodetab[nt].index = nl;
						strcpy(nodetab[nt].name, midtemp.var1);
						left = nl;
						nt++;
						//		}
					}
					else {
						left = nodetab[i].index;
						child1 = nodelist[left];
					}
					j = nodetabnameloc(nt, midtemp.var2);
					if (j == nt) {
						child2 = (struct Nodelist*)malloc(sizeof(struct Nodelist));
						strcpy(child2->op, "");
						child2->in_order = 0;
						child2->pi = 0;
						child2->is_leaf = 1;
						child2->left_index = 0;
						child2->right_index = 0;
						memset(child2->parent_index,'\0',100);
						nodelist[++nl] = child2;
						nodetab[nt].index = nl;
						strcpy(nodetab[nt].name, midtemp.var2);
						right = nl;
						nt++;
					}
					else {
						right = nodetab[j].index;
						child2 = nodelist[right];
					}
					k = nodelistloc(nl, midtemp.op, left, right);
					if (k == -1) {
						p = (struct Nodelist*)malloc(sizeof(struct Nodelist));
						strcpy(p->op, midtemp.op);
						p->in_order = 0;
						p->pi = 0;
						p->is_leaf = 0;
						p->left_index = left;
						p->right_index = right;
						memset(p->parent_index,'\0',100);
						nodelist[++nl] = p;
						child1->parent_index[child1->pi++] = nl;
						child2->parent_index[child2->pi++] = nl;
						k = nl;
					}
					i = nodetabnameloc(nt, midtemp.var3);
					if (i == nt) {
						strcpy(nodetab[nt].name, midtemp.var3);
						nodetab[nt].index = k;
						nt++;
					}
					else {
						if (nodelist[nodetab[i].index]->is_leaf == 1 && !isnum(nodetab[i].name)) {
							strcpy(midcodeopt[optmi].var1, nodetab[i].name);
							strcpy(temp, nodetab[i].name);
							strcpy(nodetab[i].name, "$");
							strcat(nodetab[i].name, temp);
							strcpy(midcodeopt[optmi].op, "=");
							strcpy(midcodeopt[optmi].var3, nodetab[i].name);
							fprintf(mid_opt_file, "%s\t%s\t%s\t%s\n", midcodeopt[optmi].op, midcodeopt[optmi].var1, midcodeopt[optmi].var2, midcodeopt[optmi].var3);

							optmi++;
							strcpy(nodetab[nt].name, midtemp.var3);
							nodetab[nt].index = k;
							nt++;
						}
						else {
							nodetab[i].index = k;
						}
					}
				}
				mi++;
			}
			mi--;
			for (i = 1; i <= nl; i++) {
				if (nodelist[i]->is_leaf == 1) {
					nodetabindexloc(nt, i);
					if (tn > 1) {
						for (j = 1; j < tn; j++) {
							strcpy(midcodeopt[optmi].op, "=");
							strcpy(midcodeopt[optmi].var1, nodetab[tabnum[0]].name);
							strcpy(midcodeopt[optmi].var3, nodetab[tabnum[j]].name);
							fprintf(mid_opt_file, "%s\t%s\t%s\t%s\n", midcodeopt[optmi].op, midcodeopt[optmi].var1, midcodeopt[optmi].var2, midcodeopt[optmi].var3);
							optmi++;
						}
					}
				}
			}
			while (1) {
				for (i = 1; i <= nl; i++) {
					if (can_in(i)) {
						order[od++] = i;
						nodelist[i]->in_order = 1;
						j = nodelist[i]->left_index;
						while (j != 0)
						{
							if (can_in(j)) {
								order[od++] = j;
								nodelist[j]->in_order = 1;
								
							}
							j = nodelist[j]->left_index;
						}
					}
				}
				if (all_in(nl)) {
					break;
				}
			}
			while (od > 0) {
				od--;
				strcpy(midcodeopt[optmi].op, nodelist[order[od]]->op);
				left = nodelist[order[od]]->left_index;
				right = nodelist[order[od]]->right_index;
				nodetabindexloc(nt, left);
				for (i = 0; i < tn; i++) {
					if (nodetab[tabnum[i]].name[0] != '$') {
						strcpy(midcodeopt[optmi].var1, nodetab[tabnum[i]].name);
						break;
					}
				}
				if (tn != 0) {
					if (i == tn) {
						strcpy(midcodeopt[optmi].var1, nodetab[tabnum[0]].name);
					}
				}
				else {
					strcpy(midcodeopt[optmi].var1, "");
				}
				nodetabindexloc(nt, right);
				for (i = 0; i < tn; i++) {
					if (nodetab[tabnum[i]].name[0] != '$') {
						strcpy(midcodeopt[optmi].var2, nodetab[tabnum[i]].name);
						break;
					}
				}
				if (tn != 0) {
					if (i == tn) {
						strcpy(midcodeopt[optmi].var2, nodetab[tabnum[0]].name);
					}
				}
				else {
					strcpy(midcodeopt[optmi].var2, "");
				}
				nodetabindexloc(nt, order[od]);
				first = 1;
				for (i = 0; i < tn; i++) {
					if (nodetab[tabnum[i]].name[0] != '$') {
						if (first) {
							strcpy(midcodeopt[optmi].var3, nodetab[tabnum[i]].name);
							strcpy(var3, nodetab[tabnum[i]].name);
							fprintf(mid_opt_file, "%s\t%s\t%s\t%s\n", midcodeopt[optmi].op, midcodeopt[optmi].var1, midcodeopt[optmi].var2, midcodeopt[optmi].var3);
							optmi++;
							first = 0;
						}
						else {
							strcpy(midcodeopt[optmi].op, "=");
							strcpy(midcodeopt[optmi].var1, var3);
							strcpy(midcodeopt[optmi].var3, nodetab[tabnum[i]].name);
							fprintf(mid_opt_file, "%s\t%s\t%s\t%s\n", midcodeopt[optmi].op, midcodeopt[optmi].var1, midcodeopt[optmi].var2, midcodeopt[optmi].var3);
							optmi++;
						}
					}
				}
				if (first) {
					strcpy(midcodeopt[optmi].var3, nodetab[tabnum[0]].name);
					fprintf(mid_opt_file, "%s\t%s\t%s\t%s\n", midcodeopt[optmi].op, midcodeopt[optmi].var1, midcodeopt[optmi].var2, midcodeopt[optmi].var3);
					optmi++;
				}
			}

		}
		else {
			midcodeopt[optmi] = midcodelist[mi];
			fprintf(mid_opt_file, "%s\t%s\t%s\t%s\n", midcodeopt[optmi].op, midcodeopt[optmi].var1, midcodeopt[optmi].var2, midcodeopt[optmi].var3);
			optmi++;
		}
	}
	memset(midcodelist, '\0', 1000 * sizeof(struct Midcode));
	for (midcodeiter = 0; midcodeiter < optmi; midcodeiter++) {
		midcodelist[midcodeiter] = midcodeopt[midcodeiter];
	}
}
void nodetabindexloc(int nt,int index) {
	int i;
	tn = 0;
	for (i = 0; i < nt; i++) {
		if (nodetab[i].index == index) {
			tabnum[tn++] = i;
		}
	}
}
int can_in(int x) {
	int i, p;
	if (nodelist[x]->is_leaf == 0) {
		if (nodelist[x]->in_order == 1) {
			return 0;
		}
		if (nodelist[x]->pi == 0) {
			return 1;
		}
		for (i = 0; i < nodelist[x]->pi; i++) {
			p = nodelist[x]->parent_index[i];
			if (nodelist[p]->in_order != 1) {
				return 0;
			}
		}
		return 1;
	}
	return 0;
}
int all_in(int nl) {
	int i;
	for (i = 1; i <= nl; i++) {
		if (nodelist[i]->is_leaf == 0 && nodelist[i]->in_order == 0) {
			return 0;
		}
	}
	return 1;
}