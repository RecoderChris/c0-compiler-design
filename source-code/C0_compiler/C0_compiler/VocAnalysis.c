#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"compiler.h"

char ch = 0, SymType[MAX_SYM_LENGTH] = "                         ", StringValue[MAX_ID_LENGTH] = "                         ";
char *StringHead, *StringCur, *StringPrint;
int number = 0, NumValue = 0, EOF_FLAG = 0, ll = 1, cc = 1, StringLen = 0;
symb SYM[MAX_SYMBOL_TYPE];
int CmpString(char *tmp, char *src, int length) {
	int ArrayIndex;
	for (ArrayIndex = 0; ArrayIndex < length; ArrayIndex++) {
		if (tmp[ArrayIndex] != src[ArrayIndex])
			return 0;
	}
	return 1;
}
void CpyString(char *tmp, char *src, int length) {
	int ArrayIndex;
	for (ArrayIndex = 0; ArrayIndex < length; ArrayIndex++) {
		tmp[ArrayIndex] = src[ArrayIndex];
	}
}
/*IsLegalChar:
	Whether current ch is a legal ch for char type.
*/
int IsLegalChar() {
	int i;
	char chset[68] = { '+','-','*','/','0','1','2','3','4','5','6','7','8','9','_','a','b','c','d','e','f','g','h','i','j',
					'k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','A','B','C','D','E','F','G','H','I',
					'J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',',' };
	for (i = 0; i < 68; i++) {
		if (ch == chset[i])
			return 1;
	}
	return 0;
}
/*IsSingleTerminalSym:
	Whether current ch is a single terminal sym.
*/
int IsSingleTerminalSym() {
	int i;
	char TS[13] = { '+','-','*','/','[',']','(',')','{','}',',',';',':' };
	for (i = 0; i < 15; i++) {
		if (ch == TS[i])
			return 1;
	}
	return 0;
}
/*OutputToken(options):
	According to different type of token, output token.
*/
void OutputToken(int options) {
	/*
	switch (options) {
	case 0: {
		printf("%d %s %d\n", number, SymType, NumValue);
		break;
	}
	case 1: {
		printf("%d %s %s\n", number, SymType, StringValue);
		break;
	}
	case 2: {
		printf("%d %s ", number, SymType);
		while (StringPrint <= StringCur) {
			printf("%c", *StringPrint);
			StringPrint++;
		}
		printf("\n");
		break;
	}
	default:
		printf("%d %s\n", number, SymType);
	}
	*/
}
/*GetNextch:
	Get next ch.*/
