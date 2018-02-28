#include <stdio.h>
#include "err.h"
#include "Global_Var.h"
#include "lex.h"
#include "asm.h"
char msg[100][100]= {
	"缺少源文件",			//0
	"文件不完整",			//1
	"缺少\"",				//2
	"不可接受的字符",		//3
	"缺少“'”",			//4
	"超出符号表范围",		//5
	"变量名冲突",			//6	
	"常量声明失败",			//7
	"等号右侧字符非法",		//8	
	"丢失“;”",			//9
	"关键字错误",			//10
	"丢失标识符",			//11
	"丢失“]”",			//12
	"调用函数未定义",		//13
	"不允许前0",			//14
	"形参个数不匹配",		//15
	"未定义变量",			//16
	"丢失“(”",			//17
	"丢失“)”",			//18
	"丢失“}”",			//19
	"函数返回错误",			//20
	"表达式缺失或错误",		//21
	"句子不合法",			//22
	"给常数赋值",			//23
	"缺少“{”",			//24
	"缺少main函数",			//25
	"常量没有初始化",		//26
	"数字有前零",			//27
	"数字过大",				//28
	"字符串内容错误",		//29
	"字符内容错误",			//30
	"变量声明失败",			//31
	"参数错误",				//32
	"函数声明错误",			//33
	"文法中不允许有default", //34
	"数组下标越界"			//35
};
void endskip() {
	while (errpos < cc) {
		fprintf(out, "-");
		errpos = errpos + 1;
	}
	skipflag = 0;
}
void skipstatement() {
	skipflag = 1;
	while ( sy != ifsy && sy != switchsy &&
		sy != dosy && sy != lbrace && sy != scanfsy &&
		sy != printsy && sy != semicolon && sy != returnsy  && sy!=rbrace)
	{
		insymbol();
	}
	if (sy == semicolon) {
		insymbol();
	}
	if (skipflag)endskip();
}
void skiprparent() {
	skipflag = 1;
	while (sy != rparent)
	{
		insymbol();
	}
	if (skipflag)endskip();
}
void skiprbrack() {
	skipflag = 1;
	while (sy != rbrack )
	{
		insymbol();
	}
	if (skipflag)endskip();
}
void skipcase() {
	skipflag = 1;
	while (sy != rbrace && sy != casesy)
	{
		insymbol();
	}
	if (skipflag)endskip();
}
void skiprbrace() {
	skipflag = 1;
	while (sy != rbrace)
	{
		insymbol();
	}
	if (skipflag)endskip();
}
void skipfacnext() {
	skipflag = 1;
	while (sy != plus && sy != minus && sy != times && sy!=idiv && sy!=rparent && sy!=semicolon)
	{
		insymbol();
	}
	if (skipflag)endskip();
}
void skipfundec() {
	skipflag = 1;
	while (sy != voidsy && sy != intsy && sy != charsy)
	{
		insymbol();
	}
	if (skipflag)endskip();
}
void skipdec() {
	skipflag = 1;
	while ( sy!=intsy && sy!=charsy)
	{
		insymbol();
	}
	if (skipflag)endskip();
}
void skipcondec() {
	skipflag = 1;
	while (sy != constsy && sy != intsy && sy != charsy)
	{
		insymbol();
	}
	if (skipflag)endskip();
}
void skipsem() {
	skipflag = 1;
	while (sy != semicolon)
	{
		insymbol();
	}
	insymbol();
	if (skipflag)endskip();
}
void skiplbrace() {
	skipflag = 1;
	while (sy != lbrace) {
		insymbol();
	}
	if (skipflag)endskip();
}

void err(int n) {
	int i;
	if (errpos == 0) {
		fprintf(out, "*");
	}
	if (cc > errpos) {
		for (i = 3; i < cc - errpos; i++) {
			fprintf(out," ");
		}
		fprintf(out, "^");
		errpos = cc+1;
	}
	printf("第%d行，第%d个字符：%s\n",linenum,cc,msg[n]);
}