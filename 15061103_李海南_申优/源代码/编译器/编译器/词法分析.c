#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "err.h"
#include "Global_Var.h"
void setup() {
	
	strcpy(key[1], "case");
	strcpy(key[2], "char");
	strcpy(key[3], "const");
	strcpy(key[4], "do");
	strcpy(key[5], "if");
	strcpy(key[6], "int");
	strcpy(key[7], "main");
	strcpy(key[8],"printf");
	strcpy(key[9],"return");
	strcpy(key[10],"scanf");
	strcpy(key[11], "switch");
	strcpy(key[12], "void");
	strcpy(key[13], "while");
	
	

	
	ksy[1] = casesy;
	ksy[2] = charsy;
	ksy[3] = constsy;
	ksy[4] = dosy;
	ksy[5] = ifsy;
	ksy[6] = intsy;
	ksy[7] = mainsy;
	ksy[8] = printsy;
	ksy[9] = returnsy;
	ksy[10] = scanfsy;
	ksy[11] = switchsy;
	ksy[12] = voidsy;
	ksy[13] = whilesy;
	


	sps['+'] = plus;
	sps['-'] = minus;
	sps['*'] = times;
	sps['/'] = idiv;
	sps['('] = lparent;
	sps[')'] = rparent;
	sps['}'] = rbrace;
	sps[','] = comma;
	sps['['] = lbrack;
	sps[']'] = rbrack;
	sps[';'] = semicolon;
}
char h2l(char c) {
	char lower;
	if (isupper(c)) {
		lower = c + 32;
		return lower;
	}
	return c;
}
void sytosym(enum Symbol sy,char sym[]) {
	strcpy(sym, "");
	switch (sy)
	{
	case intsy:
		strcpy(sym, "intsy");
		break;
	case charsy:
		strcpy(sym, "charsy");
		break;
	case intcon:
		strcpy(sym,"intcon");
		break;
	case charcon:
		strcpy(sym, "charcon");
		break;
	case stringcon:
		strcpy(sym, "stringcon");
		break;
	case plus:
		strcpy(sym, "plus");
		break;
	case minus:
		strcpy(sym, "minus");
		break;
	case times:
		strcpy(sym, "times");
		break;
	case idiv:
		strcpy(sym, "idiv");
		break;
	case eql:
		strcpy(sym, "eql");
		break;
	case neq:
		strcpy(sym, "neq");
		break;
	case gtr:
		strcpy(sym, "gtr");
		break;
	case geq:
		strcpy(sym, "geq");
		break;
	case lss:
		strcpy(sym, "lss");
		break;
	case leq:
		strcpy(sym, "leq");
		break;
	case lparent:
		strcpy(sym, "lparent");
		break;
	case rparent:
		strcpy(sym, "rparent");
		break;
	case lbrack:
		strcpy(sym, "lbrack");
		break;
	case rbrack:
		strcpy(sym, "rbrack");
		break;
	case lbrace:
		strcpy(sym, "lbrace");
		break;
	case rbrace:
		strcpy(sym, "rbrace");
		break;
	case comma:
		strcpy(sym, "comma");
		break;
	case semicolon:
		strcpy(sym, "semicolon");
		break;
	case colon:
		strcpy(sym, "colon");
		break;
	case becomes:
		strcpy(sym, "becomes");
		break;
	case constsy:
		strcpy(sym, "constsy");
		break;
	case voidsy:
		strcpy(sym, "voidsy");
		break;
	case mainsy:
		strcpy(sym, "mainsy");
		break;
	case ident:
		strcpy(sym, "ident");
		break;
	case ifsy:
		strcpy(sym, "ifsy");
		break;
	case switchsy:
		strcpy(sym, "switchsy");
		break;
	case casesy:
		strcpy(sym, "casesy");
		break;
	case whilesy:
		strcpy(sym, "whilesy");
		break;
	case dosy:
		strcpy(sym, "dosy");
		break;
	case printsy:
		strcpy(sym, "printsy");
		break;
	case returnsy:
		strcpy(sym, "returnsy");
		break;
	case scanfsy:
		strcpy(sym, "scanfsy");
		break;
	default:
		strcpy(sym, "");
		break;
	}
}
void nextch() {
	if (cc == ll) {
		if(feof(in)) {
			printf("success");
			fprintf(out, "\n");
			fprintf(out, "program complete");
			ch = '\0';
			return;
		}
		if (errpos != 0) {
			if(skipflag){ endskip(); }
			fprintf(out, "\n");
			errpos = 0;
		} 
		linenum++;
		ll = 0;
		cc = 0;
		while ((ch = fgetc(in)) != '\n')
		{
			if(ch!=EOF){
				ll = ll + 1;
				line[ll] = ch;
				fputc(ch,out);
			}
			else
			{
				break;
			}
		}
		fputc('\n', out);
		fflush(out);
		ll = ll + 1;
		line[ll] = ' ';
	}
	cc = cc + 1;
	ch = line[cc];
}
void insymbol(){
	//label 1, 2, 3; 
	int i, j, k, e,flag=0;
	char s[100] = {'\0'};
	while (ch == ' ' || ch == '\t') {
			nextch();
	}
	if (isalpha(ch) || ch == '_')
	{
		k = 0;
		i = 0;
		memset(id, '\0', sizeof(id));
		memset(idl, '\0', sizeof(idl));
		do {
			if (k < alng) {
				id[k] = ch;
				idl[k] = h2l(ch);
				k = k + 1;
			}
			nextch();
		} while (isalpha(ch) || isdigit(ch) || ch == '_');
		i = 1;
		j = nkw - 1; //binary search } {二分查表，查找id在表中的位置}
		do {
			k = (i + j) / 2;
			if (strcmp(idl, key[k]) <= 0)
				j = k - 1;
			if (strcmp(idl, key[k]) >= 0)
				i = k + 1;
		} while (i <= j);
		if (i - 1 > j) {
			sy = ksy[k];
		}
		else sy = ident;
		sytosym(sy, sym);
		fprintf(lex_result, "%s %s\r\n", sym, id);
	}
	else if (isdigit(ch)) {
		k = 0;
		inum = 0;
		sy = intcon;
		flag = 0;
		if (ch == '0') {
			flag = 1;
		}
		do {
			inum = inum * 10 + ch - '0';
			k = k + 1;
			nextch();
		} while ((ch >= '0') && (ch <= '9'));
		if (k > 1 && flag){err(27);inum=0;k=1;}
		if ((k > kmax) || (inum > nmax)) {
			printf("k is bigger than kmax or inum is bigger than nmax");
			err(28);
			inum = 0;
			k = 1;
		}
		sytosym(sy, sym);
		fprintf(lex_result, "%s %d\r\n", sym, inum);
	}
	else {
		switch (ch) {
		case '!': {
			nextch();
			if (ch == '=') {       //不等于}
				sy = neq;
				sytosym(sy, sym);
				fprintf(lex_result, "%s \r\n", sym );
				nextch();
			}
			break;
		}
		case ':':{
			nextch();
			sy = colon;
			sytosym(sy, sym);
			fprintf(lex_result, "%s \r\n", sym );
			break;
		}
		case '=': {
			sy = becomes;
			nextch();
			if (ch == '=') {
				nextch();
				sy = eql;
			}
			sytosym(sy, sym);
			fprintf(lex_result, "%s\r\n", sym);
			break;
		}
		case '<': {
			nextch();
			if (ch == '=') {
				sy = leq;		//小于等于}
				nextch();
			}
			else  sy = lss;
			sytosym(sy, sym);
			fprintf(lex_result, "%s\r\n", sym);
			break;
		}//小于 }
		case '>': {				//大于号}
			nextch();
			if (ch == '=') {
				sy = geq;		//大于等于}
				nextch();
			}
			else  sy = gtr;		//{ 大于 }
			sytosym(sy, sym);
			fprintf(lex_result, "%s\r\n", sym);
			break;
		}
		case '"': {		//{双引号}
			k = 0;
			i = 0;
			while (cc != ll) {
				nextch();
				if (ch < 32 || ch >126) {
					printf("%c can not exist in string", ch);
					err(29);
				}
				if (ch == '"') {
					break;
				}
				s[i] = ch;
				i++;
				if (sx + k == smax) {		//{ 是否超表 }
					printf("stab over");
					exit(0);
				}
				stab[sx + k] = ch;			
				k = k + 1;
				if (cc == ll) {           
					err(2);
					break;
				}
			}
			nextch();
			sy = stringcon; //{字符串类型}
			inum = sx;
			sleng = k;
			sx = sx + k;
			sytosym(sy, sym);
			fprintf(lex_result, "%s %s\r\n", sym,s);
			strcpy(idl,s);
			break;
		}
		case '\'': {
			i = 0;
			s[i] = '\'';
			nextch();
			i++;
			if (cc == ll) {           // {？？？？？？}
				printf("到行尾了");
				err(4);
				return;
			}
			if (!isalpha(ch) && !isdigit(ch) &&
				ch != '+' && ch != '-' && ch != '*'
				&& ch != '/' && ch != '_') {
				//baocuo
				printf("%c is not char", ch);
				err(30);
				s[i] = ch;
				stab[sx] = ch;
			}else if(ch == '\''){
				err(30);
				nextch();
				return;
			}
			else
			{
				s[i] = ch;
				stab[sx] = ch;
			}
			nextch();
			i++;
			if (ch != '\'') {
				//baocuo 
				err(4);
				s[i] = '\'';
				nextch();
				sy = charcon; //{字符类型}
				inum = stab[sx];// { inum存ascii码 }
				sx = sx + 1;
				sytosym(sy, sym);
				fprintf(lex_result, "%s %s\r\n", sym, s);
			}
			else {
				s[i] = '\'';
				nextch();
				sy = charcon; //{字符类型}
				inum = stab[sx];// { inum存ascii码 }
				sx = sx + 1;
				sytosym(sy, sym);
				fprintf(lex_result, "%s %s\r\n", sym, s);
			}
			break;
		}
		case '(': {
			nextch();
			sy = lparent;//{ 左括号 }
			sytosym(sy, sym);
			fprintf(lex_result, "%s\r\n", sym);
			break;
		}
		case '{': {
			nextch();
			sy = lbrace;
			sytosym(sy, sym);
			fprintf(lex_result, "%s\r\n", sym);
			break;
		}
		case '+':case '-':case '*':case '/':case ')':case '}':case  ',':case '[':case ']':case ';': {
			sy = sps[ch];
			nextch();
			sytosym(sy, sym);
			fprintf(lex_result, "%s\r\n", sym);
			break;
		}
		case '$':case '@':case '?':case '&':case '^': {
			printf("%c is illegal", ch);
			err(3);
			nextch();
			break;
		}
		}
	}
}
