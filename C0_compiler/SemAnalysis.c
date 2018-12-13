#include<stdio.h>
#include<stdlib.h>
#include"compiler.h"

char op[OP_NUM][MAX_OP_LEN];

extern int StringLen;
extern char* StringHead;
extern int str_num;
extern char str[MAX_ID_LENGTH];
symb SYM[MAX_SYMBOL_TYPE];

void initop() {
	CpyString(op[EMPTY], "                         ", MAX_OP_LEN);
	CpyString(op[CON], "CON                      ", MAX_OP_LEN);
	CpyString(op[VAR], "VAR                      ", MAX_OP_LEN);
	CpyString(op[ARR], "ARR                      ", MAX_OP_LEN);
	CpyString(op[PARA], "PARA                     ", MAX_OP_LEN);
	CpyString(op[END], "END                      ", MAX_OP_LEN);
	CpyString(op[ADD], "ADD                      ", MAX_OP_LEN);
	CpyString(op[SUB], "SUB                      ", MAX_OP_LEN);
	CpyString(op[MUL], "MUL                      ", MAX_OP_LEN);
	CpyString(op[DIVIDE], "DIVIDE                   ", MAX_OP_LEN);
	CpyString(op[LODARR], "LODARR                   ", MAX_OP_LEN);
	CpyString(op[LODI], "LODI                     ", MAX_OP_LEN);
	CpyString(op[LODV], "LODV                     ", MAX_OP_LEN);
	CpyString(op[STOARR], "STOARR                   ", MAX_OP_LEN);
	CpyString(op[GQL], "GQL                      ", MAX_OP_LEN);
	CpyString(op[GRT], "GRT                      ", MAX_OP_LEN);
	CpyString(op[LESS], "LESS                     ", MAX_OP_LEN);
	CpyString(op[LQL], "LQL                      ", MAX_OP_LEN);
	CpyString(op[EQU], "EQU                      ", MAX_OP_LEN);
	CpyString(op[NQL], "NQL                      ", MAX_OP_LEN);
	CpyString(op[PRT], "PRT                      ", MAX_OP_LEN);
	CpyString(op[SCF], "SCF                      ", MAX_OP_LEN);
	CpyString(op[CALL], "CALL                     ", MAX_OP_LEN);
	CpyString(op[LAB], "LAB                      ", MAX_OP_LEN);
	CpyString(op[JMP], "JMP                      ", MAX_OP_LEN);
	CpyString(op[RET], "RET                      ", MAX_OP_LEN);
	CpyString(op[GLBSTR], "GLBSTR                   ", MAX_OP_LEN);
	CpyString(op[GLBEND], "GLBEND                   ", MAX_OP_LEN);
	CpyString(op[FUNC], "FUNC                     ", MAX_OP_LEN);
	CpyString(op[NEG], "NEG                      ", MAX_OP_LEN);
	CpyString(op[LODR], "LODR                     ", MAX_OP_LEN);
}
void initquad() {
	C0_Quad.top = 0;
	return;
	
}
void emit(char* operation,char* operand1,char* operand2,char* operand3) {
	int top = C0_Quad.top;
	CpyString(C0_Quad.item[top].op, operation, MAX_OP_LEN);
	CpyString(C0_Quad.item[top].operand1, operand1, MAX_OP_LEN);
	CpyString(C0_Quad.item[top].operand2, operand2, MAX_OP_LEN);
	CpyString(C0_Quad.item[top].operand3, operand3, MAX_OP_LEN);
	C0_Quad.item[top].op[MAX_OP_LEN] = '\0';
	C0_Quad.item[top].operand1[MAX_OP_LEN] = '\0';
	C0_Quad.item[top].operand2[MAX_OP_LEN] = '\0';
	C0_Quad.item[top].operand3[MAX_OP_LEN] = '\0';
	C0_Quad.item[top].str_name[MAX_OP_LEN] = '\0';
	if (CmpString(operation, op[PRT], MAX_OP_LEN)&&
		!CmpString(operand3, "3                        ", MAX_OP_LEN)) {
		CpyString(strset[str_num], StringHead, StringLen);
		strset[str_num][StringLen] = '\0';
		CpyString(C0_Quad.item[top].str_name, str, MAX_OP_LEN);
	}
	C0_Quad.top++;
	return;
}
void getQuad() {
	int i;
	
	for (i = 0; i < C0_Quad.top; i++) {
		printf("%s %s %s %s\n", C0_Quad.item[i].op, C0_Quad.item[i].operand1, C0_Quad.item[i].operand2, C0_Quad.item[i].operand3);

	}
	
	return;
}