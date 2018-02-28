#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "Global_Var.h"
#include "lex.h"
#include "err.h"
#include "gra.h"
#include "midcode.h"
#include "asm.h"
#include "opt.h"
#include "ref_count.h"
void main() {
	char test_file[100];
	int optflag=1;
	printf("test_file:");
	scanf("%s",&test_file);
	in = fopen(test_file, "r");
	out = fopen("15061103_read.txt", "w");
	lex_result = fopen("15061103_李海南_词法分析结果.txt", "w");
	midcode = fopen("midcode.txt", "w");
	mid_opt_file = fopen("mid_opt_file.txt","w");
	if (optflag == 0) {
		aim_code = fopen("aimcode.asm", "w");
	}
	else {
		aim_code = fopen("aimcode_opt.asm", "w");
	}
	if (!in) {
		err(0);
		exit(0);
	}
	memset(midcodelist, '\0', Maxcode);
	midcodeiter = 0;
	facerrflag = 0;
	linenum = 0;
	varcnt = 0;
	labelcnt = 0;
	cc = 0;
	ll = 0;
	sx = 0;
	lc = 0;
	t = 0;
	a = 0;
	f = 0;
	skipflag = 0;
	sleng = 0;
	inum = 0;
	ch = ' ';
	setup();
	enterids();
	insymbol();
	block();
	printmidcode();
	if (optflag) {
		DAG();
		ref_count();
	}
	midcodetoasm();
	fclose(in);
	fclose(out);
	fclose(lex_result);
	fclose(midcode);
	fclose(aim_code);
}