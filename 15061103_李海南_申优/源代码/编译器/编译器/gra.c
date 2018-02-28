#include "Global_Var.h"
#include "lex.h"
#include "err.h"
#include "string.h"
#include "gra.h"
#include "midcode.h"
#include "asm.h"

int loc(char idl[]) {
	int i, j;
	j = t;
	i = ftab[f - 1].ref;
	strcpy(tab[0].name, idl);
	while (strcmp(tab[j].name, idl)!=0) {
		j = tab[j].link;
	}
	if (j == 0) {
		while (strcmp(tab[i].name,idl)!=0)
		{
			i = tab[i].link;
		}
		return i;
	}
	else return j;
}
void testsemicolon() {
	if (sy == semicolon) {
		insymbol();
	}
	else {
		err(9);
	}
}
enum types testi_ch() {
	if (sy!=intsy && sy != charsy) {
		err(7);
		return notyp;
	}
	if (sy == intsy) {
		insymbol();
		return ints;
	}
	else {
		insymbol();
		return chars;
	}
	
}
void enter(char id[], enum objecttyp objtyp, enum types type,int isfun) {
	int j = 0, l = 0;
	if (t == tmax) {
		exit(0);
	}
	strcpy(tab[0].name, id);
	if (isfun) {
		if (f) {
			j = ftab[f - 1].ref;
		}
		else {
			j = t;
		}
	}
	else {
		if (tab[t].obj == funktion) {
			j = 0;
		}else{
			j = t;
		}
	}
	l = j;
	while (strcmp(tab[j].name,id)!=0)
	{
		j = tab[j].link;
	}
	if (j != 0) {
		err(6);
	}
	else {
		t = t + 1;
		strcpy(tab[t].name, id);
		tab[t].link = l;
		tab[t].obj = objtyp;
		tab[t].type = type;
		tab[t].ref = 0;
		tab[t].adr = 0;
		tab[t].normal = 0;


	}
}
void entervariable(char temp[],enum types type) {
	enter(temp, variable, type,0);
	if (type == ints) {
		inmidcode("int","","",temp);
	}
	else {
		inmidcode("char","","",temp);
	}
	ftab[f - 1].size++;
}
void enter_arr_variable(char temp[],enum types type) {
	char name[100];
	char numstr[30];
	strcpy(name, temp);
	enter(temp, variable, type,0);	
	if (sy == lbrack) {
		insymbol();
		if (sy == intcon && inum > 0) {
			enterarray(type, inum);
			itoa(inum,numstr,10);
			if (type == ints) {
				inmidcode("ints", "", numstr, name);
			}
			else {
				inmidcode("chars", "", numstr, name);
			}
			ftab[f - 1].size += inum;
			insymbol();
		}
		else {
			err(31);
			enterarray(type, 0);
			itoa(inum, numstr, 10);
			if (type == ints) {
				inmidcode("ints", "", numstr, name);
			}
			else {
				inmidcode("chars", "", numstr, name);
			}
			insymbol();
		}
		if (sy == rbrack) {
			insymbol();
		}
		else {
			err(12);
			skipsem();
		}
		tab[t].type = arrays;
		tab[t].ref = a;
	}
	
}