void GetNextch() {
	char prec;
	if (scanf("%c", &prec) == EOF) {
		EOF_FLAG = 1;
		return;
	}
	if (prec == '\n') {
		ll = ll + 1;
		cc = 1;
	}
	else {
		cc++;
	}
	ch = prec;
}
/*Str2Int:
	According to file,get the int expression of str.
*/
void Str2Int() {
	NumValue = 0;
	do {
		NumValue = NumValue * 10 + (ch - '0');
		GetNextch();
	} while ((ch >= '0') && (ch <= '9'));
}
/*Lower2Upper:
	Because C0 grammer don't really care lower/upper a id is, so transfer them into Upper instead.
*/
void Lower2Upper() {
	int index;
	for (index = 0; index < MAX_ID_LENGTH; index++) {
		if ((StringValue[index] >= 'a') && (StringValue[index] <= 'z')) {
			StringValue[index] += ('A' - 'a');
		}
	}
}
/*Insym:
	Do one-step vocabulary analysis.
*/
void InSym() {
	while ((ch == '\n') || (ch == '\t') || (ch == ' ')||(ch == 0)) {
		GetNextch();
		if (EOF_FLAG == 1) {
			return;
		}
	}
	number++;
	int index = 0;
	int i, t;
	char temp = '\0';
	if (EOF_FLAG == 1)
		return;
	//if is 'a' and 'z'
	if (((ch >= 'a') && (ch <= 'z')) || ((ch >= 'A') && (ch <= 'Z'))||(ch == '_')) {
		CpyString(StringValue, SYM[0].expression, MAX_ID_LENGTH);
		do {
			if (index <= 24) {
				StringValue[index] = ch;
				
			}
			GetNextch();
			index++;
		} while (((ch >= 'a') && (ch <= 'z')) || ((ch >= 'A') && (ch <= 'Z')) || (ch == '_')||((ch >= '0') && (ch <= '9')));
		if (index > MAX_ID_LENGTH) {
			;
		}
		Lower2Upper();
		CpyString(SymType, SYM[0].symbol, MAX_SYM_LENGTH);
		for (i = 1; i < MAX_SYMBOL_TYPE; i++) {
			if (CmpString(StringValue, SYM[i].expression, MAX_ID_LENGTH)) {
				CpyString(SymType, SYM[i].symbol, MAX_SYM_LENGTH);
				break;
			}
		}
		if (CmpString(SymType, SYM[0].symbol, MAX_SYM_LENGTH)) {
			OutputToken(1);
		}
		else
			OutputToken(-1);
	}
	else if (((ch >= '0') && (ch <= '9'))) {
		Str2Int();
		CpyString(SymType, SYM[33].symbol, MAX_SYM_LENGTH);
		OutputToken(0);
	}
	else if (IsSingleTerminalSym()) {
		CpyString(StringValue, SYM[0].expression, MAX_ID_LENGTH);
		StringValue[0] = ch;
		for (i = 1; i < MAX_SYMBOL_TYPE; i++) {
			if (CmpString(StringValue, SYM[i].expression, MAX_SYM_LENGTH)) {
				CpyString(SymType, SYM[i].symbol, MAX_SYM_LENGTH);
				OutputToken(-1);
				break;
			}
		}
		GetNextch();
	}
	else if ((ch == '<') || (ch == '>') || (ch == '=') || (ch == '!')) {
		CpyString(StringValue, SYM[0].expression, MAX_ID_LENGTH);
		temp = ch;
		GetNextch();
		if (ch == '=') {
			StringValue[0] = temp;
			StringValue[1] = ch;
			GetNextch();
		}
		else {
			if (temp == '!') {
				errormsg(NEQ_ERROR);
				errorhandle();
				return;
			}
			StringValue[0] = temp;
		}
		for (i = 1; i < MAX_SYMBOL_TYPE; i++) {
			t = (CmpString(StringValue, SYM[i].expression, MAX_ID_LENGTH));
			if (t == 1) {
				CpyString(SymType, SYM[i].symbol, MAX_SYM_LENGTH);
				OutputToken(-1);
				break;
			}
		}
	}
	else if ((ch == '\'') || (ch == '"')) {
		temp = ch;
		GetNextch();
		if (temp == '\'') {
			if (IsLegalChar()) {
				CpyString(SymType, SYM[35].expression, MAX_SYM_LENGTH);
				StringValue[0] = ch;
				GetNextch();
				if (ch != '\'') {
					errormsg(OVER_LEN_CHAR);
					errorhandle();
				}
				else {
					OutputToken(1);
					GetNextch();
				}
			}
			else {
				while (ch != '\'') {
					GetNextch();
				}
				GetNextch();
				errormsg(ILLEGAL_CHAR);
				errorhandle();
			}
		}
		else {
			StringLen = 0;
			*StringHead = ch;
			StringCur = StringHead;//start
			StringPrint = StringHead;
			while ((ch != '"') && (EOF_FLAG != 1)&&(ch!='\n')) {
				GetNextch();
				StringLen++;
				StringCur++;
				*StringCur = ch;
			}
			if (ch == '\n') {
				errormsg(D_QUOTE_NOMATCH);
				errorhandle();
			}
			StringLen++;
			*StringCur = '\0';
			if (ch == '"') {
				CpyString(SymType, SYM[34].symbol, MAX_SYM_LENGTH);
				OutputToken(2);
				for (StringPrint = StringHead; StringPrint != StringCur; StringPrint++) {
					if (!((*StringPrint == 32) || (*StringPrint == 33) || ((*StringPrint >= 35) && (*StringPrint <= 126)))) {
						errormsg(ILLEGAL_STRING);
						errorhandle();
						break;
					}
				}
			}
			else {
				errormsg(UNEXPECTED_EOF);
				errorhandle();
			}
			//free(StringHead);
			//free(StringCur);
			//free(StringPrint);
			GetNextch();
		}
	}
	else {
		errormsg(2);
		GetNextch();
	}
}
//main
void setup() {
	/* Initialize SYM structure array.  */
	CpyString(SYM[IDENT].expression, "                         ", MAX_ID_LENGTH); CpyString(SYM[IDENT].symbol, "IDENT                    ", MAX_SYM_LENGTH);
	CpyString(SYM[PLUS].expression, "+                        ", MAX_ID_LENGTH); CpyString(SYM[PLUS].symbol, "PLUS                     ", MAX_SYM_LENGTH);
	CpyString(SYM[MINUS].expression, "-                        ", MAX_ID_LENGTH); CpyString(SYM[MINUS].symbol, "MINUS                    ", MAX_SYM_LENGTH);
	CpyString(SYM[TIMES].expression, "*                        ", MAX_ID_LENGTH); CpyString(SYM[TIMES].symbol, "TIMES                    ", MAX_SYM_LENGTH);
	CpyString(SYM[DIV].expression, "/                        ", MAX_ID_LENGTH); CpyString(SYM[DIV].symbol, "DIV                      ", MAX_SYM_LENGTH);
	CpyString(SYM[LSS].expression, "<                        ", MAX_ID_LENGTH); CpyString(SYM[LSS].symbol, "LSS                      ", MAX_SYM_LENGTH);
	CpyString(SYM[LEQ].expression, "<=                       ", MAX_ID_LENGTH); CpyString(SYM[LEQ].symbol, "LEQ                      ", MAX_SYM_LENGTH);
	CpyString(SYM[GTR].expression, ">                        ", MAX_ID_LENGTH); CpyString(SYM[GTR].symbol, "GTR                      ", MAX_SYM_LENGTH);
	CpyString(SYM[GEQ].expression, ">=                       ", MAX_ID_LENGTH); CpyString(SYM[GEQ].symbol, "GEQ                      ", MAX_SYM_LENGTH);
	CpyString(SYM[EQL].expression, "==                       ", MAX_ID_LENGTH); CpyString(SYM[EQL].symbol, "EQL                      ", MAX_SYM_LENGTH);
	CpyString(SYM[NEQ].expression, "!=                       ", MAX_ID_LENGTH); CpyString(SYM[NEQ].symbol, "NEQ                      ", MAX_SYM_LENGTH);
	CpyString(SYM[LBRACK].expression, "[                        ", MAX_ID_LENGTH); CpyString(SYM[LBRACK].symbol, "LBRACK                   ", MAX_SYM_LENGTH);
	CpyString(SYM[RBRACK].expression, "]                        ", MAX_ID_LENGTH); CpyString(SYM[RBRACK].symbol, "RBRACK                   ", MAX_SYM_LENGTH);
	CpyString(SYM[LPARENT].expression, "(                        ", MAX_ID_LENGTH); CpyString(SYM[LPARENT].symbol, "LPARENT                  ", MAX_SYM_LENGTH);
	CpyString(SYM[RPARENT].expression, ")                        ", MAX_ID_LENGTH); CpyString(SYM[RPARENT].symbol, "RPAREN               T   ", MAX_SYM_LENGTH);
	CpyString(SYM[LBRACE].expression, "{                        ", MAX_ID_LENGTH); CpyString(SYM[LBRACE].symbol, "LBRACE    ", MAX_SYM_LENGTH);
	CpyString(SYM[RBRACE].expression, "}                        ", MAX_ID_LENGTH); CpyString(SYM[RBRACE].symbol, "RBRACE                   ", MAX_SYM_LENGTH);
	CpyString(SYM[BECOME].expression, "=                        ", MAX_ID_LENGTH); CpyString(SYM[BECOME].symbol, "BECOME                   ", MAX_SYM_LENGTH);
	CpyString(SYM[COMMA].expression, ",                        ", MAX_ID_LENGTH); CpyString(SYM[COMMA].symbol, "COMMA                    ", MAX_SYM_LENGTH);
	CpyString(SYM[COLON].expression, ":                        ", MAX_ID_LENGTH); CpyString(SYM[COLON].symbol, "COLON                    ", MAX_SYM_LENGTH);
	CpyString(SYM[SEMICOLON].expression, ";                        ", MAX_ID_LENGTH); CpyString(SYM[SEMICOLON].symbol, "SEMICOLON                ", MAX_SYM_LENGTH);
	CpyString(SYM[QUOTE].expression, "'                        ", MAX_ID_LENGTH); CpyString(SYM[QUOTE].symbol, "QUOTE                    ", MAX_SYM_LENGTH);
	CpyString(SYM[DQUOTE].expression, "\"                        ", MAX_ID_LENGTH); CpyString(SYM[DQUOTE].symbol, "D-QUOTE                  ", MAX_SYM_LENGTH);
	CpyString(SYM[IFSY].expression, "IF                       ", MAX_ID_LENGTH); CpyString(SYM[IFSY].symbol, "IFSY                     ", MAX_SYM_LENGTH);
	CpyString(SYM[ELSESY].expression, "ELSE                     ", MAX_ID_LENGTH); CpyString(SYM[ELSESY].symbol, "ELSESY                   ", MAX_SYM_LENGTH);
	CpyString(SYM[DOSY].expression, "DO                       ", MAX_ID_LENGTH); CpyString(SYM[DOSY].symbol, "DOSY                     ", MAX_SYM_LENGTH);
	CpyString(SYM[WHILESY].expression, "WHILE                    ", MAX_ID_LENGTH); CpyString(SYM[WHILESY].symbol, "WHILESY                  ", MAX_SYM_LENGTH);
	CpyString(SYM[SWITCHSY].expression, "SWITCH                   ", MAX_ID_LENGTH); CpyString(SYM[SWITCHSY].symbol, "SWITCHSY                 ", MAX_SYM_LENGTH);
	CpyString(SYM[CASESY].expression, "CASE                     ", MAX_ID_LENGTH); CpyString(SYM[CASESY].symbol, "CASESY                   ", MAX_SYM_LENGTH);
	CpyString(SYM[DEFAULTSY].expression, "DEFAULT                  ", MAX_ID_LENGTH); CpyString(SYM[DEFAULTSY].symbol, "DEFAULTSY                ", MAX_SYM_LENGTH);
	CpyString(SYM[SCANFSY].expression, "SCANF                    ", MAX_ID_LENGTH); CpyString(SYM[SCANFSY].symbol, "SCANFSY                  ", MAX_SYM_LENGTH);
	CpyString(SYM[PRINTFSY].expression, "PRINTF                   ", MAX_ID_LENGTH); CpyString(SYM[PRINTFSY].symbol, "PRINTFSY                 ", MAX_SYM_LENGTH);
	CpyString(SYM[RETURNSY].expression, "RETURN                   ", MAX_ID_LENGTH); CpyString(SYM[RETURNSY].symbol, "RETURNSY                 ", MAX_SYM_LENGTH);
	CpyString(SYM[INTCON].expression, "INTCON                   ", MAX_ID_LENGTH); CpyString(SYM[INTCON].symbol, "INTCON                   ", MAX_SYM_LENGTH);
	CpyString(SYM[STRCON].expression, "STRCON                   ", MAX_ID_LENGTH); CpyString(SYM[STRCON].symbol, "STRCON                   ", MAX_SYM_LENGTH);
	CpyString(SYM[CHARCON].expression, "CHARCON                  ", MAX_ID_LENGTH); CpyString(SYM[CHARCON].symbol, "CHARCON                  ", MAX_SYM_LENGTH);
	CpyString(SYM[INTSY].expression, "INT                      ", MAX_ID_LENGTH); CpyString(SYM[INTSY].symbol, "INTSY                    ", MAX_SYM_LENGTH);
	CpyString(SYM[CHARSY].expression, "CHAR                     ", MAX_ID_LENGTH); CpyString(SYM[CHARSY].symbol, "CHARSY                   ", MAX_SYM_LENGTH);
	CpyString(SYM[VOIDSY].expression, "VOID                     ", MAX_ID_LENGTH); CpyString(SYM[VOIDSY].symbol, "VOIDSY                   ", MAX_SYM_LENGTH);
	CpyString(SYM[CONSTSY].expression, "CONST                    ", MAX_ID_LENGTH); CpyString(SYM[CONSTSY].symbol, "CONSTSY                  ", MAX_SYM_LENGTH);
	CpyString(SYM[MAIN].expression, "MAIN                     ", MAX_ID_LENGTH); CpyString(SYM[MAIN].symbol, "IDENT                    ", MAX_SYM_LENGTH);

	//Alloc StringHead Point.
	StringHead = (char*)malloc(sizeof(char));
	StringCur = (char*)malloc(sizeof(char));
}