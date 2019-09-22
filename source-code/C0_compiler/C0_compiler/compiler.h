#ifndef COMPILER_H
#define COMPILER_H

#include<stdio.h>
#include<stdlib.h>
#define	MAX_GLOBAL_REG		8
#define TABLESIZE			2000
#define QUADSIZE			2000
#define BASICSIZE			500
#define MAX_SYM_LENGTH		25
#define MAX_ID_LENGTH		25
#define MAX_SYMBOL_TYPE		41
#define MAX_OP_LEN			25
#define OP_NUM				31
#define MAX_STRING			100
#define MAX_BASIC_NEIGHBOR	40
//ERROR CATEGORY
#define NEQ_ERROR			0
#define ID_OVER_LEN			1
#define NO_SENSE_SYM		2
#define ILLEGAL_CHAR		3
#define OVER_LEN_CHAR		4
#define UNEXPECTED_EOF		5
#define ILLEGAL_STRING		6
#define MISS_CONST			7
#define MISS_SEMICOLON		8
#define MISS_TYPE			9
#define MISS_IDENT			10
#define MISS_BECOME			11
#define MISS_VALUE			12
#define BADEND_VARDCL		13
#define MISS_RBRACK			14
#define ILLEGAL_DIM			15
#define MISS_RPARENT		16
#define MISS_RBRACE			17
#define MISS_LBRACE			18
#define MISS_VOID			19
#define MISS_LPARENT		20
#define BAD_STATEMENT_HEAD	21
#define BAD_FACTOR_HEAD		22
#define BAD_ASSIGN			23
#define MISS_ELSE			24
#define MISS_IF				25
#define MISS_WHILE			26	
#define MISS_DO				27
#define MISS_COLON			28
#define MISS_CASE			29
#define MISS_DEFAULT		30
#define MISS_SWITCH			31
#define BAD_PRINT_PARA		32
#define MISS_PRINTF			33
#define MISS_SCANF			34
#define MISS_RETURN			35
#define SIGNAL_WITHOUT_NUM	36
#define SYM_REDEFINE		37
#define ID_UNDEF			38
#define ASSIGN_KIND			39
#define SCF_ERROR			40
#define VOID_RETURN			41
#define CALL_PARANUM_ERROR	42
#define NO_PARA_WITH_PARENT 43
#define CALL_PARATYPE_ERROR 44
#define VOID_WITH_RETURN	45
#define D_QUOTE_NOMATCH		46
#define ID_CONFLICT			47
#define GET_A_WRONG_ARR		48
#define ARR_OUT_RANGE		49
#define TYPE_ERROR			50
//functions in Voc Analysis.c
typedef struct symb {
	char expression[MAX_ID_LENGTH];
	char symbol[MAX_SYM_LENGTH];
} symb;

int CmpString(char *tmp, char *src, int length);
void CpyString(char *tmp, char *src, int length);
void GetNextch();
void InSym();
void setup();
//fucntions in GraAnalysis.c
#define	IDENT		0
#define	PLUS		1
#define	MINUS		2
#define	TIMES		3
#define	DIV			4
#define	LSS			5
#define	LEQ			6
#define	GTR			7
#define	GEQ			8
#define	EQL			9
#define	NEQ			10
#define	LBRACK		11
#define	RBRACK		12
#define	LPARENT		13
#define	RPARENT		14
#define	LBRACE		15
#define	RBRACE		16
#define	BECOME		17
#define	COMMA		18
#define	COLON		19
#define	SEMICOLON	20
#define QUOTE		21
#define DQUOTE		22
#define	IFSY		23
#define	ELSESY		24
#define	DOSY		25
#define	WHILESY		26
#define	SWITCHSY	27
#define	CASESY		28
#define	DEFAULTSY	29
#define	SCANFSY		30
#define	PRINTFSY	31
#define	RETURNSY	32
#define	INTCON		33
#define	STRCON		34
#define	CHARCON		35
#define	INTSY		36
#define	CHARSY		37
#define	VOIDSY		38
#define	CONSTSY		39
#define	MAIN		40

/*SymTable.c
*/

typedef struct {
	char name[MAX_ID_LENGTH];
	char type[MAX_SYM_LENGTH];
	char kind[MAX_SYM_LENGTH];
	int addr;
	int value;
	int number;
	int lev;
	int ref;
	int tmp;
	int exp_num;
}TableItem;
typedef struct {
	TableItem item[TABLESIZE];
	int top;
}SymTable;
SymTable C0_SymTable;
TableItem global_var[MAX_GLOBAL_REG];
void inittable();
int entertable();
void setfunsize();
int find_sym(char name[]);
/*GraAnalysis.c
*/
void program();
void ConDecl();
void ConDef();
void FuncDecl();
void VarDecl();
void VarDef();
void paralist();
void VoidFuncDecl();
void MainDecl();
void statement();
void expression();
void term();
void factor();
void assignstate();
void condition(char label[]);
void ifstate();
void dowhilestate();
void casetable(char *defaultlabel);
void defaulttable(char *defaultlabel);
void switchstate();
void printfstate();
void scanfstate();
void returnstate();
void callstate(char *func_name);
//functions in SemAnalysis.c

#define EMPTY	0
#define CON		1
#define VAR		2
#define ARR		3
#define PARA	4
#define END		5
#define ADD		6
#define SUB		7
#define MUL		8
#define DIVIDE	9
#define LODARR	10
#define LODI	11
#define LODV	12
#define STOARR	13
#define GQL		14
#define GRT		15
#define LESS	16
#define LQL		17
#define EQU		18
#define NQL		19
#define PRT		20
#define SCF		21
#define CALL	22
#define LAB		23
#define JMP		24
#define RET		25
#define GLBSTR	26
#define GLBEND	27
#define FUNC	28
#define NEG		29
#define LODR	30
/*
	QuadItem
*/
typedef struct {
	char op[26];
	char operand1[26];
	char operand2[26];
	char operand3[26];
	char str_name[26];
}QuadItem;
typedef struct {
	QuadItem item[QUADSIZE];
	int top;
}QuadSet;
QuadSet C0_Quad,C0_Quad_opt;
/*defination
	Basics
*/
typedef struct {
	int begin;
	int end;
	int before[MAX_BASIC_NEIGHBOR];
	int after[MAX_BASIC_NEIGHBOR];
}BasicItem;
typedef struct {
	BasicItem item[BASICSIZE];
	int top;
}BasicSet;
BasicSet C0_Basic;

void initop();
void initquad();
void new_str();
void emit(char* op, char* operand1, char* operand2, char* operand3);
void getQuad();
//functions in error.c
void errormsg(int options);
void errorhandle();
//functions in genmips.c
int ref_weight;
char StringSet[MAX_STRING][MAX_STRING];
void gen_mips();

//functions in OptMethods
void DAG();
void peep_hole();
void const_combination();
void divide_basics();
void global_reg_alloc();
int find_in_global_var(char name[]);

/*
	NodeTableItem and NodeTable
*/
typedef struct {
	char name[MAX_OP_LEN];
	int num;
	
	int temp;
	
}NodeTableItem;
typedef struct {
	NodeTableItem item[1000];
	int top;
}NodeTable;

/*
	DAGGraphItem and DAGGraph
*/
typedef struct {
	char range[MAX_OP_LEN];
	int already;
	int num;
	char exp_name[MAX_OP_LEN];
	int name[40];
	int name_len;
	char op[MAX_OP_LEN];
	int Lchild;
	int Rchild;
	int father[40];
	int father_len;
}DAGItem;
typedef struct {
	DAGItem item[1000];
	int top;
}DAGGraph;
#endif
