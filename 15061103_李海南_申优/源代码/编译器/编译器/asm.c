#include "Global_Var.h"
#include "midcode.h"
#include <string.h>
#include "asm.h"
#include "ref_count.h"
#define VOID 0
#define INT 1
#define CHAR 2

int mi = 0;
int ismain = 0;
int offset = 4;
int funid;
char funcname[100];

struct Addr
{
	char name[100];
	int addr;
	int type;
	int isglo;
}addrlist[2000];
int ap=0;


int isnum(char var[]) {
	if ((var[0] >= '0' && var[0] <= '9') || var[0] == '-' || var[0] == '+') {
		return 1;
	}
	return 0;
}

int addrloc(char name[]) {
	int i;
	for (i = ap-1; i >=0; i--) {
		if (strcmp(addrlist[i].name, name) == 0) {
			return i;
		}
	}
	return -1;
}

int funcloc(char name[]) {
	int i;
	for (i = 0; i < f; i++) {
		if (strcmp(ftab[i].name, name) == 0) {
			return i;
		}
	}
	return -1;
}
void instack(char var[],int size) {
	if (size == 1) {
		fprintf(aim_code, "li $t0 %d\n", atoi(var));
		fprintf(aim_code, "sw $t0 0x%x($sp)\n",offset);
	}
	offset = offset + 4*size;
}

void inaddr(int type,int tmi,int t,int glob) {
	if (tmi == 1) {
		strcpy(addrlist[ap].name, midcodelist[mi].var3);
	}
	else {
		strcpy(addrlist[ap].name, midcodelist[tmi].var3);
	}

	addrlist[ap].addr = offset;
	addrlist[ap].isglo = glob;
	addrlist[ap].type = type;

	ap++;
}
void midcodetoasm() {
	int tmi,i,size;
	fprintf(aim_code, ".text\n");
	mi = ap = size = 0;
	tmi = mi;

	while (mi < midcodeiter)
	{
		while (strcmp(midcodelist[tmi].op, "const") == 0)
		{
			size++;
			tmi++;
		}
		while (strcmp(midcodelist[tmi].op, "int") == 0 ||
			strcmp(midcodelist[tmi].op, "char") == 0 ||
			strcmp(midcodelist[tmi].op, "ints") == 0 ||
			strcmp(midcodelist[tmi].op, "chars") == 0
			) {
			if (strcmp(midcodelist[tmi].var2, "") == 0) {
				size++;
			}
			else {
				size = size + atoi(midcodelist[tmi].var2);
			}
			tmi++;
		}
		fprintf(aim_code, "subi $sp $sp %d\n", 4 * size);
		fprintf(aim_code, "addi $t8 $sp 0\n");
		while (strcmp(midcodelist[mi].op,"const") == 0)
		{
			
			if (strcmp(midcodelist[mi].var1, "int") == 0 || strcmp(midcodelist[mi].var1, "ints") == 0) {
				inaddr(INT,1,0,1);
			}
			else {
				inaddr(CHAR,1,0,1);
			}
			instack(midcodelist[mi].var2, 1);
			mi++;
		}
		while (strcmp(midcodelist[mi].op,"int") == 0 ||
			strcmp(midcodelist[mi].op,"char") == 0 ||
			strcmp(midcodelist[mi].op, "ints") == 0 ||
			strcmp(midcodelist[mi].op, "chars") == 0){
			
			if (strcmp(midcodelist[mi].op, "int") == 0 || strcmp(midcodelist[mi].op, "ints")==0) {
				inaddr(INT,1,0,1);
			}
			else {
				inaddr(CHAR,1,0,1);
			}
			if (strcmp(midcodelist[mi].var2, "") == 0) {
				offset += 4;
			}
			else instack("0", atoi(midcodelist[mi].var2));
			mi++;
		}
		
		fprintf(aim_code,"j main\n");
		
		while (strcmp(midcodelist[mi].op,"func") == 0){
			fprintf(aim_code, "%s:",midcodelist[mi].var3);
			funid = funcloc(midcodelist[mi].var3);
			strcpy(funcname, midcodelist[mi].var3);
			if (strcmp(funcname, "main") == 0) {
				ismain = 1;
			}
			mi++;
			funcop();

		}
		
	}

}

