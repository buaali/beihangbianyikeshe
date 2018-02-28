#include "Global_Var.h"
#include "lex.h"
#include "err.h"
#include "string.h"
#include "gra.h"
#include "midcode.h"
#include "asm.h"
void statement() {
	int i,j;
	char paravar[200][200];
	int paracon=0,t;
	if (sy == ident) {}
	switch (sy) {
	case ident: {
		i = loc(idl);
		if (i != 0) {
			insymbol();
			switch (tab[i].obj) {
			case constant:case typel: err(22); skipsem(); break;
				//赋值
			case variable:assignment(tab[i]); break;
				//函数调用
			case funktion: {
				if (sy == lparent) {
					insymbol();
					if (tab[i].paranum == 0) {
						if (sy == rparent) {
							inmidcode("call", tab[i].name, "", "");
							insymbol();
							testsemicolon();
						}
						else { err(18); skipsem(); }
					}
					else {
						for (j = 1; j < tab[i].paranum; j++) {
							facerrflag = 0;
							simpleexpression();
							if (facerrflag == 1) {
								skiprparent();
								break;
							}
							
							strcpy(paravar[paracon++], nowvar);
							if (sy != comma) {
								err(15); skiprparent(); break;
							}
							else {
								insymbol();
							}
						}
						if (j == tab[i].paranum) {
							facerrflag = 0;
							simpleexpression();
							if (facerrflag == 1) {
								skiprparent();
							}
							strcpy(paravar[paracon++], nowvar);
							
						}
						for (t = 0; t < paracon; t++) {
							inmidcode("PUSH", "", "", paravar[t]);
						}
						if (sy == rparent) {
							inmidcode("call", tab[i].name, "", "");
							insymbol();
							testsemicolon();
						}
						else { err(18); skipsem(); }
					}
				}
				break;
			}
			}
		}
		else {
			err(16);
			skipsem();
		}
		break;
	}
	case ifsy: ifstatement(); break;
	case switchsy: switchstatement(); break;
	case dosy: dowhilestatement(); break;
	case lbrace: {
		insymbol();
		linestatement();
		if (sy != rbrace) {
			err(19);
		}
		else {
			insymbol();
		}
		break;
	}
	case scanfsy: scanfstatement(); break;
	case printsy: printstatement(); break;
	case semicolon: {
		insymbol();
		break;
	}
	case returnsy: returnstatement(); break;
	default: {
		err(22);
		skipsem(); 
		break;
	
	}
	}
}
void returnstatement() {
	char var3[200];
	insymbol();
	retflag = 1;
	if (sy == lparent) {
		if (funtypflag == 0)err(20);
		insymbol();
		facerrflag = 0;
		simpleexpression();
		if (facerrflag == 1) {
			skiprparent();
		}
		strcpy(var3,nowvar);
		if (sy == rparent) {
			inmidcode("ret","","",var3);
			insymbol();
			testsemicolon();
		}
	}
	else if (sy == semicolon) {
		if (funtypflag == 1)err(20);
		inmidcode("ret","","","");
		insymbol();
	}
	else {
		err(22); skipsem();
	}
}
void printstatement() {
	char var1[200] = { '\0' }, var2[200] = { '\0' }, var3[200] = {'\0'};
	insymbol();
	if (sy == lparent) {
		insymbol();
		if (sy == stringcon) {
			strcpy(var1,idl);
			insymbol();
			if (sy == comma) {
				insymbol();
				facerrflag = 0;
				simpleexpression();
				if (facerrflag == 1) {
					skiprparent();
				}
				strcpy(var2, nowvar);
				if (sy != rparent) {
					err(18);
					skipsem();
				}
				else {
					insymbol();
					testsemicolon();
				}
			}
			else if (sy == rparent) {
				insymbol();
				testsemicolon();
			}

		}
		else {
			facerrflag = 0;
			simpleexpression();
			if (facerrflag == 1) {
				skiprparent();
			}
			strcpy(var2, nowvar);
			if (sy != rparent) {
				err(18);
				skipsem();
			}
			else {
				insymbol();
				testsemicolon();
			}
		}
		inmidcode("prt",var1,var2,(cla == CHAR)?"char":"int");
	}
}
void scanfstatement() {
	int i;
	insymbol();
	if (sy == lparent) {
		insymbol();
		if (sy != ident) {
			err(11);
			skipsem();
			return;
		}
		i = loc(idl);
		if (i != 0) {
			if (tab[i].obj != variable) {
				err(22);
				skipsem();
				return;
			}
			inmidcode("scf","","",idl);
			insymbol();
		}
		else {
			err(16);
			skipsem();
			return;
		}
		
		while (sy == comma)
		{
			insymbol();
			if (sy != ident) {
				err(11);
				skipsem();
				return;
			}
			i = loc(idl);
			if (i != 0) {
				if (tab[i].obj != variable) {
					err(22);
					skipsem();
					return;
				}
				inmidcode("scf", "", "", idl);
				insymbol();
			}
			else {
				err(16);
				skipsem();
				return;
			}
			
		}
		if (sy == rparent) {
			insymbol();
			testsemicolon();
		}
		else {
			err(18);
			skipsem();
			return;
		}
	}
	else {
		err(17);
		skipsem();
		return;
	}
}
void linestatement() {
	while (1) {
		if (sy == ident || sy == ifsy || sy == switchsy ||
			sy == dosy || sy == lbrace || sy == scanfsy ||
			sy == printsy || sy == semicolon || sy == returnsy) {
			statement();
		}
		else if (sy == rbrace) {
			break;
		}
		else {
			err(22);
			skipstatement();
		}
	}
}
void switchstatement() {
	char var1[200], var2[200], var3[200];
	char label1[20];
	strcpy(label1, newlabel());
	insymbol();
	if (sy == lparent) {
		insymbol();
		facerrflag = 0;
		simpleexpression();
		if (facerrflag == 1) {
			skiprparent();
		}
		strcpy(var1,nowvar);
		if (sy == rparent) {
			insymbol();
			if (sy == lbrace) {
				insymbol();
				fact_sheet(var1,label1);
				if (sy == rbrace) {
					inmidcode("lab:","","",label1);
					insymbol();
				}
				else {
					err(19);
					skiprbrace();
				}
			}
			else {
				err(24);
				skipcase();
				fact_sheet(var1, label1);
				if (sy == rbrace) {
					inmidcode("lab:", "", "", label1);
					insymbol();
				}
				else {
					err(19);
					skiprbrace();
				}
			}
		}
	}
}
void fact_sheet(char var[],char label[]) {
	char label1[20], label2[20];
	char var2[200],numstr[20];
	
	int i;
	while (sy == casesy) {
		strcpy(label2, newlabel());
		insymbol();
		if (sy == charcon || sy == intcon ) {
			itoa(inum, numstr, 10);
			inmidcode("==",var,numstr,"");
			insymbol();
			if (sy == colon)
			{
				inmidcode("BZ", "", "", label2);
				insymbol();
				statement();
				inmidcode("GOTO","","",label);
				inmidcode("lab:", "", "", label2);
			}
			else {
				err(22);
				skipcase();
			}
		}
		else {
			err(22);
			skipcase();
		}
	}
	if (sy == rbrace) {
		return;
	}
	else if (strcmp("default", idl) == 0) {
		err(34);
		skiprbrace();
	}
}
void dowhilestatement() {
	char label1[20],label2[20];
	strcpy(label1, newlabel());
	strcpy(label2, newlabel());
	inmidcode("lab:","","",label1);
	insymbol();
	statement();

	if (sy == whilesy) {
		insymbol();
		if (sy != lparent) {
			err(17);
		}
		else {
			insymbol();
		}
		codition();
		if (sy != rparent) {
			err(18);
		}
		else {
			insymbol();
		}
		inmidcode("BZ","","",label2);
		
		inmidcode("GOTO","","",label1);
		inmidcode("lab:", "", "", label2);
	}

}
void factor() {
	int i, j,sign;
	char paravar[200][200];
	int paracon = 0,k;
	char var3[30],t[30];
	if (sy == plus || sy == minus || sy == ident || sy == charcon || sy == intcon || sy == lparent)
	{
		if (sy == ident)
		{
			i = loc(idl);
			if (i != 0) {
				insymbol();
				switch (tab[i].obj) {
				case constant: {
					strcpy(nowvar, tab[i].name);
					break;
				}
				case variable: {

					if (sy == lbrack)
					{
						if (tab[i].type != arrays) {
							err(22);
							facerrflag = 1;
							return;
						}
						selector(tab[i]);
						if (isnum(nowvar)) {
							if (tab[i].type == arrays && atab[tab[i].ref].size <= atoi(nowvar)) {
								err(35);
							}
						}
						strcpy(t, nowvar);
						strcpy(nowvar, newvar());
						ftab[f - 1].size++;
						inmidcode("geta", tab[i].name, t, nowvar);
					}
					else {
						strcpy(nowvar,tab[i].name);
					}
					break;
				}
				case funktion: {
					if (tab[i].type == notyp) {
						err(20);
					}
					if (sy == lparent) {
						insymbol();
						if (tab[i].paranum == 0) {
							if (sy == rparent) {
								strcpy(var3, newvar());
								ftab[f - 1].size++;
								inmidcode("call", tab[i].name, "", var3);
								strcpy(nowvar, var3);
								insymbol();
							}
							else { err(18); facerrflag = 1; return;
							}
						}
						else {
							for (j = 1; j < tab[i].paranum; j++) {
								facerrflag = 0;
								simpleexpression();
								if (facerrflag == 1) {
									skiprparent();
									break;
								}
						
								strcpy(paravar[paracon++], nowvar);
								if (sy != comma) {
									err(15); skiprparent(); break;
								}
								insymbol();
							}
							if (j == tab[i].paranum) {
								facerrflag = 0;
								simpleexpression();
								if (facerrflag == 1) {
									skiprparent();
								}
						
								strcpy(paravar[paracon++], nowvar);
							}
							for (k = 0; k < paracon; k++) {
								inmidcode("PUSH", "", "", paravar[k]);
							}
							if (sy == rparent) {
								strcpy(var3, newvar());
								ftab[f - 1].size++;
								inmidcode("call", tab[i].name,"",var3);
								strcpy(nowvar, var3);
								insymbol();
							}
							else { err(18); facerrflag = 1; return;
							}
						}
					}
					break;
				}
				}
			}
			else { 
				err(16);
				facerrflag = 1;
				return;
			}
		}
		else if (sy == plus || sy == minus || sy == charcon || sy == intcon)
		{
			sign = 1;
			if (sy == charcon)
			{
				sprintf(nowvar, "%d", inum);
				cla = CHAR;
			}
			else if (sy == plus) {
				insymbol();
			}
			else if (sy == minus) {
				sign = -1;
				insymbol();
			}
			if (sy == intcon) {
				sprintf(nowvar, "%d", sign*inum);
				cla = INT;
			}
			
			insymbol();
		}
		else if (sy == lparent)
		{
			insymbol();
			facerrflag = 0;
			simpleexpression();
			if (facerrflag == 1) {
				skiprparent();
			}
			if (sy == rparent)
				insymbol();
			else { err(18); facerrflag = 1; return; }
		}
	}
	else { err(22); facerrflag = 1; return;
	}
}
void term() {
	char var1[200], var2[200], var3[200];
	enum Symbol op;
	factor();
	strcpy(var3, nowvar);
	while (sy == times || sy == idiv)
	{
		strcpy(var1,var3);
		op = sy;
		insymbol();
		factor();
		if (op == times) {
			strcpy(var2, nowvar);
			strcpy(var3, newvar());
			ftab[f - 1].size++;
			inmidcode("mul",var1,var2,var3);
		}
		else {
			strcpy(var2,nowvar);
			strcpy(var3,newvar());
			ftab[f - 1].size++;
			inmidcode("div",var1,var2,var3);
		}
		
		//		}
	}

	strcpy(nowvar, var3);
}
void simpleexpression() {
	enum Symbol op;
	char var1[200], var2[200], var3[200];
	if (sy == plus || sy == minus) {
		op = sy;
		insymbol();
		term();
		if (op == plus) {
			strcpy(var3, nowvar);
		}
		else {
			strcpy(var2, nowvar);
			strcpy(var3, newvar());
			ftab[f - 1].size++;
			inmidcode("minu","0",var2,var3);
		}
	}
	else {
		term();
		strcpy(var3, nowvar);
	}
	while (sy == plus || sy == minus) {
		strcpy(var1, var3);
		op = sy;
		insymbol();
		term();
		strcpy(var2, nowvar);
		strcpy(var3, newvar());
		ftab[f - 1].size++;
		if (op == plus) {
			inmidcode("plus",var1,var2,var3);
		}
		else {
			inmidcode("minu",var1,var2,var3);
		}
	}

	strcpy(nowvar, var3);
}
void assignment(struct Tab tabi) {
	char var1[200],var2[200], var3[200];
	strcpy(var3,tabi.name);
	strcpy(var2,"");
	if (sy == lbrack) {
		selector(tabi);
		strcpy(var2, nowvar);
		if (isnum(nowvar)) {
			if (tabi.type == arrays && atab[tabi.ref].size <= atoi(nowvar)) {
				err(35);
			}
		}
	}
	if (sy == becomes)
		insymbol();
	else {
		err(22);
		if (sy == eql) {
			insymbol();
		}
		else {
			skipsem();
		}
	}
	facerrflag = 0;
	simpleexpression();
	strcpy(var1, nowvar);
	if (strcmp(var2, "") != 0) {
		inmidcode("[]=", var1, var2, var3);
	}
	else {
		inmidcode("=", var1, "", var3);
	}
	if (facerrflag == 1) {
		skipsem();
	}
	else {
		testsemicolon();
	}

}
void selector(struct Tab tabi) {
	char t[30];
	insymbol();
	facerrflag = 0;
	simpleexpression();
	if (facerrflag == 1) {
		skiprbrack();
	}
	if (sy == rbrack) {
		insymbol();
	}
	else
		err(12);
}
void codition() {
	char var1[200], var2[200], var3[200];
	enum Symbol op;
	facerrflag = 0;
	simpleexpression();
	if (facerrflag == 1) {
		skiprparent();
	}
	strcpy(var1, nowvar);
	if (sy == eql || sy == neq || sy == geq || sy == gtr || sy == lss || sy == leq) {
		op = sy;
		insymbol();
		facerrflag = 0;
		simpleexpression();
		if (facerrflag == 1) {
			skiprparent();
		}
		strcpy(var2, nowvar);
		strcpy(var3, newvar());
		ftab[f - 1].size++;
		if (op == lss) {
			inmidcode("<",var1,var2,var3);
		}
		else if (op == leq) {
			inmidcode("<=", var1, var2, var3);
		}
		else if (op == gtr) {
			inmidcode(">", var1, var2, var3);
		}
		else if (op == geq) {
			inmidcode(">=", var1, var2, var3);
		}
		else if (op == neq) {
			inmidcode("!=", var1, var2, var3);
		}
		else if (op == eql) {
			inmidcode("==", var1, var2, var3);
		}
	}
	else {
		strcpy(var2, "0");
		strcpy(var3, newvar());
		ftab[f - 1].size++;
		inmidcode("!=",var1,var2,var3);
	}
	strcpy(nowvar, var3);
}
void ifstatement() {
	char label1[20],result[200];
	strcpy(label1, newlabel());
	insymbol();
	if (sy == lparent) {
		insymbol();
		codition();
		strcpy(result,nowvar);
		inmidcode("BZ", "  ", "  ", label1);
		if (sy == rparent) {
			insymbol();
			statement();
			inmidcode("lab:", "", "", label1);
		}
		else {
			err(18); skipsem();
		}
	}
	else { err(17); skipsem(); }
}