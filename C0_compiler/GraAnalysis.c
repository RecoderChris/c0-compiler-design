#include<stdio.h>
#include<stdlib.h>
#include"compiler.h"
#define	EXPNAME		1
#define TERMNAME	2
#define FACTORNAME	3
extern SymTable C0_SymTable;
extern symb SYM[MAX_SYMBOL_TYPE];
extern char SymType[MAX_SYM_LENGTH], StringValue[MAX_ID_LENGTH],op[OP_NUM][MAX_OP_LEN],*StringHead;
extern int NumValue, number, ll;
int Judged = 0,pos = 0;
char itemname[MAX_ID_LENGTH] = "                         ", itemtype[MAX_SYM_LENGTH] = "VOID                     ", itemkind[MAX_SYM_LENGTH] = "NOTYPE                   ";
char num10[MAX_ID_LENGTH] = "                         ";
char operation[MAX_ID_LENGTH], operand1[MAX_ID_LENGTH], operand2[MAX_ID_LENGTH], operand3[MAX_ID_LENGTH];
int itemvalue = 0, itemnumber = 0, funcpara = 0, itemaddress = 0,curlev = 0;
int return_flag = 0;
char exp_name[MAX_ID_LENGTH] = "exp                      ", term_name[MAX_ID_LENGTH] = "term                     ", factor_name[MAX_ID_LENGTH] = "$tmp                     ";
int exp_num = 0, term_num = 0, factor_num = 0;
char left[MAX_ID_LENGTH] = "                         ";
char if_label[MAX_ID_LENGTH] = "Ilabel                   ", else_label[MAX_ID_LENGTH] = "Elabel                   ",
while_label[MAX_ID_LENGTH] = "Wlabel                   ", out_loop_label[MAX_ID_LENGTH] = "Olabel                   ", case_label[MAX_ID_LENGTH] = "Clabel                   ";
char tmp_name[MAX_ID_LENGTH] = "tmp                      ";
int if_num = 0, else_num = 0,while_num = 0,case_num = 0,out_loop_num = 0;
int tmp_num = 0;
char str[MAX_ID_LENGTH] = "str                      ";
int str_num = 0;
char string[MAX_STRING][MAX_ID_LENGTH];
void new_str() {
	int i = str_num, begin;
	int digit[10], bit = 0;
	do {
		digit[bit] = i % 10;
		i = i / 10;
		bit++;
	} while (i != 0);
	for (begin = 3, bit = bit - 1; (bit >= 0) && (begin <= 9); bit--, begin++) {
		str[begin] = digit[bit] + '0';
	}
	CpyString(string[str_num], str, 10);
	str_num++;
}
void new_tmp_name() {
	int i = tmp_num, begin;
	int digit[10], bit = 0;
	do {
		digit[bit] = i % 10;
		i = i / 10;
		bit++;
	} while (i != 0);
	tmp_num++;
	for (begin = 3, bit = bit - 1; (bit >= 0) && (begin <= 9); bit--, begin++) {
		tmp_name[begin] = digit[bit] + '0';
	}
}
void new_case_label() {
	int i = case_num, begin;
	int digit[10], bit = 0;
	do {
		digit[bit] = i % 10;
		i = i / 10;
		bit++;
	} while (i != 0);
	case_num++;
	for (begin = 6, bit = bit - 1; (bit >= 0) && (begin <= 9); bit--, begin++) {
		case_label[begin] = digit[bit] + '0';
	}
}
void new_out_loop_label() {
	int i = out_loop_num, begin;
	int digit[10], bit = 0;
	do {
		digit[bit] = i % 10;
		i = i / 10;
		bit++;
	} while (i != 0);
	out_loop_num++;
	for (begin = 6, bit = bit - 1; (bit >= 0) && (begin <= 9); bit--, begin++) {
		out_loop_label[begin] = digit[bit] + '0';
	}
}
void new_while_label() {
	int i = while_num, begin;
	int digit[10], bit = 0;
	do {
		digit[bit] = i % 10;
		i = i / 10;
		bit++;
	} while (i != 0);
	while_num++;
	for (begin = 6, bit = bit - 1; (bit >= 0) && (begin <= 9); bit--, begin++) {
		while_label[begin] = digit[bit] + '0';
	}
}
void new_if_label() {
	int i = if_num,begin;
	int digit[10], bit = 0;
	do {
		digit[bit] = i % 10;
		i = i / 10;
		bit++;
	} while (i != 0);
	if_num++;
	for (begin = 6, bit = bit - 1; (bit >= 0) && (begin <= 9); bit--, begin++) {
		if_label[begin] = digit[bit] + '0';
	}
}
void new_else_label() {
	int i = else_num, begin;
	int digit[10], bit = 0;
	do {
		digit[bit] = i % 10;
		i = i / 10;
		bit++;
	} while (i != 0);
	else_num++;
	for (begin = 6, bit = bit - 1; (bit >= 0) && (begin <= 9); bit--, begin++) {
		else_label[begin] = digit[bit] + '0';
	}
}
void random_name(int n) {
	int i = exp_num, j = term_num, k = factor_num, begin;
	int digit[10],bit = 0;
	switch (n)
	{
	case EXPNAME:
		do {
			digit[bit] = i % 10;
			i = i / 10;
			bit++;
		} while (i != 0);
		exp_num++;
		for (begin = 3,bit = bit - 1; (bit >= 0)&&(begin <= 9); bit--, begin++) {
			exp_name[begin] = digit[bit] + '0';
		}
		break;
	case TERMNAME:
		do {
			digit[bit] = j % 10;
			j = j / 10;
			bit++;
		} while (j != 0);
		term_num++;
		for (begin = 4, bit = bit - 1; (bit >= 0) && (begin <= 9); bit--, begin++) {
			term_name[begin] = digit[bit] + '0';
		}
		break;
	default:
		do {
			digit[bit] = k % 10;
			k = k / 10;
			bit++;
		} while (k != 0);
		factor_num++;
		for (begin = 4, bit = bit - 1; (bit >= 0) && (begin <= 9); bit--, begin++) {
			factor_name[begin] = digit[bit] + '0';
		}
	}
}
void num2str(int num) {
	CpyString(num10, op[EMPTY], MAX_OP_LEN);
	int i = 0;
	int j;
	char ch;
	int neg = 0;
	
	if (num < 0) {
		num = num * (-1);
		neg = 1;
	}
		
	do {
		num10[i] = num % 10 + '0';
		num = num / 10;
		i++;
	} while (num != 0);
	if (neg) {
		num10[i] = '-';
		i++;
	}
	for (j = i - 1,i = 0; j > i; j--, i++) {
		ch = num10[j];
		num10[j] = num10[i];
		num10[i] = ch;
	}
}
/*Grammer:program
   ［＜常量说明＞］［＜变量说明＞］{＜有返回值函数定义＞|＜无返回值函数定义＞}＜主函数＞
*/
void program() {
	emit(op[GLBSTR], op[EMPTY], op[EMPTY], op[EMPTY]);
	if (CmpString(SymType, SYM[CONSTSY].symbol, MAX_SYM_LENGTH)) {
		ConDecl();
	}
	if (CmpString(SymType, SYM[INTSY].symbol, MAX_SYM_LENGTH)|
		CmpString(SymType, SYM[CHARSY].symbol, MAX_SYM_LENGTH)) {
		VarDecl();
	}
	emit(op[GLBEND], op[EMPTY], op[EMPTY], op[EMPTY]);
	while (CmpString(SymType, SYM[INTSY].symbol, MAX_SYM_LENGTH) |
		CmpString(SymType, SYM[CHARSY].symbol, MAX_SYM_LENGTH) |
		CmpString(SymType, SYM[VOIDSY].symbol, MAX_SYM_LENGTH)|
		CmpString(SymType, SYM[LPARENT].symbol, MAX_SYM_LENGTH)) {
		if (CmpString(SymType, SYM[INTSY].symbol, MAX_SYM_LENGTH) |
			CmpString(SymType, SYM[CHARSY].symbol, MAX_SYM_LENGTH)|
			CmpString(SymType, SYM[LPARENT].symbol, MAX_SYM_LENGTH)) {
			FuncDecl();
		}
		else {
			VoidFuncDecl();
		}
	}
	MainDecl();
}
/*Grammer:Constant Declaration
	＜常量说明＞ ::=  const＜常量定义＞;{ const＜常量定义＞;}
*/
void ConDecl() {
	CpyString(itemkind, "CONSTANT                 ", MAX_SYM_LENGTH);
	if (CmpString(SymType, SYM[CONSTSY].symbol, MAX_SYM_LENGTH)) {
		InSym();
		ConDef();
		if (CmpString(SymType, SYM[SEMICOLON].symbol, MAX_SYM_LENGTH)) {
			//is semicolon
			InSym();
			while (CmpString(SymType, SYM[CONSTSY].symbol, MAX_SYM_LENGTH)) {
				InSym();
				ConDef();
				if (CmpString(SymType, SYM[SEMICOLON].symbol, MAX_SYM_LENGTH))
					InSym();
				else {
					errormsg(MISS_SEMICOLON);
					errorhandle();
					return;
				}
			}
		}
		else {
			errormsg(MISS_SEMICOLON);//not semicolon,error.
			errorhandle();
			return;
		}
	}
	else {
		//not const:error!
		errormsg(MISS_CONST);
		errorhandle();
		return;
	}
}
/*Grammer:Constant Defination
＜常量定义＞   ::=   int＜标识符＞＝＜整数＞{,＜标识符＞＝＜整数＞}
					| char＜标识符＞＝＜字符＞{,＜标识符＞＝＜字符＞}
					*/