void funcop() {
	int tmi,i,tap,n;
	offset = 4;
	tmi = mi;
	tap = ap;
	i = funcloc(funcname);
	fprintf(aim_code, "subi $sp $sp 40\nsw $ra 4($sp)\nsw $t9 8($sp)\n");
	fprintf(aim_code, "sw $s0 12($sp)\n");
	fprintf(aim_code, "sw $s1 16($sp)\n");
	fprintf(aim_code, "sw $s2 20($sp)\n");
	fprintf(aim_code, "sw $s3 24($sp)\n");
	fprintf(aim_code, "sw $s4 28($sp)\n");
	fprintf(aim_code, "sw $s5 32($sp)\n");
	fprintf(aim_code, "sw $s6 36($sp)\n");
	fprintf(aim_code, "sw $s7 40($sp)\n");
	fprintf(aim_code, "addi $t9 $sp 0\n");
	fprintf(aim_code, "subi $sp $sp %d\n", 4 * ftab[i].size);
	if (strcmp(midcodelist[mi].op, "para") == 0) paraop();
	while (strcmp(midcodelist[mi].op, "const") == 0)
	{

		if (strcmp(midcodelist[mi].var1, "int") == 0 || strcmp(midcodelist[mi].var1, "ints") == 0) {
			inaddr(INT, 1, 0, 0);
		}
		else {
			inaddr(CHAR, 1, 0, 0);
		}
		if ((n=refloc(funid, midcodelist[mi].var3)) == -1) {
			instack(midcodelist[mi].var2, 1);
		}
		else {
			offset += 4;
			fprintf(aim_code, "li $s%d %d\n",n, atoi(midcodelist[mi].var2));
		}
		mi++;
	}
	while (strcmp(midcodelist[mi].op, "int") == 0 ||
		strcmp(midcodelist[mi].op, "char") == 0 ||
		strcmp(midcodelist[mi].op, "ints") == 0 ||
		strcmp(midcodelist[mi].op, "chars") == 0) {

		if (strcmp(midcodelist[mi].op, "int") == 0 || strcmp(midcodelist[mi].op, "ints") == 0) {
			inaddr(INT, 1, 0, 0);
		}
		else {
			inaddr(CHAR, 1, 0, 0);
		}
		if (strcmp(midcodelist[mi].var2, "") == 0) {
			offset += 4;
		}
		else {
			instack("0", atoi(midcodelist[mi].var2));
		}
		mi++;
	}
	while (strcmp(midcodelist[tmi].op, "end") != 0) {
		
		if (midcodelist[tmi].var1[0] == '$' && addrloc(midcodelist[tmi].var1) == -1) {
			
			inaddr(INT, tmi,0,0);
			offset += 4;
		}
		if (midcodelist[tmi].var2[0] == '$' && addrloc(midcodelist[tmi].var2) == -1) {
			
			inaddr(INT,tmi,0,0);
			offset += 4;
		}
		if (midcodelist[tmi].var3[0] == '$' && addrloc(midcodelist[tmi].var3) == -1) {
			
			if (strcmp(midcodelist[tmi].op, "call") == 0) {
				inaddr(ftab[funcloc(midcodelist[tmi].var1)].type,tmi,0,0);
			}
			else if(strcmp(midcodelist[tmi].op, "geta") == 0){
				inaddr(addrlist[addrloc(midcodelist[tmi].var1)].type,tmi,0,0);
			}
			else {
				inaddr(INT, tmi,0,0);
			}
			offset += 4;
		}
		tmi++;
	}
	
	

	while (strcmp(midcodelist[mi].op, "end") != 0) {
		
		if (strcmp(midcodelist[mi].op, "plus") == 0) plusop();
		if (strcmp(midcodelist[mi].op, "minu") == 0) minuop();
		if (strcmp(midcodelist[mi].op, "mul") == 0) mulop();
		if (strcmp(midcodelist[mi].op, "div") == 0) divop();
		if (strcmp(midcodelist[mi].op, ">") == 0) grtop();
		if (strcmp(midcodelist[mi].op, ">=") == 0) geqop();
		if (strcmp(midcodelist[mi].op, "<") == 0) lssop();
		if (strcmp(midcodelist[mi].op, "<=") == 0) leqop();
		if (strcmp(midcodelist[mi].op, "!=") == 0) neqop();
		if (strcmp(midcodelist[mi].op, "==") == 0) eqlop();
		if (strcmp(midcodelist[mi].op, "=") == 0) assop();
		if (strcmp(midcodelist[mi].op, "[]=") == 0) sassop();
		if (strcmp(midcodelist[mi].op, "geta") == 0) getaop();
		if (strcmp(midcodelist[mi].op, "lab:") == 0) labop();
		if (strcmp(midcodelist[mi].op, "scf") == 0) scfop();
		if (strcmp(midcodelist[mi].op, "prt") == 0) prtop();
		if (strcmp(midcodelist[mi].op, "BZ") == 0) BZop();
		if (strcmp(midcodelist[mi].op, "GOTO") == 0) GOTOop();
		if (strcmp(midcodelist[mi].op, "PUSH") == 0) fpaop();
		if (strcmp(midcodelist[mi].op, "call") == 0) callop();
		if (strcmp(midcodelist[mi].op, "ret") == 0) retop();
		
		mi++;
	}
	fprintf(aim_code, "fend_%s:\n", funcname);
	fprintf(aim_code, "addi $sp $t9 0\n");
	fprintf(aim_code, "lw $s0 12($sp)\n");
	fprintf(aim_code, "lw $s1 16($sp)\n");
	fprintf(aim_code, "lw $s2 20($sp)\n");
	fprintf(aim_code, "lw $s3 24($sp)\n");
	fprintf(aim_code, "lw $s4 28($sp)\n");
	fprintf(aim_code, "lw $s5 32($sp)\n");
	fprintf(aim_code, "lw $s6 36($sp)\n");
	fprintf(aim_code, "lw $s7 40($sp)\n");
	fprintf(aim_code, "lw $ra 4($sp)\nlw $t9 8($sp)\naddi $sp $sp 40\n");
	if (ismain) {
		fprintf(aim_code, "li $v0 10\nsyscall\n");
	}
	else {
		fprintf(aim_code, "jr $ra\n");
	};
	mi++;
	ap = tap;
}
void callop() {
	int i=0,j=0,n=-1;
	fprintf(aim_code, "jal %s\n", midcodelist[mi].var1);
	if (strcmp(midcodelist[mi].var3, "") != 0) {
		if ((n=refloc(funid, midcodelist[mi].var3)) != -1) {
			fprintf(aim_code, "addi $s%d $v0 0\n", n);
		}
		else {
			i = addrloc(midcodelist[mi].var3);
			if (i != -1) {
				if (addrlist[i].isglo) {
					fprintf(aim_code, "sw $v0 0x%x($t8)\n", addrlist[i].addr);
				}
				else {
					fprintf(aim_code, "sw $v0 0x%x($sp)\n", addrlist[i].addr);
				}
			}
		}
	}
	j = funcloc(midcodelist[mi].var1);
	if (j != -1) {
		ftab[j].offset = 4;
	}
}
void GOTOop() {
	fprintf(aim_code, "j %s\n", midcodelist[mi].var3);
}
void BZop() {
	fprintf(aim_code, "bne $t0 1 %s\n", midcodelist[mi].var3);
}
void labop() {
	fprintf(aim_code, "%s:\n", midcodelist[mi].var3);
}