int enterconst(enum types type) {
	int x;
	char numstr[10];
	if (sy == ident) {
		enter(idl, constant, type,0);
		insymbol();
		if (sy == becomes) {
			insymbol();
		}
		else
		{
			err(22);
			if (sy == eql) {
				insymbol();
			}
			else {
				skipsem();
				return -1;
			}
		}
		if (type == chars) {
			tab[t].adr = inum;
			itoa(inum,numstr,10);
			inmidcode("const", "char",numstr , tab[t].name);
			insymbol();
		}
		else {
			sign = 1;
			if (sy == plus || sy == minus) {
				if (sy == minus)sign = -1;
				insymbol();
			}
			if (sy == intcon) {
				tab[t].adr = sign*inum;
				itoa(tab[t].adr, numstr, 10);
				inmidcode("const", "int", numstr, tab[t].name);
				insymbol();
			}
			else {
				err(7);
				skipsem();
				return -1;
			}
		}
	}

	else {
		err(7);
		skipsem();
		return -1;
	}
	return 0;
}
void enterarray(enum types type,long size) {
	if (size > xmax) {
		err(28);
		size = 0;
	}
	if (a == amax) {
		exit(0);
	}
	else {
		a = a + 1;
		atab[a].eltype  = type; 
		atab[a].size = size;
		if (type == ints) {
			atab[a].elsize = 4;
		}
		else {
			atab[a].elsize = 1;
		}
	}
}
void enterid(char name[],enum objecttyp objtyp,enum types type,int adr) {
	t = t + 1;
	strcpy(tab[t].name, name);
	tab[t].link = t - 1;
	
	tab[t].adr = adr;
	tab[t].normal = 1;
	tab[t].obj = objtyp;
	tab[t].paranum = 0;
	tab[t].ref = 0;
	tab[t].type = type;
}
void enterids() {
	enterid("char", typel, chars, 1);
	enterid("int", typel, ints, 1);
}
void parameterlist(enum types type) {
	if (sy == intsy) {
		type = ints;
	}
	else if (sy == charsy) {
		type = chars;
	}
	else {
		err(32);
	}
	insymbol();
	if (sy == ident) {
		enter(idl, variable, type, 0);
		if (type == ints) {
			inmidcode("para", "int", "", idl);
		}
		else {
			inmidcode("para", "char", "", idl);
		}
		tab[ftab[f - 1].ref].paranum++;
		ftab[f - 1].paranum++;
		ftab[f - 1].size++;
		insymbol();
		if (sy == comma || sy == rparent) {

			if (sy == comma)
			{
				insymbol();
				parameterlist(type);
			}
			else {
				insymbol();
			}
		}
		else {
			err(32);
			skiplbrace();
		}
	}
	else {
		err(32);
		skiplbrace();
	}


}
void constdec(enum types type) {
	int errflag;
	while (sy == constsy) {
		insymbol();
		if ((type = testi_ch()) == notyp) {
			skipsem();
			continue;
		}
		if (enterconst(type) == -1) {
			continue;
		}
		if (f != 0)ftab[f - 1].size++;
		while (sy == comma)
		{
			insymbol();
			if (enterconst(type) == -1) {
				continue;
			}
			if (f != 0)ftab[f - 1].size++;
		}
		testsemicolon();

	}
}
void v_a_fdec(char temp[],enum types type) {
	char name[100];
	if (sy == semicolon || sy == comma) {
		entervariable(temp,type);

		while (sy == comma) {
			insymbol();
			if (sy == ident) {
				strcpy(name, idl);
				insymbol();
				if (sy == semicolon || sy == comma) {
					entervariable(name, type);

				}
				else if (sy == lbrack) {
					enter_arr_variable(name, type);

				}
				else {
					entervariable(name, type);
				}
			}
		}
	}
	else if (sy == lbrack) {
		enter_arr_variable(temp, type);

		while (sy == comma) {
			insymbol();
			if (sy == ident) {
				strcpy(name, idl);
				insymbol();
				if (sy == semicolon || sy == comma) {
					entervariable(name, type);

				}
				else if (sy == lbrack) {
					enter_arr_variable(name, type);

				}
				else {
					entervariable(name, type);
				}
			}
		}
	}
	else {
		entervariable(temp, type);
	}
	testsemicolon();
}
void enterfun(char name[],int ref,enum types type) {
	strcpy(ftab[f].name, name);
	ftab[f].ref = ref;
	ftab[f].type = type;
	ftab[f].paranum = 0;
	ftab[f].offset = 4;
	f++;
}
void i_cfundec(enum types type) {
	char funname[100],name[100];
	strcpy(funname, idl);
	enter(idl, funktion, type, 1);
	
	if (type == ints) {
		inmidcode("func", "int", "", idl);
	}
	else if(type == chars){
		inmidcode("func", "char", "", idl);
	}
	else {
		inmidcode("func","void","",idl);
	}
	enterfun(idl, t,type);
	insymbol();
	if (sy == rparent)
	{
		tab[t].paranum = 0;
		insymbol();
	}
	else {
		parameterlist(type);
	}

	if (sy == lbrace)insymbol();
	else {
		err(24);
		skipcondec();
	}
	if (sy == constsy) constdec(type);
	while (sy == intsy || sy == charsy)
	{
		if (sy == intsy) {
			type = ints;
		}
		else {
			type = chars;
		}
		insymbol();
		if (sy == ident) {
			strcpy(name, idl);
			insymbol();
			v_a_fdec(name,type);
		}
	}
	linestatement();
	if (sy == rbrace) {
		insymbol();
	}
	else err(19);
	inmidcode("end","","",funname);

}
void block() {
	int n;
	char temp[100];
	enum types type = notyp;
	if (sy == constsy) { constdec(type); }
	if (sy != voidsy && sy != intsy && sy != charsy) {
		err(22);
		skipfundec();
	}
	while (sy == intsy || sy == charsy) {
		if (sy == intsy) {
			type = ints;
		}
		else {
			type = chars;
		}
		insymbol();
		if (sy == ident) {
			strcpy(temp, idl);
			insymbol();
			if (sy == lparent) {
				retflag = 0;
				funtypflag = 1;
				i_cfundec(type);
				if (retflag == 0)err(20);
				break;
			}
			else {
				v_a_fdec(temp,type);
			}
		}
		else {
			err(11);
			skiplbrace();
		}

	}
	if (sy != voidsy && sy != intsy && sy != charsy) {
		err(22);
		skipfundec();
	}
	while (sy == voidsy || sy == intsy || sy == charsy) {
		switch (sy)
		{
		case voidsy: {
			type = notyp;
			funtypflag = 0;
			break;
		}
		case intsy: {type = ints; funtypflag = 1; break; }
		case charsy:type = chars; funtypflag = 1; break;
		
		}
		insymbol();
		if (sy == ident) {
			insymbol();
			if (sy == lparent) {
				retflag = 0;
				i_cfundec(type);
				if (funtypflag == 1) {
					if (retflag == 0) {
						err(20);
					}
				}
			}
		}
		else if (sy == mainsy) {
			if (type != notyp) {
				err(33);
			}
			insymbol();
			if (sy == lparent) {
				i_cfundec(type);
			}
			break;
		}
	}
	if (strcmp(ftab[f - 1].name, "main") != 0) {
		err(25);
	}
	
}