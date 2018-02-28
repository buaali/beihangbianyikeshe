#include <stdio.h>
#include "Global_Var.h"
#include "err.h"
#include "gra.h"
#include "lex.h"
#include <string.h>
#include "midcode.h"

void inmidcode(char op[],char v1[],char v2[],char v3[]) {
	strcpy(midcodelist[midcodeiter].op, op);
	strcpy(midcodelist[midcodeiter].var1, v1);
	strcpy(midcodelist[midcodeiter].var2, v2);
	strcpy(midcodelist[midcodeiter].var3, v3);
	midcodeiter++;
}
char * newlabel() {
	char label[20];
	sprintf(label, "LAB%d", labelcnt++);
	return label;
}
char * newvar() {
	char var[20];
	sprintf(var,"$%d",varcnt++);
	return var;
}
void printmidcode() {
	int i = 0;
	for (i = 0; i < midcodeiter; i++) {
		/*
		if (strcmp(midcodelist[i].op, "func") == 0) {
			fprintf(midcode, "%s\t%s()\n", midcodelist[i].var1, midcodelist[i].var3);
			continue;
		}
		if (strcmp(midcodelist[i].op, "call") == 0) {
			if (strcmp(midcodelist[i].var3, "") != 0) {
				fprintf(midcode, "%s\t%s\n%s=RET\n", midcodelist[i].op, midcodelist[i].var1, midcodelist[i].var3);
				continue;
			}
		}
		if (strcmp(midcodelist[i].op, "minu") == 0) {
			fprintf(midcode, "%s=%s-%s\n", midcodelist[i].var3, midcodelist[i].var1, midcodelist[i].var2);
			continue;
		}
		if (strcmp(midcodelist[i].op, "plus") == 0) {
			fprintf(midcode, "%s=%s+%s\n", midcodelist[i].var3, midcodelist[i].var1, midcodelist[i].var2);
			continue;
		}
		if (strcmp(midcodelist[i].op, "mul") == 0) {
			fprintf(midcode, "%s=%s*%s\n", midcodelist[i].var3, midcodelist[i].var1, midcodelist[i].var2);
			continue;
		}
		if (strcmp(midcodelist[i].op, "div") == 0) {
			fprintf(midcode, "%s=%s/%s\n", midcodelist[i].var3, midcodelist[i].var1, midcodelist[i].var2);
			continue;
		}
		if (strcmp(midcodelist[i].op, "[]=") == 0) {
			fprintf(midcode, "%s[%s]=%s\n", midcodelist[i].var3, midcodelist[i].var2, midcodelist[i].var1);
			continue;
		}
		if (strcmp(midcodelist[i].op, "=") == 0) {
			fprintf(midcode, "%s=%s\n", midcodelist[i].var3, midcodelist[i].var1);
			continue;
		}
		if (strcmp(midcodelist[i].op, "geta") == 0) {
			fprintf(midcode, "%s=%s[%s]\n", midcodelist[i].var3, midcodelist[i].var1, midcodelist[i].var2);
			continue;
		}
		if (strcmp(midcodelist[i].op, "<") == 0) {
			fprintf(midcode, "%s<%s\n", midcodelist[i].var1, midcodelist[i].var2);
			continue;
		}
		if (strcmp(midcodelist[i].op, "<=") == 0) {
			fprintf(midcode, "%s<=%s\n", midcodelist[i].var1, midcodelist[i].var2);
			continue;
		}
		if (strcmp(midcodelist[i].op, ">") == 0) {
			fprintf(midcode, "%s>%s\n", midcodelist[i].var1, midcodelist[i].var2);
			continue;
		}
		if (strcmp(midcodelist[i].op, ">=") == 0) {
			fprintf(midcode, "%s>=%s\n", midcodelist[i].var1, midcodelist[i].var2);
			continue;
		}
		if (strcmp(midcodelist[i].op, "==") == 0) {
			fprintf(midcode, "%s==%s\n", midcodelist[i].var1, midcodelist[i].var2);
			continue;
		}if (strcmp(midcodelist[i].op, "!=") == 0) {
			fprintf(midcode, "%s!=%s\n", midcodelist[i].var1, midcodelist[i].var2);
			continue;
		}
		*/
		fprintf(midcode, "%s\t%s\t%s\t%s\n", midcodelist[i].op, midcodelist[i].var1, midcodelist[i].var2, midcodelist[i].var3);
	}
}