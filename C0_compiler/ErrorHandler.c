#include<stdio.h>
#include<stdlib.h>
#include"compiler.h"

extern int ll, cc;
extern char SymType[MAX_SYM_LENGTH], StringValue[MAX_ID_LENGTH];
extern symb SYM[MAX_SYMBOL_TYPE];
void errormsg(int options) {
	switch (options) {
	case NEQ_ERROR:
		printf("Error:(ll: %d, cc: %d) Got '!' without '='.\n", ll, cc);
		break;
	case ID_OVER_LEN:printf("Error:(ll: %d, cc: %d) Length Overflow.n", ll, cc);
		break;
	case NO_SENSE_SYM:printf("Error:(ll: %d, cc: %d) Unknown symbol.\n", ll, cc);
		break;
	case ILLEGAL_CHAR:printf("Error:(ll: %d, cc: %d) Unlegal character in ''.\n", ll, cc);
		break;
	case OVER_LEN_CHAR:printf("Error:(ll: %d, cc: %d) Charcon's length more than 1.\n", ll, cc);
		break;
	case UNEXPECTED_EOF:printf("Error: Unexpected End of File while processing constant string.\n");
		break;
	case ILLEGAL_STRING:printf("Error:(ll: %d, cc: %d) Unexpected String element while processing constant string!\n", ll, cc);
		break;
	case MISS_CONST:printf("Error:(ll: %d, cc: %d) Missing Const Symbol while processing const declaration.\n", ll, cc);
		break;
	case MISS_SEMICOLON:printf("Error:(ll: %d, cc: %d) Missing ';' in the sentence.\n",ll,cc);
		break;
	case MISS_TYPE:printf("Error:(ll: %d, cc: %d) Missing Type while processing declaration head.\n",ll,cc);
		break;
	case MISS_IDENT:printf("Error:(ll: %d, cc: %d) Missing an identifier in the sentence.\n",ll,cc);
		break;
	case MISS_BECOME:printf("Error:(ll: %d, cc: %d) Missing a become signal in the sentence.\n",ll,cc);
		break;
	case MISS_VALUE:printf("Error:(ll: %d, cc: %d) Missing constant value in the sentence.\n",ll,cc);
		break;
	case BADEND_VARDCL:printf("Error:(ll: %d,cc: %d) A bad end for variable declaration.Missing semicolon.\n",ll,cc);
		break;
	case MISS_RBRACK:printf("Error:(ll: %d,cc: %d) Missing ']' in the sentence.\n",ll,cc);
		break;
	case ILLEGAL_DIM:printf("Error:(ll: %d,cc: %d) An illegal array dimension.Dimension need to be greater than 0.\n",ll,cc);
		break;
	case MISS_RPARENT:printf("Error:(ll: %d,cc: %d) Missing ')' in the sentence.\n",ll,cc);
		break;
	case MISS_RBRACE:printf("Error:(ll: %d,cc: %d) Missing '}' in the sentence.\n",ll,cc);
		break;
	case MISS_LBRACE:printf("Error:(ll: %d,cc: %d) Missingg '{' in the sentence.\n",ll,cc);
		break;
	case MISS_VOID:printf("Error:(ll: %d,cc: %d) Missing 'VOID' in the sentence.\n",ll,cc);
		break;
	case MISS_LPARENT:printf("Error:(ll: %d,cc: %d) Missing '(' in the sentence.\n",ll,cc);
		break;
	case BAD_STATEMENT_HEAD:printf("Error:(ll: %d,cc: %d) Illegal statement head.\n",ll,cc);
		break;
	case BAD_FACTOR_HEAD:printf("Error:(ll: %d,cc: %d) Illegal factor head.\n",ll,cc);
		break;
	case BAD_ASSIGN:printf("Error:(ll: %d,cc: %d) Wrong assigning sentence.\n",ll,cc);
		break;
	case MISS_ELSE:printf("Error:(ll: %d,cc: %d) Missing \"else\" in sentence.\n",ll,cc);
		break;
	case MISS_IF:printf("Error:(ll: %d,cc: %d) Missing \"if\" in the sentence.\n",ll,cc);
		break;
	case MISS_WHILE:printf("Error:(ll: %d,cc: %d) Missing \"while\" in the sentence.\n",ll,cc);
		break;
	case MISS_DO:printf("Error:(ll: %d,cc: %d) Missing \"do\" in the sentence.\n",ll,cc);
		break;
	case MISS_COLON:printf("Error:(ll: %d,cc: %d) Missing ':' in the sentence.\n",ll,cc);
		break;
	case MISS_CASE:printf("Error:(ll: %d,cc: %d) Missing \"case\" in the sentence.\n",ll,cc);
		break;
	case MISS_DEFAULT:printf("Error:(ll: %d,cc: %d) Missing \"default\" in the sentence.\n",ll,cc);
		break;
	case MISS_SWITCH:printf("Error:(ll: %d,cc: %d) Missing \"switch\" in the sentence.\n",ll,cc);
		break;
	case BAD_PRINT_PARA:printf("Error:(ll: %d,cc: %d) Bad parameter in print sentene.\n",ll,cc);
		break;
	case MISS_PRINTF:printf("Error:(ll: %d,cc: %d) Missing \"printf\" in the sentence.\n",ll,cc);
		break;
	case MISS_SCANF:printf("Error:(ll: %d,cc: %d) Missing \"scanf\" in the sentence.\n",ll,cc);
		break;
	case MISS_RETURN:printf("Error:(ll: %d,cc: %d) Missing \"return\" in the sentence.\n",ll,cc);
		break;
	case SYM_REDEFINE:printf("Error:(ll: %d,cc: %d) Symbol have been defined in this yield.\n", ll, cc);
		break;
	case ID_UNDEF:printf("Error:(ll: %d,cc: %d)ID is not predefined in this sentence.\n", ll, cc);
		break;
	case ASSIGN_KIND:printf("Error:(ll: %d,cc: %d) Left of assigning must be value which can be overwritten.\n");
		break;
	case SCF_ERROR:printf("Error:(ll: %d,cc: %d) Scanf error:type of scanf is not variable.\n");
		break;
	case VOID_RETURN:printf("Error:(ll: %d,cc: %d) It is a function without return value.\n");
		break;
	case CALL_ERROR:printf("Error:(ll: %d,cc: %d) Calling parameter's number is not enough.\n");
		break;
	default:printf("Error:(ll: %d,cc: %d) Unknown error.\n");
	}
}
void errorhandle() {
	while (!CmpString(SymType, SYM[SEMICOLON].symbol, MAX_SYM_LENGTH)) {
		InSym();
	}
	InSym();
}