void plusop() {
	int i=0, j=0, k=0,n1=-1,n2=-1,n3=-1;
	if (isnum(midcodelist[mi].var1)) {
		fprintf(aim_code, "li $t0 %d\n", atoi(midcodelist[mi].var1));
	}
	else {
		if ((n1 = refloc(funid, midcodelist[mi].var1)) == -1) {
			i = addrloc(midcodelist[mi].var1);
			if (i != -1) {
				if (addrlist[i].isglo) {
					fprintf(aim_code, "lw $t0 0x%x($t8)\n", addrlist[i].addr);
				}
				else {
					fprintf(aim_code, "lw $t0 0x%x($sp)\n", addrlist[i].addr);
				}
			}
		}
	}
	if (isnum(midcodelist[mi].var2)) {
		fprintf(aim_code, "li $t1 %d\n", atoi(midcodelist[mi].var2));
	}
	else {
		if ((n2 = refloc(funid, midcodelist[mi].var2)) == -1) {
			j = addrloc(midcodelist[mi].var2);
			if (j != -1) {
				if (addrlist[j].isglo) {
					fprintf(aim_code, "lw $t1 0x%x($t8)\n", addrlist[j].addr);
				}
				else {
					fprintf(aim_code, "lw $t1 0x%x($sp)\n", addrlist[j].addr);
				}
			}
		}
	}
	if ((n3 = refloc(funid, midcodelist[mi].var3)) == -1) {
		k = addrloc(midcodelist[mi].var3);
		if (n1 == -1 && n2 == -1) {
			fprintf(aim_code, "add $t0 $t0 $t1\n");
		}
		else if (n1 == -1 && n2 !=-1) {
			fprintf(aim_code, "add $t0 $t0 $s%d\n",n2);
		}
		else if (n1 != -1 && n2 == -1) {
			fprintf(aim_code, "add $t0 $s%d $t1\n", n1);
		}
		else {
			fprintf(aim_code, "add $t0 $s%d $s%d\n", n1,n2);
		}
		if (addrlist[k].isglo) {
			fprintf(aim_code, "sw $t0 0x%x($t8)\n", addrlist[k].addr);
		}
		else {
			fprintf(aim_code, "sw $t0 0x%x($sp)\n", addrlist[k].addr);
		}
	}
	else {
		if (n1 == -1 && n2 == -1) {
			fprintf(aim_code, "add $s%d $t0 $t1\n",n3);
		}
		else if (n1 == -1 && n2 != -1) {
			fprintf(aim_code, "add $s%d $t0 $s%d\n", n3,n2);
		}
		else if (n1 != -1 && n2 == -1) {
			fprintf(aim_code, "add $s%d $s%d $t1\n", n3, n1);
		}
		else {
			fprintf(aim_code, "add $s%d $s%d $s%d\n",n3, n1, n2);
		}
	}
	
}
void minuop() {
	int i=0, j=0, k=0,n1=-1,n2 = -1,n3 = -1;
	if (isnum(midcodelist[mi].var1)) {
		fprintf(aim_code, "li $t0 %d\n", atoi(midcodelist[mi].var1));
	}
	else {
		if ((n1 = refloc(funid, midcodelist[mi].var1)) == -1) {
			i = addrloc(midcodelist[mi].var1);
			if (i != -1) {
				if (addrlist[i].isglo) {
					fprintf(aim_code, "lw $t0 0x%x($t8)\n", addrlist[i].addr);
				}
				else {
					fprintf(aim_code, "lw $t0 0x%x($sp)\n", addrlist[i].addr);
				}
			}
		}
	}
	if (isnum(midcodelist[mi].var2)) {
		fprintf(aim_code, "li $t1 %d\n", atoi(midcodelist[mi].var2));
	}
	else {
		if ((n2 = refloc(funid, midcodelist[mi].var2)) == -1) {
			j = addrloc(midcodelist[mi].var2);
			if (j != -1) {
				if (addrlist[j].isglo) {
					fprintf(aim_code, "lw $t1 0x%x($t8)\n", addrlist[j].addr);
				}
				else {
					fprintf(aim_code, "lw $t1 0x%x($sp)\n", addrlist[j].addr);
				}
			}
		}
	}
	if ((n3 = refloc(funid, midcodelist[mi].var3)) == -1) {
		k = addrloc(midcodelist[mi].var3);
		if (n1 == -1 && n2 == -1) {
			fprintf(aim_code, "sub $t0 $t0 $t1\n");
		}
		else if (n1 == -1 && n2 != -1) {
			fprintf(aim_code, "sub $t0 $t0 $s%d\n", n2);
		}
		else if (n1 != -1 && n2 == -1) {
			fprintf(aim_code, "sub $t0 $s%d $t1\n", n1);
		}
		else {
			fprintf(aim_code, "sub $t0 $s%d $s%d\n", n1, n2);
		}
		if (addrlist[k].isglo) {
			fprintf(aim_code, "sw $t0 0x%x($t8)\n", addrlist[k].addr);
		}
		else {
			fprintf(aim_code, "sw $t0 0x%x($sp)\n", addrlist[k].addr);
		}
	}
	else {
		if (n1 == -1 && n2 == -1) {
			fprintf(aim_code, "sub $s%d $t0 $t1\n", n3);
		}
		else if (n1 == -1 && n2 != -1) {
			fprintf(aim_code, "sub $s%d $t0 $s%d\n", n3, n2);
		}
		else if (n1 != -1 && n2 == -1) {
			fprintf(aim_code, "sub $s%d $s%d $t1\n", n3, n1);
		}
		else {
			fprintf(aim_code, "sub $s%d $s%d $s%d\n", n3, n1, n2);
		}
	}
}
void mulop() {
	int i=0, j=0, k=0,n1 = -1,n2 = -1,n3 = -1;
	if (isnum(midcodelist[mi].var1)) {
		fprintf(aim_code, "li $t0 %d\n", atoi(midcodelist[mi].var1));
	}
	else {
		if ((n1 = refloc(funid, midcodelist[mi].var1)) == -1) {
			i = addrloc(midcodelist[mi].var1);
			if (i != -1) {
				if (addrlist[i].isglo) {
					fprintf(aim_code, "lw $t0 0x%x($t8)\n", addrlist[i].addr);
				}
				else {
					fprintf(aim_code, "lw $t0 0x%x($sp)\n", addrlist[i].addr);
				}
			}
		}
	}
	if (isnum(midcodelist[mi].var2)) {
		fprintf(aim_code, "li $t1 %d\n", atoi(midcodelist[mi].var2));
	}
	else {
		if ((n2 = refloc(funid, midcodelist[mi].var2)) == -1) {
			j = addrloc(midcodelist[mi].var2);
			if (j != -1) {
				if (addrlist[j].isglo) {
					fprintf(aim_code, "lw $t1 0x%x($t8)\n", addrlist[j].addr);
				}
				else {
					fprintf(aim_code, "lw $t1 0x%x($sp)\n", addrlist[j].addr);
				}
			}
		}
	}
	if ((n3 = refloc(funid, midcodelist[mi].var3)) == -1) {
		k = addrloc(midcodelist[mi].var3);
		if (n1 == -1 && n2 == -1) {
			fprintf(aim_code, "mul $t0 $t0 $t1\n");
		}
		else if (n1 == -1 && n2 != -1) {
			fprintf(aim_code, "mul $t0 $t0 $s%d\n", n2);
		}
		else if (n1 != -1 && n2 == -1) {
			fprintf(aim_code, "mul $t0 $s%d $t1\n", n1);
		}
		else {
			fprintf(aim_code, "mul $t0 $s%d $s%d\n", n1, n2);
		}
		if (addrlist[k].isglo) {
			fprintf(aim_code, "sw $t0 0x%x($t8)\n", addrlist[k].addr);
		}
		else {
			fprintf(aim_code, "sw $t0 0x%x($sp)\n", addrlist[k].addr);
		}
	}
	else {
		if (n1 == -1 && n2 == -1) {
			fprintf(aim_code, "mul $s%d $t0 $t1\n", n3);
		}
		else if (n1 == -1 && n2 != -1) {
			fprintf(aim_code, "mul $s%d $t0 $s%d\n", n3, n2);
		}
		else if (n1 != -1 && n2 == -1) {
			fprintf(aim_code, "mul $s%d $s%d $t1\n", n3, n1);
		}
		else {
			fprintf(aim_code, "mul $s%d $s%d $s%d\n", n3, n1, n2);
		}
	}
}
void divop() {
	int i=0, j=0, k=0,n1 = -1,n2 = -1,n3 = -1;
	if (isnum(midcodelist[mi].var1)) {
		fprintf(aim_code, "li $t0 %d\n", atoi(midcodelist[mi].var1));
	}
	else {
		if ((n1 = refloc(funid, midcodelist[mi].var1)) == -1) {
			i = addrloc(midcodelist[mi].var1);
			if (i != -1) {
				if (addrlist[i].isglo) {
					fprintf(aim_code, "lw $t0 0x%x($t8)\n", addrlist[i].addr);
				}
				else {
					fprintf(aim_code, "lw $t0 0x%x($sp)\n", addrlist[i].addr);
				}
			}
		}
	}
	if (isnum(midcodelist[mi].var2)) {
		fprintf(aim_code, "li $t1 %d\n", atoi(midcodelist[mi].var2));
	}
	else {
		if ((n2 = refloc(funid, midcodelist[mi].var2)) == -1) {
			j = addrloc(midcodelist[mi].var2);
			if (j != -1) {
				if (addrlist[j].isglo) {
					fprintf(aim_code, "lw $t1 0x%x($t8)\n", addrlist[j].addr);
				}
				else {
					fprintf(aim_code, "lw $t1 0x%x($sp)\n", addrlist[j].addr);
				}
			}
		}
	}
	if ((n3 = refloc(funid, midcodelist[mi].var3)) == -1) {
		k = addrloc(midcodelist[mi].var3);
		if (n1 == -1 && n2 == -1) {
			fprintf(aim_code, "div $t0 $t0 $t1\n");
		}
		else if (n1 == -1 && n2 != -1) {
			fprintf(aim_code, "div $t0 $t0 $s%d\n", n2);
		}
		else if (n1 != -1 && n2 == -1) {
			fprintf(aim_code, "div $t0 $s%d $t1\n", n1);
		}
		else {
			fprintf(aim_code, "div $t0 $s%d $s%d\n", n1, n2);
		}
		if (addrlist[k].isglo) {
			fprintf(aim_code, "sw $t0 0x%x($t8)\n", addrlist[k].addr);
		}
		else {
			fprintf(aim_code, "sw $t0 0x%x($sp)\n", addrlist[k].addr);
		}
	}
	else {
		if (n1 == -1 && n2 == -1) {
			fprintf(aim_code, "div $s%d $t0 $t1\n", n3);
		}
		else if (n1 == -1 && n2 != -1) {
			fprintf(aim_code, "div $s%d $t0 $s%d\n", n3, n2);
		}
		else if (n1 != -1 && n2 == -1) {
			fprintf(aim_code, "div $s%d $s%d $t1\n", n3, n1);
		}
		else {
			fprintf(aim_code, "div $s%d $s%d $s%d\n", n3, n1, n2);
		}
	}
}
//		>
void grtop() {
	int i = 0, j = 0, n1 = -1, n2 = -1;
	if (isnum(midcodelist[mi].var1)) {
		fprintf(aim_code, "li $t0 %d\n", atoi(midcodelist[mi].var1));
	}
	else {
		if ((n1 = refloc(funid, midcodelist[mi].var1)) == -1) {
			i = addrloc(midcodelist[mi].var1);
			if (i != -1) {
				if (addrlist[i].isglo) {
					fprintf(aim_code, "lw $t0 0x%x($t8)\n", addrlist[i].addr);
				}
				else {
					fprintf(aim_code, "lw $t0 0x%x($sp)\n", addrlist[i].addr);
				}
			}
		}
	}
	if (isnum(midcodelist[mi].var2)) {
		fprintf(aim_code, "li $t1 %d\n", atoi(midcodelist[mi].var2));
	}
	else {
		if ((n2 = refloc(funid, midcodelist[mi].var2)) == -1) {
			j = addrloc(midcodelist[mi].var2);
			if (j != -1) {
				if (addrlist[j].isglo) {
					fprintf(aim_code, "lw $t1 0x%x($t8)\n", addrlist[j].addr);
				}
				else {
					fprintf(aim_code, "lw $t1 0x%x($sp)\n", addrlist[j].addr);
				}
			}
		}
	}
	if (n1 == -1 && n2 == -1)fprintf(aim_code, "slt $t0 $t1 $t0\n");
	if (n1 != -1 && n2 == -1)fprintf(aim_code, "slt $t0 $t1 $s%d\n",n1);
	if (n1 == -1 && n2 != -1)fprintf(aim_code, "slt $t0 $s%d $t0\n",n2);
	if (n1 != -1 && n2 != -1)fprintf(aim_code, "slt $t0 $s%d $s%d\n",n2,n1);
	
}
//		>=
void geqop() {
	int i = 0, j = 0, n1 = -1, n2 = -1;
	if (isnum(midcodelist[mi].var1)) {
		fprintf(aim_code, "li $t0 %d\n", atoi(midcodelist[mi].var1));
	}
	else {
		if ((n1 = refloc(funid, midcodelist[mi].var1)) == -1) {
			i = addrloc(midcodelist[mi].var1);
			if (i != -1) {
				if (addrlist[i].isglo) {
					fprintf(aim_code, "lw $t0 0x%x($t8)\n", addrlist[i].addr);
				}
				else {
					fprintf(aim_code, "lw $t0 0x%x($sp)\n", addrlist[i].addr);
				}
			}
		}
	}
	if (isnum(midcodelist[mi].var2)) {
		fprintf(aim_code, "li $t1 %d\n", atoi(midcodelist[mi].var2));
	}
	else {
		if ((n2 = refloc(funid, midcodelist[mi].var2)) == -1) {
			j = addrloc(midcodelist[mi].var2);
			if (j != -1) {
				if (addrlist[j].isglo) {
					fprintf(aim_code, "lw $t1 0x%x($t8)\n", addrlist[j].addr);
				}
				else {
					fprintf(aim_code, "lw $t1 0x%x($sp)\n", addrlist[j].addr);
				}
			}
		}
	}
	if (n1 == -1 && n2 == -1) {
		fprintf(aim_code, "slt $t0 $t0 $t1\n");
		fprintf(aim_code, "slti $t0 $t0 1\n");
	}
	if (n1 != -1 && n2 == -1) {
		fprintf(aim_code, "slt $t0 $s%d $t1\n",n1);
		fprintf(aim_code, "slti $t0 $t0 1\n");
	}
	if (n1 == -1 && n2 != -1) {
		fprintf(aim_code, "slt $t0 $t0 $s%d\n",n2);
		fprintf(aim_code, "slti $t0 $t0 1\n");
	}
	if (n1 != -1 && n2 != -1) {
		fprintf(aim_code, "slt $t0 $s%d $s%d\n",n1,n2);
		fprintf(aim_code, "slti $t0 $t0 1\n");
	}
	
}
//		<
void lssop() {
	int i = 0, j = 0, n1 = -1, n2 = -1;
	if (isnum(midcodelist[mi].var1)) {
		fprintf(aim_code, "li $t0 %d\n", atoi(midcodelist[mi].var1));
	}
	else {
		if ((n1 = refloc(funid, midcodelist[mi].var1)) == -1) {
			i = addrloc(midcodelist[mi].var1);
			if (i != -1) {
				if (addrlist[i].isglo) {
					fprintf(aim_code, "lw $t0 0x%x($t8)\n", addrlist[i].addr);
				}
				else {
					fprintf(aim_code, "lw $t0 0x%x($sp)\n", addrlist[i].addr);
				}
			}
		}
	}
	if (isnum(midcodelist[mi].var2)) {
		fprintf(aim_code, "li $t1 %d\n", atoi(midcodelist[mi].var2));
	}
	else {
		if ((n2 = refloc(funid, midcodelist[mi].var2)) == -1) {
			j = addrloc(midcodelist[mi].var2);
			if (j != -1) {
				if (addrlist[j].isglo) {
					fprintf(aim_code, "lw $t1 0x%x($t8)\n", addrlist[j].addr);
				}
				else {
					fprintf(aim_code, "lw $t1 0x%x($sp)\n", addrlist[j].addr);
				}
			}
		}
	}
	if (n1 == -1 && n2 == -1) {
		fprintf(aim_code, "slt $t0 $t0 $t1\n");
	}
	if (n1 != -1 && n2 == -1) {
		fprintf(aim_code, "slt $t0 $s%d $t1\n",n1);
	}
	if (n1 == -1 && n2 != -1) {
		fprintf(aim_code, "slt $t0 $t0 $s%d\n",n2);
	}
	if (n1 != -1 && n2 != -1) {
		fprintf(aim_code, "slt $t0 $s%d $s%d\n",n1,n2);
	}

}
//		<=
void leqop() {
	int i = 0, j = 0, n1 = -1, n2 = -1;
	if (isnum(midcodelist[mi].var1)) {
		fprintf(aim_code, "li $t0 %d\n", atoi(midcodelist[mi].var1));
	}
	else {
		if ((n1 = refloc(funid, midcodelist[mi].var1)) == -1) {
			i = addrloc(midcodelist[mi].var1);
			if (i != -1) {
				if (addrlist[i].isglo) {
					fprintf(aim_code, "lw $t0 0x%x($t8)\n", addrlist[i].addr);
				}
				else {
					fprintf(aim_code, "lw $t0 0x%x($sp)\n", addrlist[i].addr);
				}
			}
		}
	}
	if (isnum(midcodelist[mi].var2)) {
		fprintf(aim_code, "li $t1 %d\n", atoi(midcodelist[mi].var2));
	}
	else {
		if ((n2 = refloc(funid, midcodelist[mi].var2)) == -1) {
			j = addrloc(midcodelist[mi].var2);
			if (j != -1) {
				if (addrlist[j].isglo) {
					fprintf(aim_code, "lw $t1 0x%x($t8)\n", addrlist[j].addr);
				}
				else {
					fprintf(aim_code, "lw $t1 0x%x($sp)\n", addrlist[j].addr);
				}
			}
		}
	}
	if (n1 == -1 && n2 == -1) {
		fprintf(aim_code, "slt $t0 $t1 $t0\n");
		fprintf(aim_code, "slti $t0 $t0 1\n");
	}
	if (n1 != -1 && n2 == -1) {
		fprintf(aim_code, "slt $t0 $t1 $s%d\n",n1);
		fprintf(aim_code, "slti $t0 $t0 1\n");
	}
	if (n1 == -1 && n2 != -1) {
		fprintf(aim_code, "slt $t0 $s%d $t0\n",n2);
		fprintf(aim_code, "slti $t0 $t0 1\n");
	}
	if (n1 != -1 && n2 != -1) {
		fprintf(aim_code, "slt $t0 $s%d $s%d\n",n2,n1);
		fprintf(aim_code, "slti $t0 $t0 1\n");
	}
}
//		==
void eqlop() {
	int i = 0, j = 0, n1 = -1, n2 = -1;
	if (isnum(midcodelist[mi].var1)) {
		fprintf(aim_code, "li $t0 %d\n", atoi(midcodelist[mi].var1));
	}
	else {
		if ((n1 = refloc(funid, midcodelist[mi].var1)) == -1) {
			i = addrloc(midcodelist[mi].var1);
			if (i != -1) {
				if (addrlist[i].isglo) {
					fprintf(aim_code, "lw $t0 0x%x($t8)\n", addrlist[i].addr);
				}
				else {
					fprintf(aim_code, "lw $t0 0x%x($sp)\n", addrlist[i].addr);
				}
			}
		}
	}
	if (isnum(midcodelist[mi].var2)) {
		fprintf(aim_code, "li $t1 %d\n", atoi(midcodelist[mi].var2));
	}
	else {
		if ((n2 = refloc(funid, midcodelist[mi].var2)) == -1) {
			j = addrloc(midcodelist[mi].var2);
			if (j != -1) {
				if (addrlist[j].isglo) {
					fprintf(aim_code, "lw $t1 0x%x($t8)\n", addrlist[j].addr);
				}
				else {
					fprintf(aim_code, "lw $t1 0x%x($sp)\n", addrlist[j].addr);
				}
			}
		}
	}
	if (n1 == -1 && n2 == -1) {
		fprintf(aim_code, "slt $t2 $t0 $t1\n");
		fprintf(aim_code, "slti $t2 $t2 1\n");
		fprintf(aim_code, "slt $t3 $t1 $t0\n");
		fprintf(aim_code, "slti $t3 $t3 1\n");
		fprintf(aim_code, "and $t0 $t2 $t3\n");
	}
	if (n1 != -1 && n2 == -1) {
		fprintf(aim_code, "slt $t2 $s%d $t1\n",n1);
		fprintf(aim_code, "slti $t2 $t2 1\n");
		fprintf(aim_code, "slt $t3 $t1 $s%d\n",n1);
		fprintf(aim_code, "slti $t3 $t3 1\n");
		fprintf(aim_code, "and $t0 $t2 $t3\n");
	}
	if (n1 == -1 && n2 != -1) {
		fprintf(aim_code, "slt $t2 $t0 $s%d\n",n2);
		fprintf(aim_code, "slti $t2 $t2 1\n");
		fprintf(aim_code, "slt $t3 $s%d $t0\n",n2);
		fprintf(aim_code, "slti $t3 $t3 1\n");
		fprintf(aim_code, "and $t0 $t2 $t3\n");
	}
	if (n1 != -1 && n2 != -1) {
		fprintf(aim_code, "slt $t2 $s%d $s%d\n",n1,n2);
		fprintf(aim_code, "slti $t2 $t2 1\n");
		fprintf(aim_code, "slt $t3 $s%d $s%d\n",n2,n1);
		fprintf(aim_code, "slti $t3 $t3 1\n");
		fprintf(aim_code, "and $t0 $t2 $t3\n");
	}
	
}
//		!=
void neqop() {
	int i = 0, j = 0, n1 = -1, n2 = -1;
	if (isnum(midcodelist[mi].var1)) {
		fprintf(aim_code, "li $t0 %d\n", atoi(midcodelist[mi].var1));
	}
	else {
		if ((n1 = refloc(funid, midcodelist[mi].var1)) == -1) {
			i = addrloc(midcodelist[mi].var1);
			if (i != -1) {
				if (addrlist[i].isglo) {
					fprintf(aim_code, "lw $t0 0x%x($t8)\n", addrlist[i].addr);
				}
				else {
					fprintf(aim_code, "lw $t0 0x%x($sp)\n", addrlist[i].addr);
				}
			}
		}
	}
	if (isnum(midcodelist[mi].var2)) {
		fprintf(aim_code, "li $t1 %d\n", atoi(midcodelist[mi].var2));
	}
	else {
		if ((n2 = refloc(funid, midcodelist[mi].var2)) == -1) {
			j = addrloc(midcodelist[mi].var2);
			if (j != -1) {
				if (addrlist[j].isglo) {
					fprintf(aim_code, "lw $t1 0x%x($t8)\n", addrlist[j].addr);
				}
				else {
					fprintf(aim_code, "lw $t1 0x%x($sp)\n", addrlist[j].addr);
				}
			}
		}
	}
	if (n1 == -1 && n2 == -1) {
		fprintf(aim_code, "slt $t2 $t0 $t1\n");
		fprintf(aim_code, "slt $t3 $t1 $t0\n");
		fprintf(aim_code, "or $t0 $t2 $t3\n");
	}
	if (n1 != -1 && n2 == -1) {
		fprintf(aim_code, "slt $t2 $s%d $t1\n",n1);
		fprintf(aim_code, "slt $t3 $t1 $s%d\n",n1);
		fprintf(aim_code, "or $t0 $t2 $t3\n");
	}
	if (n1 == -1 && n2 != -1) {
		fprintf(aim_code, "slt $t2 $t0 $s%d\n",n2);
		fprintf(aim_code, "slt $t3 $s%d $t0\n",n2);
		fprintf(aim_code, "or $t0 $t2 $t3\n");
	}
	if (n1 != -1 && n2 != -1) {
		fprintf(aim_code, "slt $t2 $s%d $s%d\n",n1,n2);
		fprintf(aim_code, "slt $t3 $s%d $s%d\n",n2,n1);
		fprintf(aim_code, "or $t0 $t2 $t3\n");
	}
	
}
//		=
void assop() {
	int i=0, j=0,n1=-1,n2=-1;
	if (isnum(midcodelist[mi].var1)) {
		fprintf(aim_code, "li $t0 %d\n", atoi(midcodelist[mi].var1));
	}
	else {
		if ((n1 = refloc(funid, midcodelist[mi].var1)) == -1) {
			i = addrloc(midcodelist[mi].var1);
			if (i != -1) {
				if (addrlist[i].isglo) {
					fprintf(aim_code, "lw $t0 0x%x($t8)\n", addrlist[i].addr);
				}
				else {
					fprintf(aim_code, "lw $t0 0x%x($sp)\n", addrlist[i].addr);
				}
			}
		}
	}
	if ((n2 = refloc(funid, midcodelist[mi].var3)) == -1) {
		j = addrloc(midcodelist[mi].var3);
		if (j != -1) {
			if (n1 == -1) {
				if (addrlist[j].isglo) {
					fprintf(aim_code, "sw $t0 0x%x($t8)\n", addrlist[j].addr);
				}
				else {
					fprintf(aim_code, "sw $t0 0x%x($sp)\n", addrlist[j].addr);
				}
			}
			else {
				if (addrlist[j].isglo) {
					fprintf(aim_code, "sw $s%d 0x%x($t8)\n", n1,addrlist[j].addr);
				}
				else {
					fprintf(aim_code, "sw $s%d 0x%x($sp)\n", n1,addrlist[j].addr);
				}
			}
		}
	}
	else {
		if (n1 == -1) {
			fprintf(aim_code, "addi $s%d $t0 0\n",n2);
		}
		else {
			fprintf(aim_code, "addi $s%d $s%d 0\n", n2,n1);
		}
	}
	
}
//		[]=
void sassop() {
	int i=0, j=0, k=0,n1 = -1,n2 = -1,n3 = -1;
	int ad1=0, ad2=0, ad3=0;
	i = addrloc(midcodelist[mi].var3);
	if (i != -1)ad1 = addrlist[i].addr;
	if (isnum(midcodelist[mi].var2)) {
		ad1 = ad1 + atoi(midcodelist[mi].var2) * 4;
		if (isnum(midcodelist[mi].var1)) {
			fprintf(aim_code, "li $t0 %s\n", midcodelist[mi].var1);
		}
		else {
			if ((n1 = refloc(funid, midcodelist[mi].var1)) == -1) {
				j = addrloc(midcodelist[mi].var1);
				if (j != -1) {
					if (addrlist[j].isglo) {
						fprintf(aim_code, "lw $t0 0x%x($t8)\n", addrlist[j].addr);
					}
					else {
						fprintf(aim_code, "lw $t0 0x%x($sp)\n", addrlist[j].addr);
					}
				}
			}
		}
		if (n1 == -1) {
			if (addrlist[i].isglo) {
				fprintf(aim_code, "sw $t0 0x%x($t8)\n", ad1);
			}
			else {
				fprintf(aim_code, "sw $t0 0x%x($sp)\n", ad1);
			}
		}
		else {
			if (addrlist[i].isglo) {
				fprintf(aim_code, "sw $s%d 0x%x($t8)\n",n1, ad1);
			}
			else {
				fprintf(aim_code, "sw $s%d 0x%x($sp)\n",n1, ad1);
			}
		}
	}
	else {
		if ((n2 = refloc(funid, midcodelist[mi].var2)) == -1) {
			k = addrloc(midcodelist[mi].var2);
			if (k != -1) {
				if (addrlist[k].isglo) {
					fprintf(aim_code, "lw $t1 0x%x($t8)\n", addrlist[k].addr);
				}
				else {
					fprintf(aim_code, "lw $t1 0x%x($sp)\n", addrlist[k].addr);
				}
			}
			fprintf(aim_code, "mul $t1 $t1 4\naddi $t1 $t1 %d\n", ad1);
		}
		else {
			fprintf(aim_code, "mul $t1 $s%d 4\naddi $t1 $t1 %d\n", n2, ad1);
		}

		if (isnum(midcodelist[mi].var1)) {
			fprintf(aim_code, "li $t0 %s\n", midcodelist[mi].var1);
		}
		else {
			if ((n1 = refloc(funid, midcodelist[mi].var1)) == -1) {
				j = addrloc(midcodelist[mi].var1);
				if (j != -1) {
					if (addrlist[j].isglo) {
						fprintf(aim_code, "lw $t0 0x%x($t8)\n", addrlist[j].addr);
					}
					else {
						fprintf(aim_code, "lw $t0 0x%x($sp)\n", addrlist[j].addr);
					}
				}
			}
		}
		if (addrlist[i].isglo) {
			fprintf(aim_code, "add $t1 $t1 $t8\n");
		}
		else {
			fprintf(aim_code, "add $t1 $t1 $sp\n");
		}
		if (n1 == -1) {
			fprintf(aim_code, "sw $t0 ($t1)\n");
		}
		else {
			fprintf(aim_code, "sw $s%d ($t1)\n",n1);
		}
	}
}
//		geta
void getaop() {
	int i=0, j=0,k=0,n2 = -1,n3 = -1;
	int ad1=0,ad2=0,ad3=0;
	i = addrloc(midcodelist[mi].var1);
	if (i != -1)ad1 = addrlist[i].addr;
	if (isnum(midcodelist[mi].var2)) {
		ad1 = ad1 + atoi(midcodelist[mi].var2) * 4;
		if ((n3 = refloc(funid, midcodelist[mi].var3)) == -1) {
			j = addrloc(midcodelist[mi].var3);
			if (j != -1)ad2 = addrlist[j].addr;
			if (addrlist[i].isglo) {
				fprintf(aim_code, "lw $t0 0x%x($t8)\n", ad1);
			}
			else {
				fprintf(aim_code, "lw $t0 0x%x($sp)\n", ad1);
			}
			if (addrlist[j].isglo) {
				fprintf(aim_code, "sw $t0 0x%x($t8)\n", ad2);
			}
			else {
				fprintf(aim_code, "sw $t0 0x%x($sp)\n", ad2);
			}
		}
		else {
			if (addrlist[i].isglo) {
				fprintf(aim_code, "lw $s%d 0x%x($t8)\n",n3, ad1);
			}
			else {
				fprintf(aim_code, "lw $s%d 0x%x($sp)\n",n3, ad1);
			}
		}
	}
	else {
		if ((n2 = refloc(funid, midcodelist[mi].var2)) == -1) {
			k = addrloc(midcodelist[mi].var2);
			if (k != -1) {
				if (addrlist[k].isglo) {
					fprintf(aim_code, "lw $t1 0x%x($t8)\n", addrlist[k].addr);
				}
				else {
					fprintf(aim_code, "lw $t1 0x%x($sp)\n", addrlist[k].addr);
				}
			}
			fprintf(aim_code, "mul $t1 $t1 4\naddi $t1 $t1 %d\n", ad1);
		}
		else {
			fprintf(aim_code, "mul $t1 $s%d 4\naddi $t1 $t1 %d\n",n2, ad1);
		}
		if (addrlist[i].isglo) {
			fprintf(aim_code, "add $t1 $t1 $t8\n");
		}
		else {
			fprintf(aim_code, "add $t1 $t1 $sp\n");
		}
		if ((n3 = refloc(funid, midcodelist[mi].var3)) == -1) {
			fprintf(aim_code, "lw $t0 ($t1)\n");
			j = addrloc(midcodelist[mi].var3);
			if (j != -1) {
				ad2 = addrlist[j].addr;
				if (addrlist[j].isglo) {
					fprintf(aim_code, "sw $t0 0x%x($t8)\n", addrlist[j].addr);
				}
				else {
					fprintf(aim_code, "sw $t0 0x%x($sp)\n", addrlist[j].addr);
				}
			}
		}
		else {
			fprintf(aim_code, "lw $s%d ($t1)\n",n3);
		}
	}
}
//		scf,,,c
void scfop() {
	int i=0,ty=0,ad=0,n3 = -1;
	i = addrloc(midcodelist[mi].var3);
	if (i != -1) {
		ad = addrlist[i].addr;
		ty = addrlist[i].type;
		if (ty == INT) {
			fprintf(aim_code, "li $v0 5\nsyscall\n");
			if ((n3 = refloc(funid, midcodelist[mi].var3)) == -1) {
				if (addrlist[i].isglo) {
					fprintf(aim_code, "sw $v0 0x%x($t8)\n", addrlist[i].addr);
				}
				else {
					fprintf(aim_code, "sw $v0 0x%x($sp)\n", addrlist[i].addr);
				}
			}
			else {
				fprintf(aim_code, "addi $s%d $v0 0\n", n3);
			}

		}
		else {
			fprintf(aim_code, "li $v0 12\nsyscall\n");
			if ((n3 = refloc(funid, midcodelist[mi].var3)) == -1) {
				if (addrlist[i].isglo) {
					fprintf(aim_code, "sw $v0 0x%x($t8)\n", addrlist[i].addr);
				}
				else {
					fprintf(aim_code, "sw $v0 0x%x($sp)\n", addrlist[i].addr);
				}
			}
			else {
				fprintf(aim_code, "addi $s%d $v0 0\n", n3);
			}
		}
	}
}
//		prt,a,b,,
void prtop() {
	int ad=0,j=0,type=INT;
	int i = 0,n2 = -1;
	while (midcodelist[mi].var1[i]!='\0')
	{
		fprintf(aim_code, "li $v0 11\nli $a0 %d\nsyscall\n", midcodelist[mi].var1[i]);
		i++;
	}
	if (strcmp(midcodelist[mi].var2, "") != 0) {
		if (isnum(midcodelist[mi].var2) && strcmp(midcodelist[mi].var3, "char") == 0) {
			fprintf(aim_code, "li $v0 11\nli $a0 %d\nsyscall\n", atoi(midcodelist[mi].var2));
		}
		else if (isnum(midcodelist[mi].var2)) {
			fprintf(aim_code, "li $v0 1\nli $a0 %d\nsyscall\n", atoi(midcodelist[mi].var2));
		}
		else {
			j = addrloc(midcodelist[mi].var2);
			if (j != -1) {
				ad = addrlist[j].addr;
				type = addrlist[j].type;
			}
			if (type == INT) {
				if ((n2 = refloc(funid, midcodelist[mi].var2)) == -1) {
					if (addrlist[j].isglo) {
						fprintf(aim_code, "li $v0 1\nlw $a0 0x%x($t8)\nsyscall\n", addrlist[j].addr);
					}
					else {
						fprintf(aim_code, "li $v0 1\nlw $a0 0x%x($sp)\nsyscall\n", addrlist[j].addr);
					}
				}
				else {
					fprintf(aim_code, "li $v0 1\naddi $a0 $s%d 0\nsyscall\n", n2);
				}
			}
			else {
				if ((n2 = refloc(funid, midcodelist[mi].var2)) == -1) {
					if (addrlist[j].isglo) {
						fprintf(aim_code, "li $v0 11\nlw $a0 0x%x($t8)\nsyscall\n", addrlist[j].addr);
					}
					else {
						fprintf(aim_code, "li $v0 11\nlw $a0 0x%x($sp)\nsyscall\n", addrlist[j].addr);
					}
				}
				else {
					fprintf(aim_code, "li $v0 11\naddi $a0 $s%d 0\nsyscall\n", n2);
				}
			}
		}
	}
}
//		PUSH,,,c
void fpaop() {
	int i = 0, ad = 0, tmi = 0, j = 0, size = 0, pushnum = 0, pushoff = 0, n3 = -1;
	int temp, t;
	tmi = mi;
	while (strcmp(midcodelist[tmi].op, "call") != 0) {
		if (strcmp(midcodelist[tmi].op, "PUSH") == 0) {
			pushnum++;
		}
		tmi++;
	}
	j = funcloc(midcodelist[tmi].var1);
	size = ftab[j].size;
	pushoff = ftab[j].offset;
	if (isnum(midcodelist[mi].var3)) {
		fprintf(aim_code, "li $t0 %s\n", midcodelist[mi].var3);
	}
	else {
		if ((n3 = refloc(funid, midcodelist[mi].var3)) == -1) {
			i = addrloc(midcodelist[mi].var3);
			if (i != -1) {
				ad = addrlist[i].addr;
				if (addrlist[i].isglo) {
					fprintf(aim_code, "lw $t0 0x%x($t8)\n", addrlist[i].addr);
				}
				else {
					fprintf(aim_code, "lw $t0 0x%x($sp)\n", addrlist[i].addr);
				}

			}
		}
	}
	fprintf(aim_code, "addi $t7 $sp 0\n");
	fprintf(aim_code, "subi $t7 $t7 %d\n", size * 4 + 40 - pushoff);
	ftab[j].offset += 4;
	if (n3 == -1) {
		fprintf(aim_code, "sw $t0 ($t7)\n");
	}
	else {
		fprintf(aim_code, "sw $s%d ($t7)\n", n3);
	}
}




//		ret,,,[c]
void retop() {
	int i=0,n3 = -1;
	if (strcmp(midcodelist[mi].var3, "") != 0) {
		if (isnum(midcodelist[mi].var3)) {
			fprintf(aim_code, "li $v0 %d\n", atoi(midcodelist[mi].var3));
		}
		else {
			if ((n3 = refloc(funid, midcodelist[mi].var3)) == -1) {
				i = addrloc(midcodelist[mi].var3);
				if (i != -1) {
					if (addrlist[i].isglo) {
						fprintf(aim_code, "lw $v0 0x%x($t8)\n", addrlist[i].addr);
					}
					else {
						fprintf(aim_code, "lw $v0 0x%x($sp)\n", addrlist[i].addr);
					}
				}
			}
			else {
				fprintf(aim_code, "addi $v0 $s%d 0\n", n3);
			}
		}
	}
	fprintf(aim_code, "j fend_%s\n",funcname);
}
//		para,char,,c
void paraop() {
	int ty = 0;
	while (strcmp(midcodelist[mi].op, "para") == 0) {
		ty = (strcmp(midcodelist[mi].var1, "int") == 0) ? INT : CHAR;
		inaddr(ty, 1, 1, 0);
		offset = offset + 4;
		mi++;
	}
}