void ConDef() {
	int neg = 0;
	if (CmpString(SymType, SYM[INTSY].symbol, MAX_SYM_LENGTH)) {//This is a int constant
		InSym();
		CpyString(itemtype, "INT                      ", MAX_SYM_LENGTH);
		if (CmpString(SymType, SYM[IDENT].symbol, MAX_SYM_LENGTH)) {
			CpyString(itemname, StringValue, MAX_SYM_LENGTH);
			InSym();
			if (CmpString(SymType, SYM[BECOME].symbol, MAX_SYM_LENGTH)) {
				InSym();
				if (CmpString(SymType, SYM[PLUS].symbol, MAX_SYM_LENGTH) |
					CmpString(SymType, SYM[MINUS].symbol, MAX_SYM_LENGTH)) {
					if (CmpString(SymType, SYM[MINUS].symbol, MAX_SYM_LENGTH))
						neg = 1;
					InSym();
				}
				if (CmpString(SymType, SYM[INTCON].symbol, MAX_SYM_LENGTH)) {
					if (neg) {
						NumValue = (-1) * NumValue;
						neg = 0;
					}
					itemvalue = NumValue;
					num2str(itemvalue);
					if (entertable()) {
						errormsg(SYM_REDEFINE);
						errorhandle();
						return;
					}
					emit(op[CON],itemname,itemtype,num10);
					InSym();
					while (CmpString(SymType, SYM[COMMA].symbol, MAX_SYM_LENGTH)) {
						InSym();
						if (CmpString(SymType, SYM[IDENT].symbol, MAX_SYM_LENGTH)) {
							CpyString(itemname, StringValue, MAX_SYM_LENGTH);
							InSym();
							if (CmpString(SymType, SYM[BECOME].symbol, MAX_SYM_LENGTH)){
								InSym();
								if (CmpString(SymType, SYM[PLUS].symbol, MAX_SYM_LENGTH) |
									CmpString(SymType, SYM[MINUS].symbol, MAX_SYM_LENGTH)) {
									if (CmpString(SymType, SYM[MINUS].symbol, MAX_SYM_LENGTH))
										neg = 1;
									InSym();
								}
								if (CmpString(SymType, SYM[INTCON].symbol, MAX_SYM_LENGTH)) {
									if (neg) {
										NumValue = (-1) * NumValue;
										neg = 0;
									}
									itemvalue = NumValue;
									num2str(itemvalue);
									if (entertable()) {
										errormsg(SYM_REDEFINE);
										errorhandle();
										return;
									}
									emit(op[CON], itemname, itemtype, num10);
									InSym();
									continue;
								}
								else {
									errormsg(MISS_VALUE);
									errorhandle();
									return;
								}
							}
							else {
								errormsg(MISS_BECOME);
								errorhandle();
								return;
							}
						}
						else {
							errormsg(MISS_IDENT);
							errorhandle();
							return;
						}
					}
					//printf("%d This is a Int Constant defination sentence!\n",ll-1);
					return;
				}
				else {
					errormsg(MISS_VALUE);
					errorhandle();
					return;
				}
			}
			else {
				errormsg(MISS_BECOME);
				errorhandle();
				return;
			}
		}
		else {
			errormsg(MISS_IDENT);
			errorhandle();
			return;
		}
	}
	else if (CmpString(SymType, SYM[CHARSY].symbol, MAX_SYM_LENGTH)) {//This is a char constant
		CpyString(itemtype, "CHAR                     ", MAX_SYM_LENGTH);
		InSym();
		if (CmpString(SymType, SYM[IDENT].symbol, MAX_SYM_LENGTH)) {
			CpyString(itemname, StringValue, MAX_SYM_LENGTH);
			InSym();
			if (CmpString(SymType, SYM[BECOME].symbol, MAX_SYM_LENGTH)) {
				InSym();
				if (CmpString(SymType, SYM[CHARCON].symbol, MAX_SYM_LENGTH)) {
					itemvalue = (int)StringValue[0];
					emit(op[CON], itemname, itemtype, StringValue);
					InSym();
					entertable();
					while (CmpString(SymType, SYM[COMMA].symbol, MAX_SYM_LENGTH)) {
						InSym();
						if (CmpString(SymType, SYM[IDENT].symbol, MAX_SYM_LENGTH)) {
							CpyString(itemname, StringValue, MAX_SYM_LENGTH);
							InSym();
							if (CmpString(SymType, SYM[BECOME].symbol, MAX_SYM_LENGTH)) {
								InSym();
								if (CmpString(SymType, SYM[CHARCON].symbol, MAX_SYM_LENGTH)) {
									itemvalue = (int)StringValue[0];
									emit(op[CON], itemname, itemtype, StringValue);
									entertable();
									InSym();
									continue;
								}
								else {
									errormsg(MISS_VALUE);
									errorhandle();
									return;
								}
							}
							else {
								errormsg(MISS_BECOME);
								errorhandle();
								return;
							}
						}
						else {
							errormsg(MISS_IDENT);
							errorhandle();
							return;
						}
					}
					//printf("%d This is a Char Constant defination sentence!\n",ll-1);
					return;
				}
				else {
					errormsg(MISS_VALUE);
					errorhandle();
					return;
				}
			}
			else {
				errormsg(MISS_BECOME);
				errorhandle();
				return;
			}
		}
		else {
			errormsg(MISS_IDENT);
			errorhandle();
			return;
		}
	}
	else {
		//printf("This is not a Constant Defination Statement!\n");
		errormsg(MISS_TYPE);
		errorhandle();
		return;
	}
}
/*Grammer:DeclHead
	＜声明头部＞   ::=  ＜类型标识符>＜标识符＞
*/
void DeclHead() {
	if ((CmpString(SymType, SYM[CHARSY].symbol, MAX_SYM_LENGTH)) ||
		(CmpString(SymType, SYM[INTSY].symbol, MAX_SYM_LENGTH))) {
		if (CmpString(SymType, SYM[CHARSY].symbol, MAX_SYM_LENGTH))
			CpyString(itemtype, "CHAR                     ", MAX_SYM_LENGTH);
		else
			CpyString(itemtype, "INT                      ", MAX_SYM_LENGTH);
		InSym();
		if (CmpString(SymType, SYM[IDENT].symbol, MAX_SYM_LENGTH)) {
			CpyString(itemname, StringValue, MAX_SYM_LENGTH);
			Judged = 1;
			InSym();
			return;
		}
		else {
			errorhandle(MISS_IDENT);
			errorhandle();
			return;
		}
	}
	else {
		errorhandle(MISS_TYPE);
		errorhandle();
	}
}
/*Grammer:Variable Declaration
	＜变量说明＞  ::= ＜变量定义＞;{＜变量定义＞;}
	*/
void VarDecl() {
	VarDef();
	if (CmpString(SymType, SYM[LPARENT].symbol, MAX_SYM_LENGTH) ||
		CmpString(SymType, SYM[LBRACE].symbol, MAX_SYM_LENGTH)) {
		return;
	}
	if (CmpString(SymType, SYM[SEMICOLON].symbol, MAX_SYM_LENGTH)) {
		InSym();
		while (CmpString(SymType, SYM[INTSY].symbol, MAX_SYM_LENGTH) ||
			CmpString(SymType, SYM[CHARSY].symbol, MAX_SYM_LENGTH)) {
			VarDef();
			if (pos == 0) {
				if (CmpString(SymType, SYM[LPARENT].symbol, MAX_SYM_LENGTH) ||
					CmpString(SymType, SYM[LBRACE].symbol, MAX_SYM_LENGTH)) {
					return;
				}
			}
			if (!CmpString(SymType, SYM[SEMICOLON].symbol, MAX_SYM_LENGTH)) {
				errormsg(BADEND_VARDCL);
				errorhandle();
				return;
			}
			InSym();
		}
		return;
	}
	else return;
}
/*Grammer:Variable Defination
＜变量定义＞  ::= 
＜类型标识符＞(＜标识符＞|＜标识符＞‘[’＜无符号整数＞‘]’){,(＜标识符＞|＜标识符＞‘[’＜无符号整数＞‘]’) }
*/
void VarDef() {
	if (Judged) {
		Judged = 0;//refresh Judge
		if (CmpString(SymType, SYM[LBRACK].symbol, MAX_SYM_LENGTH)) {
			InSym();
			if (CmpString(SymType, SYM[INTCON].symbol, MAX_SYM_LENGTH)) {
				if (NumValue <= 0) {//array dimension less than 0.
					errorhandle(ILLEGAL_DIM);
					errorhandle();
					return;
				}
				itemnumber = NumValue;
				num2str(itemnumber);
				emit(op[ARR], itemname, itemtype, num10);
				InSym();
				if (CmpString(SymType, SYM[RBRACK].symbol, MAX_SYM_LENGTH)) {
					InSym();
				}
				else {
					errorhandle(MISS_RBRACK);
					errorhandle();
					return;
				}
			}
			else {
				errorhandle(MISS_CONST);
				errorhandle();
				return;
			}
		}
		else {
			emit(op[VAR], itemname, itemtype, op[EMPTY]);
		}
		if (entertable()) {
			errormsg(SYM_REDEFINE);
			errorhandle();
			return;
		}
		while (CmpString(SymType, SYM[COMMA].symbol, MAX_SYM_LENGTH)) {
			InSym();
			if (CmpString(SymType, SYM[IDENT].symbol, MAX_SYM_LENGTH)) {
				CpyString(itemname, StringValue, MAX_SYM_LENGTH);
				InSym();
				if (CmpString(SymType, SYM[LBRACK].symbol, MAX_SYM_LENGTH)) {
					InSym();
					if (CmpString(SymType, SYM[INTCON].symbol, MAX_SYM_LENGTH)) {
						if (NumValue <= 0) {//array dimension less than 0.
							errorhandle(ILLEGAL_DIM);
							errorhandle();
							return;
						}
						itemnumber = NumValue;
						num2str(itemnumber);
						emit(op[ARR], itemname, itemtype, num10);
						InSym();
						if (CmpString(SymType, SYM[RBRACK].symbol, MAX_SYM_LENGTH)) {
							InSym();
						}
						else {
							errorhandle(MISS_RBRACK);
							errorhandle();
							return;
						}
					}
					else {
						errorhandle(MISS_CONST);
						errorhandle();
						return;
					}
				}
				else {
					emit(op[VAR], itemname, itemtype, op[EMPTY]);
				}
				if (entertable()) {
					errormsg(SYM_REDEFINE);
					errorhandle();
					return;
				}
			}
			else {
				errorhandle(MISS_IDENT);
				errorhandle();
				return;
			}
		}
		//("%d This is a Variable Defination sentence!\n",ll-1);
		return;
	}
	else {
		DeclHead();
		if (CmpString(SymType, SYM[LPARENT].symbol, MAX_SYM_LENGTH) ||
			CmpString(SymType, SYM[LBRACE].symbol, MAX_SYM_LENGTH)) {
			CpyString(itemkind, "FUNCTION                 ", MAX_SYM_LENGTH);
			return;
		}
		else {
			CpyString(itemkind, "VARIABLE                 ", MAX_SYM_LENGTH);
			VarDef();
			return;
		}
	}
}
/*Grammer:paralist
	paralist:＜类型标识符＞＜标识符＞{,＜类型标识符＞＜标识符＞}
*/
void paralist() {
	DeclHead();
	CpyString(itemkind, "PARA                     ", MAX_SYM_LENGTH);
	entertable();
	funcpara++;
	while (CmpString(SymType, SYM[COMMA].symbol, MAX_SYM_LENGTH)) {
		InSym();
		DeclHead();
		CpyString(itemkind, "PARA                     ", MAX_SYM_LENGTH);
		if (entertable()) {
			errormsg(SYM_REDEFINE);
			errorhandle();
			return;
		}
		funcpara++;
	}
	Judged = 0;
	return;
}
/*Grammer:FuncDecl
	＜有返回值函数定义＞::=＜声明头部＞‘(’＜参数表＞‘)’ ‘{’＜复合语句＞‘}’|＜声明头部＞‘{’＜复合语句＞‘}’  
	第一种选择为有参数的情况，第二种选择为无参数的情况
*/
void FuncDecl() {
	int no_para = 1;
	if (Judged) {
		Judged = 0;
		if (entertable()) {
			errormsg(SYM_REDEFINE);
			errorhandle();
			return;
		}
		emit(op[FUNC], itemname, itemtype, op[EMPTY]);
		if (CmpString(SymType, SYM[LPARENT].symbol, MAX_SYM_LENGTH)) {
			InSym();
			paralist();
			no_para = 0;
			C0_SymTable.item[C0_SymTable.top - funcpara - 1].number = funcpara;
			funcpara = 0;
			if (CmpString(SymType, SYM[RPARENT].symbol, MAX_SYM_LENGTH)) {
				InSym();
			}
			else {
				errorhandle(MISS_RPARENT);
				errorhandle();
				return;
			}
		}
		if (CmpString(SymType, SYM[LBRACE].symbol, MAX_SYM_LENGTH)) {
			InSym();
			if (no_para) {
				C0_SymTable.item[C0_SymTable.top - funcpara - 1].number = funcpara;
				funcpara = 0;
			}			
			if (CmpString(SymType, SYM[CONSTSY].symbol, MAX_SYM_LENGTH)) {
				ConDecl();
			}
			if (CmpString(SymType, SYM[INTSY].symbol, MAX_SYM_LENGTH) |
				CmpString(SymType, SYM[CHARSY].symbol, MAX_SYM_LENGTH)) {
				pos = 1;
				VarDecl();
			}
			while (!CmpString(SymType, SYM[RBRACE].symbol, MAX_SYM_LENGTH)) {
				statement();
			}
			if (CmpString(SymType, SYM[RBRACE].symbol, MAX_SYM_LENGTH)) {
				//printf("%d This is a Function Declaration Sentence!\n",ll-1);
				InSym();
				emit(op[END], op[EMPTY], op[EMPTY], op[EMPTY]);
				return_flag = 0;
				setfunsize();
				return;
			}
			else {
				errorhandle(MISS_RBRACE);
				errorhandle();
				return;
			}
		}
		else {
			errorhandle(MISS_LBRACE);
			errorhandle();
			return;
		}
	}
	else {
		DeclHead();
		if (!CmpString(SymType, SYM[LPARENT].symbol, MAX_SYM_LENGTH) &&
			!CmpString(SymType, SYM[LBRACE].symbol, MAX_SYM_LENGTH)) {
			VarDef();
		}
		else {
			CpyString(itemkind, "FUNCTION                 ", MAX_SYM_LENGTH);
			FuncDecl();
			return;
		}
	}
}
/*Grammer:VoidFuncDecl
＜无返回值函数定义＞  ::= void＜标识符＞'(’＜参数表＞‘)’‘{’＜复合语句＞‘}’| void＜标识符＞'{’＜复合语句＞‘}’
第一种选择为有参数的情况，第二种选择为无参数的情况
*/
void VoidFuncDecl() {
	if (Judged) {
		Judged = 0;
		//whether there is a lparent
		if (CmpString(SymType, SYM[LPARENT].symbol, MAX_SYM_LENGTH)) {
			InSym();
			paralist();
			C0_SymTable.item[C0_SymTable.top - funcpara - 1].number = funcpara;
			funcpara = 0;
			if (CmpString(SymType, SYM[RPARENT].symbol, MAX_SYM_LENGTH)) {
				InSym();
			}
			else {
				errorhandle(MISS_RPARENT);
				errorhandle();
				return;
			}
		}
		if (CmpString(SymType, SYM[LBRACE].symbol, MAX_SYM_LENGTH)) {
			InSym();
			if (CmpString(SymType, SYM[CONSTSY].symbol, MAX_SYM_LENGTH)) {
				ConDecl();
			}
			if (CmpString(SymType, SYM[INTSY].symbol, MAX_SYM_LENGTH) |
				CmpString(SymType, SYM[CHARSY].symbol, MAX_SYM_LENGTH)) {
				pos = 1;
				VarDecl();
			}
			while (!CmpString(SymType, SYM[RBRACE].symbol, MAX_SYM_LENGTH)) {
				statement();
			}
			if (CmpString(SymType, SYM[RBRACE].symbol, MAX_SYM_LENGTH)) {
				//printf("%d This is a Function Declaration Sentence!\n",ll-1);
				InSym();
				emit(op[END], op[EMPTY], op[EMPTY], op[EMPTY]);
				return_flag = 0;
				setfunsize();
				return;
			}
			else {
				errorhandle(MISS_RBRACE);
				errorhandle();
				return;
			}
		}
		else {
			errorhandle(MISS_LBRACE);
			errorhandle();
			return;
		}
		
	}
	else {
		if (CmpString(SymType, SYM[VOIDSY].symbol, MAX_SYM_LENGTH)) {
			CpyString(itemtype, "VOID                     ", MAX_SYM_LENGTH);
			InSym();
			Judged = 1;
			CpyString(itemname, StringValue, MAX_SYM_LENGTH);
			CpyString(itemkind, "FUNCTION                 ", MAX_SYM_LENGTH);
			if (CmpString(StringValue, SYM[MAIN].expression, MAX_SYM_LENGTH)) {
				itemnumber = 0;
				emit(op[FUNC], itemname, itemtype, op[EMPTY]);
				if (entertable()) {
					errormsg(SYM_REDEFINE);
					errorhandle();
					return;
				}
				return;
			}
			else {
				itemnumber = 0;
				emit(op[FUNC], itemname, itemtype, op[EMPTY]);
				if (entertable()) {
					errormsg(SYM_REDEFINE);
					errorhandle();
					return;
				}
				InSym();
				VoidFuncDecl();
				return ;
			}
		}
		else {
			errormsg(MISS_VOID);
			errorhandle();
			return;
		}
	}
}
/*Grammer:MainDecl
＜主函数＞    ::= void main‘(’‘)’ ‘{’＜复合语句＞‘}’
*/
void MainDecl() {
	if (Judged) {
		Judged = 0;
		InSym();
		if (CmpString(SymType, SYM[LPARENT].symbol, MAX_SYM_LENGTH)) {
			InSym();
			if (CmpString(SymType, SYM[RPARENT].symbol, MAX_SYM_LENGTH)) {
				InSym();
				if (CmpString(SymType, SYM[LBRACE].symbol, MAX_SYM_LENGTH)) {
					InSym();
					if (CmpString(SymType, SYM[CONSTSY].symbol, MAX_SYM_LENGTH)) {
						ConDecl();
					}
					if (CmpString(SymType, SYM[INTSY].symbol, MAX_SYM_LENGTH) |
						CmpString(SymType, SYM[CHARSY].symbol, MAX_SYM_LENGTH)) {
						pos = 1;
						VarDecl();
					}
					while (!CmpString(SymType, SYM[RBRACE].symbol, MAX_SYM_LENGTH)) {
						statement();
					}
					if (CmpString(SymType, SYM[RBRACE].symbol, MAX_SYM_LENGTH)) {
						InSym();
						emit(op[END], op[EMPTY], op[EMPTY], op[EMPTY]);
						//printf("%d This is a main function sentence!\n",ll-1);
						setfunsize();
						return;
					}
					else {
						errormsg(MISS_RBRACE);
						errorhandle();
						return;
					}
				}
				else {
					errormsg(MISS_LBRACE);
					errorhandle();
					return;
				}
			}
			else {
				errormsg(MISS_RPARENT);
				errorhandle();
				return;
			}
		}
		else {
			errormsg(MISS_LPARENT);
			errorhandle();
			return;
		}
	}
	else {
		if (CmpString(SymType, SYM[VOIDSY].symbol, MAX_SYM_LENGTH)) {
			InSym();
			Judged = 1;
			if (CmpString(StringValue, SYM[MAIN].expression, MAX_SYM_LENGTH)) {
				MainDecl();
				return;
			}
			else {
				InSym();
				VoidFuncDecl();
				return;
			}
		}
	}
}
/*Grammer:statement
＜条件语句＞｜＜循环语句＞| ‘{’＜语句列＞‘}’｜<情况语句>|＜有返回值函数调用语句＞; |＜无返回值函数调用语句＞;｜
＜赋值语句＞;｜＜读语句＞;｜＜写语句＞;｜＜空＞;｜＜返回语句＞;
*/
void statement() {
	if (CmpString(SymType, SYM[IFSY].symbol, MAX_SYM_LENGTH)) {
		ifstate();
		//printf("%d This is a If statement!\n",ll-1);
	}
	else if (CmpString(SymType, SYM[DOSY].symbol, MAX_SYM_LENGTH)) {
		dowhilestate();
		//printf("%d This is a dowhile statement!\n",ll-1);
	}
	else if (CmpString(SymType, SYM[SWITCHSY].symbol, MAX_SYM_LENGTH)) {
		switchstate();
		//printf("%d This is a Switch statement!\n",ll-1);
	}
	else if (CmpString(SymType, SYM[SCANFSY].symbol, MAX_SYM_LENGTH)) {
		scanfstate();
		if (CmpString(SymType, SYM[SEMICOLON].symbol, MAX_SYM_LENGTH)) {
			//printf("%d This is a Scanf statement!\n",ll-1);
			InSym();
			return;
		}
		else {
			errormsg(MISS_SEMICOLON);
			errorhandle();
			return;
		}
	}
	else if (CmpString(SymType, SYM[PRINTFSY].symbol, MAX_SYM_LENGTH)) {
		printfstate();
		if (CmpString(SymType, SYM[SEMICOLON].symbol, MAX_SYM_LENGTH)) {
			//printf("%d This is a Printf statement!\n",ll-1);
			InSym();
			return;
		}
		else {
			errormsg(MISS_SEMICOLON);
			errorhandle();
			return;
		}
	}
	else if (CmpString(SymType, SYM[RETURNSY].symbol, MAX_SYM_LENGTH)) {
		returnstate();
		if (CmpString(SymType, SYM[SEMICOLON].symbol, MAX_SYM_LENGTH)) {
			//printf("%d This is a Return statement!\n",ll-1);
			InSym();
			return;
		}
		else {
			errormsg(MISS_SEMICOLON);
			errorhandle();
			return;
		}
	}
	else if (CmpString(SymType, SYM[SEMICOLON].symbol, MAX_SYM_LENGTH)) {
		//printf("%d This is a empty statement!\n",ll-1);
		InSym();
	}
	else if (CmpString(SymType, SYM[LBRACE].symbol, MAX_SYM_LENGTH)) {
		InSym();
		while (!CmpString(SymType, SYM[RBRACE].symbol, MAX_SYM_LENGTH)) {
			statement();
		}
		InSym();
	}
	else if (CmpString(SymType, SYM[IDENT].symbol, MAX_SYM_LENGTH)) {
		CpyString(left, StringValue, MAX_ID_LENGTH);
		InSym();
		if (CmpString(SymType, SYM[BECOME].symbol, MAX_SYM_LENGTH)|
			CmpString(SymType, SYM[LBRACK].symbol, MAX_SYM_LENGTH)) {
			assignstate();
			if (CmpString(SymType, SYM[SEMICOLON].symbol, MAX_SYM_LENGTH)) {
				InSym();
				//printf("%d This is an Assign statement!\n",ll-1);
				return;
			}
			else {
				errormsg(MISS_SEMICOLON);
				errorhandle();
				return;
			}
		}
		else if (CmpString(SymType, SYM[LPARENT].symbol, MAX_SYM_LENGTH)|
					CmpString(SymType, SYM[SEMICOLON].symbol, MAX_SYM_LENGTH)) {
			callstate(left);
			if (CmpString(SymType, SYM[SEMICOLON].symbol, MAX_SYM_LENGTH)) {
				InSym();
				//printf("%d This is a calling statement!\n",ll-1);
				return;
			}
			else {
				errormsg(MISS_SEMICOLON);
				errorhandle();
				return;
			}
		}
		else {
			errormsg(BAD_STATEMENT_HEAD);
			errorhandle();
			return;
		}
	}
}
/*Grammer:expression
＜表达式＞::= ［＋｜－］＜项＞{＜加法运算符＞＜项＞}  //[+|-]只作用于第一个<项>
*/
void expression() {
	int neg = 0;
	char tmp[MAX_ID_LENGTH];
	if (CmpString(SymType, SYM[PLUS].symbol, MAX_SYM_LENGTH) |
		CmpString(SymType, SYM[MINUS].symbol, MAX_SYM_LENGTH)) {
		if (CmpString(SymType, SYM[MINUS].symbol, MAX_SYM_LENGTH))
			neg = 1;
		InSym();
	}
	term();
	CpyString(tmp, term_name,  MAX_SYM_LENGTH);
	if (neg)
		emit(op[NEG], tmp , tmp, op[EMPTY]);
	while (CmpString(SymType, SYM[PLUS].symbol, MAX_SYM_LENGTH) |
		CmpString(SymType, SYM[MINUS].symbol, MAX_SYM_LENGTH)) {
		if (CmpString(SymType, SYM[PLUS].symbol, MAX_SYM_LENGTH)) {
			InSym();
			term();
			emit(op[ADD], tmp, term_name, tmp);
		}
		else {
			InSym();
			term();
			emit(op[SUB], tmp, term_name, tmp);
		}
	}
	CpyString(exp_name, tmp, MAX_OP_LEN);
	return;
}
/*Grammer:term
	＜项＞::= ＜因子＞{＜乘法运算符＞＜因子＞}
*/
void term() {
	char tmp[MAX_ID_LENGTH];
	//CpyString(tmp1, exp_name, MAX_SYM_LENGTH);
	//CpyString(tmp2, term_name, MAX_SYM_LENGTH);
	//get a factor
	factor();
	CpyString(tmp, factor_name, MAX_OP_LEN);
	//CpyString(exp_name, tmp1,  MAX_SYM_LENGTH);
	//CpyString(term_name, tmp2,  MAX_SYM_LENGTH);//recover the rewritten term and exp.
	//emit(op[LODV], term_name, factor_name, op[EMPTY]);

	while (CmpString(SymType, SYM[TIMES].symbol, MAX_SYM_LENGTH) |
		CmpString(SymType, SYM[DIV].symbol, MAX_SYM_LENGTH)) {
		if (CmpString(SymType, SYM[TIMES].symbol, MAX_SYM_LENGTH)) {
			InSym();
			factor();
			emit(op[MUL], tmp, factor_name, tmp);
		}
		else {
			InSym();
			factor();
			//CpyString(exp_name, tmp1, MAX_SYM_LENGTH);
			//CpyString(term_name, tmp2, MAX_SYM_LENGTH);
			emit(op[DIVIDE], tmp, factor_name, tmp);
		}
	}
	CpyString(term_name, tmp, MAX_OP_LEN);
	return;
}
/*Grammer:factor
	＜因子＞ ::= ＜标识符＞｜＜标识符＞‘[’＜表达式＞‘]’｜＜整数＞|＜字符＞｜＜有返回值函数调用语句＞|‘(’＜表达式＞‘)’
*/
void factor() {
	int neg = 0,found;
	char name[MAX_ID_LENGTH];
	char tmp[MAX_ID_LENGTH];
	random_name(FACTORNAME);
	//CpyString(tmp1, exp_name, MAX_SYM_LENGTH);
	//CpyString(tmp2, term_name, MAX_SYM_LENGTH);
	CpyString(tmp, factor_name, MAX_SYM_LENGTH);
	if (CmpString(SymType, SYM[IDENT].symbol, MAX_SYM_LENGTH)) {
		CpyString(name, StringValue, MAX_SYM_LENGTH);
		found = find_sym(name);
		if (found == 0) {
			errormsg(ID_UNDEF);
			errorhandle();
			return;
		}
		InSym();
		if (CmpString(SymType, SYM[LBRACK].symbol, MAX_SYM_LENGTH)) {//array
			InSym();
			expression();
			if (!CmpString(SymType, SYM[RBRACK].symbol, MAX_SYM_LENGTH)) {
				errormsg(MISS_RBRACK);
				errorhandle();
				return;
			}
			else {
				emit(op[LODARR], name, exp_name, tmp);
				CpyString(factor_name,tmp, MAX_OP_LEN);
				CpyString(itemname, factor_name, MAX_SYM_LENGTH);
				CpyString(itemtype, C0_SymTable.item[found].type, MAX_SYM_LENGTH);
				CpyString(itemkind, C0_SymTable.item[found].kind, MAX_SYM_LENGTH);
				if (entertable()) {
					errormsg(SYM_REDEFINE);
					errorhandle();
					return;
				}
				InSym();
				return;
			}
		}
		else if (CmpString(SymType, SYM[LPARENT].symbol, MAX_SYM_LENGTH)) {//function
			callstate(name);//TODO?
			emit(op[LODR], tmp, "$v0                      ", op[EMPTY]);//get the return value.
			CpyString(factor_name, tmp, MAX_OP_LEN);
			CpyString(itemname, factor_name, MAX_SYM_LENGTH);
			CpyString(itemtype, C0_SymTable.item[found].type, MAX_SYM_LENGTH);
			CpyString(itemkind, "VAR                      ", MAX_SYM_LENGTH);
			if (entertable()) {
				errormsg(SYM_REDEFINE);
				errorhandle();
				return;
			}
			return;
		}
		if (CmpString("FUNCTION                 ", C0_SymTable.item[found].kind, MAX_SYM_LENGTH)) {
			callstate(name);//TODO?
			emit(op[LODR], tmp, "$v0                      ", op[EMPTY]);//get the return value.
			CpyString(factor_name, tmp, MAX_OP_LEN);
			CpyString(itemname, factor_name, MAX_SYM_LENGTH);
			CpyString(itemtype, C0_SymTable.item[found].type, MAX_SYM_LENGTH);
			CpyString(itemkind, "VARIABLE                 ", MAX_SYM_LENGTH);
			if (entertable()) {
				errormsg(SYM_REDEFINE);
				errorhandle();
				return;
			}
			return;
		}
		emit(op[LODV], tmp, name, op[EMPTY]);
		CpyString(factor_name, tmp, MAX_OP_LEN);
		CpyString(itemname, factor_name, MAX_SYM_LENGTH);
		CpyString(itemtype, C0_SymTable.item[found].type, MAX_SYM_LENGTH);
		CpyString(itemkind, C0_SymTable.item[found].kind, MAX_SYM_LENGTH);
		if (entertable()) {
			errormsg(SYM_REDEFINE);
			errorhandle();
			return;
		}
		return;
	}
	else if (CmpString(SymType, SYM[PLUS].symbol, MAX_SYM_LENGTH) |
		CmpString(SymType, SYM[MINUS].symbol, MAX_SYM_LENGTH)) {
		if (CmpString(SymType, SYM[MINUS].symbol, MAX_SYM_LENGTH)) 
			neg = 1;
		InSym();
		if (CmpString(SymType, SYM[INTCON].symbol, MAX_SYM_LENGTH)) {
			if (neg) {
				NumValue = (-1)*NumValue;
				neg = 0;
			}
			num2str(NumValue);
			emit(op[LODI], tmp, num10, op[EMPTY]);
			CpyString(factor_name, tmp, MAX_OP_LEN);
			CpyString(itemname, factor_name, MAX_SYM_LENGTH);
			CpyString(itemtype, "INT                      ", MAX_SYM_LENGTH);
			CpyString(itemkind, "VAR                      ", MAX_SYM_LENGTH);
			if (entertable()) {
				errormsg(SYM_REDEFINE);
				errorhandle();
				return;
			}
			InSym();
			return;
		}
		else {
			errormsg(SIGNAL_WITHOUT_NUM);
			errorhandle();
			return;
		}
	}
	else if (CmpString(SymType, SYM[INTCON].symbol, MAX_SYM_LENGTH)) {
		num2str(NumValue);
		emit(op[LODI], tmp, num10, op[EMPTY]);
		CpyString(factor_name, tmp, MAX_OP_LEN);
		CpyString(itemname, factor_name, MAX_SYM_LENGTH);
		CpyString(itemtype, "INT                      ", MAX_SYM_LENGTH);
		CpyString(itemkind, "VAR                      ", MAX_SYM_LENGTH);
		if (entertable()) {
			errormsg(SYM_REDEFINE);
			errorhandle();
			return;
		}
		InSym();
		return;
	}
	else if (CmpString(SymType, SYM[CHARCON].symbol, MAX_SYM_LENGTH)) {
		emit(op[LODI], tmp, StringValue, op[EMPTY]);
		CpyString(factor_name, tmp, MAX_OP_LEN);
		CpyString(itemname, factor_name, MAX_SYM_LENGTH);
		CpyString(itemtype, "CHAR                     ", MAX_SYM_LENGTH);
		CpyString(itemkind, "VAR                      ", MAX_SYM_LENGTH);
		if (entertable()) {
			errormsg(SYM_REDEFINE);
			errorhandle();
			return;
		}
		InSym();
		return;
	}
	else if (CmpString(SymType, SYM[LPARENT].symbol, MAX_SYM_LENGTH)) {
		InSym();
		expression();
		if (CmpString(SymType, SYM[RPARENT].symbol, MAX_SYM_LENGTH)) {
			emit(op[LODV], tmp, exp_name, op[EMPTY]);
			found = find_sym(exp_name);
			CpyString(factor_name, tmp, MAX_OP_LEN);
			CpyString(itemname, factor_name, MAX_SYM_LENGTH);
			CpyString(itemtype, C0_SymTable.item[found].type, MAX_SYM_LENGTH);
			CpyString(itemkind, C0_SymTable.item[found].kind, MAX_SYM_LENGTH);
			if (entertable()) {
				errormsg(SYM_REDEFINE);
				errorhandle();
				return;
			}
			InSym();
			return;
		}
		else {
			errormsg(MISS_RPARENT);
			errorhandle();
			return;
		}
	}
	else {
		errormsg(BAD_FACTOR_HEAD);
		errorhandle();
		return;
	}
}
/*Grammer:assignstate
＜赋值语句＞::=  ＜标识符＞＝＜表达式＞|＜标识符＞‘[’＜表达式＞‘]’=＜表达式＞
*/
void assignstate() {
	char tmp[MAX_ID_LENGTH];
	int found;
	if (CmpString(SymType, SYM[BECOME].symbol, MAX_SYM_LENGTH)) {
		InSym();
		expression();
		emit(op[LODV], left, exp_name, op[EMPTY]);
		found = find_sym(left);
		if (found == 0) {
			errormsg(ID_UNDEF);
			errorhandle();
			return;
		}
		if (!CmpString(C0_SymTable.item[found].kind, "VARIABLE                 ", MAX_ID_LENGTH) &&
			!CmpString(C0_SymTable.item[found].kind, "PARA                     ", MAX_ID_LENGTH)) {
			errormsg(ASSIGN_KIND);
			errorhandle();
			return;
		}
		return;
	}
	else if (CmpString(SymType, SYM[LBRACK].symbol, MAX_SYM_LENGTH)) {
		InSym();
		expression();
		CpyString(tmp, exp_name, MAX_OP_LEN);
		if (CmpString(SymType, SYM[RBRACK].symbol, MAX_SYM_LENGTH)) {
			InSym();
			if (CmpString(SymType, SYM[BECOME].symbol, MAX_SYM_LENGTH)) {
				InSym();
				expression();
				emit(op[STOARR], left, tmp, exp_name);
				return;
			}
			else {
				errormsg(MISS_BECOME);
				errorhandle();
				return;
			}
		}
		else {
			errormsg(MISS_RBRACK);
			errorhandle();
			return;
		}
	}
	else {
		errormsg(BAD_ASSIGN);
		errorhandle();
		return;
	}
}
/*Grammer:condition
＜条件＞    ::=  ＜表达式＞＜关系运算符＞＜表达式＞｜＜表达式＞ //表达式为0条件为假，否则为真
*/
void condition(char label[]) {
	char tmp[MAX_ID_LENGTH];
	expression();
	CpyString(tmp, exp_name, MAX_OP_LEN);
	if (CmpString(SymType, SYM[LSS].symbol, MAX_SYM_LENGTH)) {
		InSym();
		expression();
		emit(op[GQL], tmp, exp_name, label);
	}
	else if (CmpString(SymType, SYM[LEQ].symbol, MAX_SYM_LENGTH)) {
		InSym();
		expression();
		emit(op[GRT], tmp, exp_name, label);
	}
	else if (CmpString(SymType, SYM[GTR].symbol, MAX_SYM_LENGTH)) {
		InSym();
		expression();
		emit(op[LQL], tmp, exp_name,label);
	}
	else if (CmpString(SymType, SYM[GEQ].symbol, MAX_SYM_LENGTH)) {
		InSym();
		expression();
		emit(op[LESS], tmp, exp_name, label);
	}
	else if (CmpString(SymType, SYM[EQL].symbol, MAX_SYM_LENGTH)) {
		InSym();
		expression();
		emit(op[NQL], tmp, exp_name, label);
	}
	else if (CmpString(SymType, SYM[NEQ].symbol, MAX_SYM_LENGTH)) {
		InSym();
		expression();
		emit(op[EQU], tmp, exp_name, label);
	}
	else {
		emit(op[EQU], tmp, "0                        ", label);
	}
}
/*Grammer:ifstate
	＜条件语句＞  ::=  if ‘(’＜条件＞‘)’＜语句＞else <语句>
*/
void ifstate() {
	char iflabel[MAX_ID_LENGTH], elselabel[MAX_ID_LENGTH];
	new_if_label();
	new_else_label();
	CpyString(iflabel, if_label, MAX_OP_LEN);
	CpyString(elselabel, else_label, MAX_OP_LEN);
	if (CmpString(SymType, SYM[IFSY].symbol, MAX_SYM_LENGTH)) {
		InSym();
		if (CmpString(SymType, SYM[LPARENT].symbol, MAX_SYM_LENGTH)) {
			InSym();
			condition(elselabel);
			if (CmpString(SymType, SYM[RPARENT].symbol, MAX_SYM_LENGTH)) {
				InSym();
				statement();
				emit(op[JMP], iflabel,op[EMPTY],op[EMPTY]);
				if (CmpString(SymType, SYM[ELSESY].symbol, MAX_SYM_LENGTH)) {
					emit(op[LAB], elselabel, op[EMPTY], op[EMPTY]);
					InSym();
					statement();
					emit(op[LAB], iflabel, op[EMPTY], op[EMPTY]);
					return;
				}
				else {
					errormsg(MISS_ELSE);
					errorhandle();
					return;
				}
			}
			else {
				errormsg(MISS_RPARENT);
				errorhandle();
				return;
			}
		}
		else {
			errormsg(MISS_LPARENT);
			errorhandle();
			return;
		}
	}
	else {
		errormsg(MISS_IF);
		errorhandle();
		return;
	}
}
/*Grammer:dowhilestate
＜循环语句＞   ::=  do＜语句＞while ‘(’＜条件＞‘)’
*/
void dowhilestate() {
	char whilelabel[MAX_ID_LENGTH],outlooplabel[MAX_ID_LENGTH];
	new_while_label();
	new_out_loop_label();
	CpyString(whilelabel, while_label, MAX_OP_LEN);
	CpyString(outlooplabel, out_loop_label, MAX_OP_LEN);
	if (CmpString(SymType, SYM[DOSY].symbol, MAX_SYM_LENGTH)) {
		emit(op[LAB], whilelabel, op[EMPTY], op[EMPTY]);
		InSym();
		statement();
		if (CmpString(SymType, SYM[WHILESY].symbol, MAX_SYM_LENGTH)) {
			InSym();
			if (CmpString(SymType, SYM[LPARENT].symbol, MAX_SYM_LENGTH)) {
				InSym();
				condition(outlooplabel);
				if (CmpString(SymType, SYM[RPARENT].symbol, MAX_SYM_LENGTH)) {
					emit(op[JMP], whilelabel, op[EMPTY], op[EMPTY]);
					emit(op[LAB], outlooplabel, op[EMPTY], op[EMPTY]);
					InSym();
					return;
				}
				else {
					errormsg(MISS_RPARENT);
					errorhandle();
					return;
				}
			}
			else {
				errormsg(MISS_LPARENT);
				errorhandle();
				return;
			}
		}
		else {
			errormsg(MISS_WHILE);
			errorhandle();
			return;
		}
	}
	else {
		errormsg(MISS_DO);
		errorhandle();
		return;
	}
}
/*Grammer:casetable()
＜情况表＞   ::=  ＜情况子语句＞{＜情况子语句＞}
＜情况子语句＞  ::=  case＜常量＞：＜语句＞
*/
void casetable(char *defaultlabel) {
	int neg = 0;
	char strtmp[MAX_ID_LENGTH] = "                         ";
	char casei[MAX_ID_LENGTH], caselabel[MAX_ID_LENGTH],expname[MAX_ID_LENGTH];
	new_tmp_name();
	new_case_label();
	CpyString(expname, exp_name, MAX_OP_LEN);
	CpyString(caselabel, case_label, MAX_OP_LEN);
	CpyString(casei, tmp_name, MAX_OP_LEN);
	if (CmpString(SymType, SYM[CASESY].symbol, MAX_SYM_LENGTH)) {
		InSym();
		if (CmpString(SymType, SYM[PLUS].symbol, MAX_SYM_LENGTH) |
			CmpString(SymType, SYM[MINUS].symbol, MAX_SYM_LENGTH)) {
			if (CmpString(SymType, SYM[MINUS].symbol, MAX_SYM_LENGTH))
				neg = 1;
			InSym();
		}
		if (CmpString(SymType, SYM[INTCON].symbol, MAX_SYM_LENGTH) |
			CmpString(SymType, SYM[CHARCON].symbol, MAX_SYM_LENGTH)) {
			if (CmpString(SymType, SYM[INTCON].symbol, MAX_SYM_LENGTH)) {
				if (neg) {
					NumValue = (-1)*NumValue;
					neg = 0;
				}
				num2str(NumValue);
				emit(op[LODI], tmp_name, num10, op[EMPTY]);
				CpyString(itemtype, "INT                      ", MAX_SYM_LENGTH);
			}
			else {
				strtmp[0] = StringValue[0];
				emit(op[LODI], tmp_name, strtmp, op[EMPTY]);
				CpyString(itemtype, "CHAR                     ", MAX_SYM_LENGTH);
			}
			emit(op[NQL], tmp_name, expname, caselabel);
			CpyString(itemname, tmp_name, MAX_SYM_LENGTH);
			CpyString(itemkind, "VARIABLE                 ", MAX_SYM_LENGTH);
			if (entertable()) {
				errormsg(SYM_REDEFINE);
				errorhandle();
				return;
			}
			InSym();
			if (CmpString(SymType, SYM[COLON].symbol, MAX_SYM_LENGTH)) {
				InSym();
				statement();
				emit(op[JMP], defaultlabel, op[EMPTY], op[EMPTY]);
				emit(op[LAB], caselabel, op[EMPTY], op[EMPTY]);
				while (CmpString(SymType, SYM[CASESY].symbol, MAX_SYM_LENGTH)) {
					new_tmp_name();
					new_case_label();
					CpyString(caselabel, case_label, MAX_OP_LEN);
					CpyString(casei, tmp_name, MAX_OP_LEN);
					InSym();
					if (CmpString(SymType, SYM[INTCON].symbol, MAX_SYM_LENGTH) |
						CmpString(SymType, SYM[CHARCON].symbol, MAX_SYM_LENGTH)) {
						if (CmpString(SymType, SYM[INTCON].symbol, MAX_SYM_LENGTH)) {
							if (neg) {
								NumValue = (-1)*NumValue;
								neg = 0;
							}
							num2str(NumValue);
							emit(op[LODI], tmp_name, num10, op[EMPTY]); 
							CpyString(itemtype, "INT                      ", MAX_SYM_LENGTH);
						}
						else {
							strtmp[0] = StringValue[0];
							emit(op[LODI], tmp_name, strtmp, op[EMPTY]);
							CpyString(itemtype, "CHAR                     ", MAX_SYM_LENGTH);
						}
						emit(op[NQL], tmp_name, expname, caselabel);
						CpyString(itemname, tmp_name, MAX_SYM_LENGTH);
						CpyString(itemkind, "VARIABLE                 ", MAX_SYM_LENGTH);
						if (entertable()) {
							errormsg(SYM_REDEFINE);
							errorhandle();
							return;
						}
						InSym();
						if (CmpString(SymType, SYM[COLON].symbol, MAX_SYM_LENGTH)) {
							InSym();
							statement();
							emit(op[JMP], defaultlabel, op[EMPTY], op[EMPTY]);
							emit(op[LAB], caselabel, op[EMPTY], op[EMPTY]);
							continue;
						}
					}
					else {
						errormsg(MISS_CONST);
						errorhandle();
						return;
					}
				}
			}
			else {
				errormsg(MISS_COLON);
				errorhandle();
				return;
			}
		}
		else {
			errormsg(MISS_CONST);
			errorhandle();
			return;
		}
	}
	else {
		errormsg(MISS_CASE);
		errorhandle();
		return;
	}
}
/*Grammer:defaulttable:
＜缺省＞   ::=  default : ＜语句＞
*/
void defaulttable(char *defaultlabel) {
	if (CmpString(SymType, SYM[DEFAULTSY].symbol, MAX_SYM_LENGTH)) {
		InSym();
		if (CmpString(SymType, SYM[COLON].symbol, MAX_SYM_LENGTH)) {
			InSym();
			statement();
			emit(op[LAB], defaultlabel, op[EMPTY], op[EMPTY]);
			return;
		}
		else {
			errormsg(MISS_COLON);
			errorhandle();
			return;
		}
	}
	else {
		errormsg(MISS_DEFAULT);
		errorhandle();
		return;
	}
}
/*Grammer:switchstate
＜情况语句＞  ::=  switch ‘(’＜表达式＞‘)’ ‘{’＜情况表＞＜缺省＞ ‘}’
*/
void switchstate() {
	char defaultlabel[MAX_ID_LENGTH];
	new_case_label();
	CpyString(defaultlabel, case_label, MAX_OP_LEN);
	if (CmpString(SymType, SYM[SWITCHSY].symbol, MAX_SYM_LENGTH)) {
		InSym();
		if (CmpString(SymType, SYM[LPARENT].symbol, MAX_SYM_LENGTH)) {
			InSym();
			expression();
			if (CmpString(SymType, SYM[RPARENT].symbol, MAX_SYM_LENGTH)) {
				InSym();
				if (CmpString(SymType, SYM[LBRACE].symbol, MAX_SYM_LENGTH)) {
					InSym();
					casetable(defaultlabel);
					defaulttable(defaultlabel);
					if (CmpString(SymType, SYM[RBRACE].symbol, MAX_SYM_LENGTH)) {
						InSym();
						return;
					}
					else {
						errormsg(MISS_RBRACE);
						errorhandle();
						return;
					}
				}
				else {
					errormsg(MISS_LBRACE);
					errorhandle();
					return;
				}
			}
			else {
				errormsg(MISS_RPARENT);
				errorhandle();
				return;
			}
		}
		else {
			errormsg(MISS_LPARENT);
			errorhandle();
			return;
		}
	}
	else {
		errormsg(MISS_SWITCH);
		errorhandle();
		return;
	}
}
/*Grammer:printfstate
＜写语句＞ ::=  printf‘(’＜字符串＞,＜表达式＞‘)’|printf ‘(’＜字符串＞‘)’|printf ‘(’＜表达式＞‘)’
*/
void printfstate() {
	int printtype;
	if (CmpString(SymType, SYM[PRINTFSY].symbol, MAX_SYM_LENGTH)) {
		InSym();
		if (CmpString(SymType, SYM[LPARENT].symbol, MAX_SYM_LENGTH)) {
			InSym();
			if (CmpString(SymType, SYM[STRCON].symbol, MAX_SYM_LENGTH)) {
				InSym();
				if (CmpString(SymType, SYM[RPARENT].symbol, MAX_SYM_LENGTH)) {
					printtype = 1;
					num2str(printtype);
					new_str();
					emit(op[PRT], str, op[EMPTY], num10);
					InSym();
					return;
				}
				else if (CmpString(SymType, SYM[COMMA].symbol, MAX_SYM_LENGTH)) {
					InSym();
					expression();
					if (CmpString(SymType, SYM[RPARENT].symbol, MAX_SYM_LENGTH)) {
						printtype = 2;
						num2str(printtype);
						new_str();
						emit(op[PRT], str, exp_name, num10);
						InSym();
						return;
					}
					else {
						errormsg(MISS_RPARENT);
						errorhandle();
						return;
					}
				}
				else {
					errormsg(BAD_PRINT_PARA);
					errorhandle();
					return;
				}
			}
			else {
				expression();
				if (CmpString(SymType, SYM[RPARENT].symbol, MAX_SYM_LENGTH)) {
					printtype = 3;
					num2str(printtype);
					emit(op[PRT], op[EMPTY], exp_name, num10);
					InSym();
				}
				else {
					errormsg(MISS_RPARENT);
					errorhandle();
					return;
				}
			}
		}
		else {
			errormsg(MISS_LPARENT);
			errorhandle();
			return;
		}
	}
	else {
		errormsg(MISS_PRINTF);
		errorhandle();
		return;
	}
}
/*Grammer:scanfstate
＜读语句＞ ::=  scanf ‘(’＜标识符＞{,＜标识符＞}‘)’
*/
void scanfstate() {
	int found;
	if (CmpString(SymType, SYM[SCANFSY].symbol, MAX_SYM_LENGTH)) {
		InSym();
		if (CmpString(SymType, SYM[LPARENT].symbol, MAX_SYM_LENGTH)) {
			InSym();
			if (CmpString(SymType, SYM[IDENT].symbol, MAX_SYM_LENGTH)) {
				emit(op[SCF], StringValue, op[EMPTY], op[EMPTY]);
				found = find_sym(StringValue);
				if (found == 0) {
					errormsg(ID_UNDEF);
					errorhandle();
					return;
				}
				if (!CmpString(C0_SymTable.item[found].kind, "VARIABLE                 ", MAX_ID_LENGTH)||
					C0_SymTable.item[found].number!=0) {
					errormsg(SCF_ERROR);
					errorhandle();
					return;
				}
				InSym();
				while (CmpString(SymType, SYM[COMMA].symbol, MAX_SYM_LENGTH)) {
					InSym();
					if (CmpString(SymType, SYM[IDENT].symbol, MAX_SYM_LENGTH)) {
						emit(op[SCF], StringValue, op[EMPTY], op[EMPTY]);
						found = find_sym(StringValue);
						if (found == 0) {
							errormsg(ID_UNDEF);
							errorhandle();
							return;
						}
						if (!CmpString(C0_SymTable.item[found].kind, "VARIABLE                 ", MAX_ID_LENGTH) ||
							C0_SymTable.item[found].number != 0) {
							errormsg(SCF_ERROR);
							errorhandle();
							return;
						}
						InSym();
						continue;
					}
					else {
						errormsg(MISS_IDENT);
						errorhandle();
						return;
					}
				}
				if (CmpString(SymType, SYM[RPARENT].symbol, MAX_SYM_LENGTH)) {
					InSym();
					return;
				}
				else {
					errormsg(MISS_RPARENT);
					errorhandle();
					return;
				}
			}
			else {
				errormsg(MISS_IDENT);
				errorhandle();
				return;
			}
		}
		else {
			errormsg(MISS_LPARENT);
			errorhandle();
			return;
		}
	}
	else {
		errormsg(MISS_SCANF);
		errorhandle();
		return;
	}
}
/*Grammer:returnstate
＜返回语句＞   ::=  return[‘(’＜表达式＞‘)’]
*/
void returnstate() {
	int found;
	if (CmpString(SymType, SYM[RETURNSY].symbol, MAX_SYM_LENGTH)) {
		InSym();
		if (CmpString(SymType, SYM[LPARENT].symbol, MAX_SYM_LENGTH)) {
			InSym();
			expression();
			emit(op[RET], exp_name, op[EMPTY], op[EMPTY]);
			if (CmpString(SymType, SYM[RPARENT].symbol, MAX_SYM_LENGTH)) {
				InSym();
			}
			else {
				errormsg(MISS_RPARENT);
				errorhandle();
				return;
			}
		}
		else {
			emit(op[END], op[EMPTY], op[EMPTY], op[EMPTY]);
		}
		return_flag = 1;
	}
	else {
		errormsg(MISS_RETURN);
		errorhandle();
		return;
	}
}
/*Grammer:valuepara
＜值参数表＞   ::= ＜表达式＞{,＜表达式＞}*/
/*Grammer:call-1state
＜有返回值函数调用语句＞ ::= ＜标识符＞‘(’＜值参数表＞‘)’|<标识符>
*/
void callstate(char *func_name) {
	int counter = 1,found;
	char func[MAX_ID_LENGTH];
	CpyString(func, func_name, MAX_OP_LEN);
	found = find_sym(func);
	if (CmpString(SymType, SYM[LPARENT].symbol, MAX_SYM_LENGTH)) {
		InSym();
		expression();
		num2str(counter);
		emit(op[PARA], exp_name, num10, func);
		counter++;
		while (CmpString(SymType, SYM[COMMA].symbol, MAX_SYM_LENGTH)) {
			InSym();
			expression();
			num2str(counter);
			emit(op[PARA], exp_name, num10, func);
			counter++;
		}
		/*if ((counter - 1) != C0_SymTable.item[found].number) {
			errormsg(CALL_ERROR);
			errorhandle();
			return;
		}*/
		if (CmpString(SymType, SYM[RPARENT].symbol, MAX_SYM_LENGTH)) {
			InSym();
			emit(op[CALL], func,op[EMPTY],op[EMPTY]);
			return;
		}
		else {
			errormsg(MISS_RPARENT);
			errorhandle();
			return;
		}
	}
	else {
		emit(op[CALL], func, op[EMPTY], op[EMPTY]);
		return;
	}
	
}