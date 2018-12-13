#include<stdio.h>
#include<stdlib.h>
#include"compiler.h"
int global;
int mips_flag = 0;
extern char op[OP_NUM][MAX_OP_LEN], string[MAX_STRING][25];
extern int curlev;
extern int str_num;
extern SymTable C0_SymTable;
int getvalue(char value[]) {
	int index = 0;
	int v = 0;
	int neg = 0;
	if (value[0] == '-') {
		neg = 1;
		index++;
	}
	while (value[index] != ' ') {
		v = v * 10 + (value[index] - '0');
		index++;
	}
	if (neg)
		v = v * (-1);
	return v;
}
void Quad2Asm(QuadItem item) {
	char *operation, *operator1, *operator2, *operator3;
	char temp[7];
	int found;
	int addr,funcsize,offset;
	int addr1, addr2, result, inaddr, nameaddr,arraddr;
	operation = item.op;
	operator1 = item.operand1;
	operator2 = item.operand2;
	operator3 = item.operand3;
	
	//Begin to handle with all kind of quad
	/*CON <NAME>,<TYPE>,<VALUE>
	*/
	if (CmpString(operation, op[CON], MAX_OP_LEN)) {
		found = find_sym(operator1);
		if (global) {
			/*global constant*/
			if (CmpString(C0_SymTable.item[found].type, "CHAR                     ", MAX_SYM_LENGTH)) {//char type
				printf("%s: .word '%c'\n", operator1, operator3[0]);
				return;
			}
			else {//int type
				printf("%s: .word %s\n", operator1, operator3);
			}
		}
		else {
			addr = C0_SymTable.item[found].addr;
			if (CmpString(C0_SymTable.item[found].type, "CHAR                     ", MAX_SYM_LENGTH)) {//char type
				printf("li $t0,'%c'\n", operator3[0]);
				printf("sw $t0,%d($sp)\n", addr);
			}
			else {//int type
				printf("li $t0,%s\n", operator3);
				printf("sw $t0,%d($sp)\n", addr);
			}
		}
		return;
	}
	/*VARIABLE <NAME>,<TYPE>*/
	else if (CmpString(operation, op[VAR], MAX_OP_LEN)) {
		if (global) {
			printf("%s: .space 4\n",operator1);
		}
		return;
	}
	/*ARR <NAME>,<TYPE>,<NUM>*/
	else if (CmpString(operation, op[ARR], MAX_OP_LEN)) {
		if (global) {
			printf("%s: .word 1:%s\n", operator1, operator3);
		}
		return;
	}
	else if (CmpString(operation, op[PARA], MAX_OP_LEN)) {
		//PARA <name>, <number>, <func>
		found = find_sym(operator1);
		addr = C0_SymTable.item[found].addr;
		found = find_sym(operator3);
		funcsize = C0_SymTable.item[found].value;
		offset = getvalue(operator2) * 4;
		printf("lw $t1,%d($sp)\n", addr);
		printf("subi $sp,$sp,%d\n",funcsize);
		printf("sw $t1,%d($sp)\n", offset);
		printf("addi $sp,$sp,%d\n", funcsize);
		/*found = find_sym(operator1);//parameter
		addr = C0_SymTable.item[found].addr;//parameter address
		found = find_sym(operator3);//function
		funcsize = C0_SymTable.item[found].value;
		offset = getvalue(operator2) * 4;
		if (C0_SymTable.item[found].number == 1) {
			printf("lw $t1,%d($sp)\n", addr);
			printf("subi $sp,$sp,%d\n", funcsize);
			printf("sw $t1,%d($sp)\n", offset);
			printf("addi $sp,$sp,%d\n", funcsize);
		}
		else if (getvalue(operator2) == 1) {
			printf("lw $t1,%d($sp)\n", addr);
			printf("subi $sp,$sp,%d\n", funcsize);
			printf("sw $t1,%d($sp)\n", offset);
		}
		else if (getvalue(operator2) == C0_SymTable.item[found].number) {
			printf("lw $t1,%d($sp)\n", addr);
			printf("sw $t1,%d($sp)\n", offset);
			printf("addi $sp,$sp,%d\n", funcsize);
		}
		else {
			printf("lw $t1,%d($sp)\n", addr);
			printf("sw $t1,%d($sp)\n", offset);
		}*/
	}
	else if (CmpString(operation, op[FUNC], MAX_OP_LEN)) {
		//FUNC <FUNCTIONNAME>,<empty>,<empty>
		found = find_sym(operator1);
		curlev = found;
		funcsize = C0_SymTable.item[found].value;
		printf("%s:\n", operator1);//label set
		printf("subi $sp,$sp,%d\n", funcsize);//set stack space
		printf("sw $ra,0($sp)\n");//set return value
	}
	else if (CmpString(operation, op[END], MAX_OP_LEN)) {
		printf("lw $ra,0($sp)\n");
		funcsize = C0_SymTable.item[curlev].value;
		if (CmpString(C0_SymTable.item[curlev].name, "MAIN                     ", MAX_SYM_LENGTH)) {
			printf("li $v0,10\n");
			printf("syscall\n");
		}
		else {
			printf("addi $sp,$sp,%d\n",funcsize);
			printf("jr $ra\n");
		}
	}
	else if (CmpString(operation, op[ADD], MAX_OP_LEN)||CmpString(operation, op[SUB], MAX_OP_LEN)||
		CmpString(operation, op[MUL], MAX_OP_LEN)||CmpString(operation, op[DIVIDE], MAX_OP_LEN)) {
		//<ADD>/<SUB>/<MUL>/<DIVIDE> op1,op2,result
		addr1 = C0_SymTable.item[find_sym(operator1)].addr;
		addr2 = C0_SymTable.item[find_sym(operator2)].addr;
		result = C0_SymTable.item[find_sym(operator3)].addr;
		printf("lw $t0,%d($sp)\n", addr1);
		printf("lw $t1,%d($sp)\n", addr2);
		if (CmpString(operation, op[ADD], MAX_OP_LEN)) {
			printf("add $t2,$t0,$t1\n");
		}
		else if (CmpString(operation, op[SUB], MAX_OP_LEN)) {
			printf("sub $t2,$t0,$t1\n");
		}
		else if (CmpString(operation, op[MUL], MAX_OP_LEN)) {
			printf("mult $t0,$t1\n");
			printf("mflo $t2\n");
		}
		else {
			printf("div $t0,$t1\n");
			printf("mflo $t2\n");
		}
		printf("sw $t2,%d($sp)\n", result);
	}
	else if (CmpString(operation, op[LODARR], MAX_OP_LEN)) {
		//LODARR <ARR>,<INDEX>,<TMP>
		inaddr = C0_SymTable.item[find_sym(operator2)].addr;
		nameaddr = C0_SymTable.item[find_sym(operator3)].addr;
		arraddr = C0_SymTable.item[find_sym(operator1)].addr;
		if (C0_SymTable.item[find_sym(operator3)].lev != 0) {
			if (C0_SymTable.item[find_sym(operator1)].lev != 0) {
				//global array
				printf("lw $t1,%d($sp)\n", inaddr);
				printf("mul $t1,$t1,4\n");
				printf("addi $t1,$t1,%d\n", arraddr);
				printf("add $t1,$t1,$sp\n");
				printf("lw $t0,0($t1)\n");
				printf("sw $t0,%d($sp)\n", nameaddr);
			}
			else {
				printf("la $t0,%s\n", operator1);
				printf("lw $t1,%d($sp)\n", inaddr);
				printf("mul $t1,$t1,4\n");
				printf("add $t1,$t1,$t0\n");
				printf("lw $t0,0($t1)\n");
				printf("sw $t0,%d($sp)\n", nameaddr);
			}
		}
		else {
			if (C0_SymTable.item[find_sym(operator1)].lev != 0) {
				//global array
				printf("lw $t1,%d($sp)\n", inaddr);
				printf("mul $t1,$t1,4\n");
				printf("addi $t1,$t1,%d\n", arraddr);
				printf("add $t1,$t1,$sp\n");
				printf("lw $t0,0($t1)\n");
				//error la $t1,operator3
				//sw $t0,0($t1)
				printf("la $t1,%s\n", operator3);
				printf("sw $t0,0($t1)\n");
			}
			else {
				printf("la $t0,%s\n", operator1);
				printf("lw $t1,%d($sp)\n", inaddr);
				printf("mul $t1,$t1,4\n");
				printf("add $t1,$t1,$t0\n");
				printf("la $t1,%s\n", operator3);
				printf("sw $t0,0($t1)\n");
			}
		}
	}
	else if (CmpString(operation, op[LODI], MAX_OP_LEN)) {
		//LODI <NAME>,<VALUE>
		found = find_sym(operator1);
		addr = C0_SymTable.item[found].addr;
		if (CmpString(C0_SymTable.item[found].type, "CHAR                     ", MAX_SYM_LENGTH)) {
			printf("li $t0,'%c'\n", operator2[0]);
			printf("sw $t0,%d($sp)\n", addr);
		}
		else {
			printf("li $t0,%s\n", operator2);
			printf("sw $t0,%d($sp)\n", addr);
		}
	}
	else if (CmpString(operation, op[LODV], MAX_OP_LEN)) {
		//lodv <tmp>,<src>
		addr = C0_SymTable.item[find_sym(operator1)].addr;
		if (C0_SymTable.item[find_sym(operator1)].lev != 0) {
			if (C0_SymTable.item[find_sym(operator2)].lev == 0) {//global
				printf("la $t0,%s\n", operator2);
				printf("lw $t1,0($t0)\n");
				printf("sw $t1,%d($sp)\n", addr);
			}
			else {
				addr1 = C0_SymTable.item[find_sym(operator2)].addr;
				printf("lw $t0,%d($sp)\n", addr1);
				printf("sw $t0,%d($sp)\n", addr);
			}
		}
		else {
			if (C0_SymTable.item[find_sym(operator2)].lev == 0) {//global
				printf("la $t0,%s\n", operator2);
				printf("lw $t1,0($t0)\n");
				printf("la $t0,%s\n", operator1);
				printf("sw $t1,0($t0)\n");
			}
			else {
				addr1 = C0_SymTable.item[find_sym(operator2)].addr;
				printf("lw $t1,%d($sp)\n", addr1);
				printf("la $t0,%s\n", operator1);
				printf("sw $t1,0($t0)\n");
			}
		}
	}
	else if (CmpString(operation, op[STOARR], MAX_OP_LEN)) {
		//STOARR <NAME>,<OFFSET>,<EXNAME>
		arraddr = C0_SymTable.item[find_sym(operator1)].addr;
		inaddr = C0_SymTable.item[find_sym(operator2)].addr;
		addr = C0_SymTable.item[find_sym(operator3)].addr;
		if (C0_SymTable.item[find_sym(operator1)].lev != 0) {
			//not global array
			printf("lw $t0,%d($sp)\n", inaddr);
			printf("mul $t0,$t0,4\n");
			printf("addi $t0,$t0,%d\n", arraddr);
			printf("add $t0,$t0,$sp\n");
			if (C0_SymTable.item[find_sym(operator3)].lev != 0) {
				printf("lw $v0,%d($sp)\n",addr);
			}
			else {
				printf("la $t2,%s\n", operator3);
				printf("lw $v0,0($t2)\n");
			}
			printf("sw $v0,0($t0)\n");
		}
		else {
			printf("la $t0,%s\n", operator1);
			printf("lw $t1,%d($sp)\n", inaddr);
			printf("mul $t1,$t1,4\n");
			printf("add $t0,$t0,$t1\n");
			if (C0_SymTable.item[find_sym(operator3)].lev != 0) {
				printf("lw $v0,%d($sp)\n", addr);
			}
			else {
				printf("la $t2,%s\n", operator3);
				printf("lw $v0,0($t2)\n");
			}
			printf("sw $v0,0($t0)\n");
		}
	}
	else if (CmpString(operation, op[GQL], MAX_OP_LEN)) {
		addr1 = C0_SymTable.item[find_sym(operator1)].addr;
		addr2 = C0_SymTable.item[find_sym(operator2)].addr;
		printf("lw $t0,%d($sp)\n", addr1);
		printf("lw $t1,%d($sp)\n", addr2);
		printf("bge $t0,$t1,%s\n", operator3);
	}
	else if (CmpString(operation, op[GRT], MAX_OP_LEN)) {
		addr1 = C0_SymTable.item[find_sym(operator1)].addr;
		addr2 = C0_SymTable.item[find_sym(operator2)].addr;
		printf("lw $t0,%d($sp)\n", addr1);
		printf("lw $t1,%d($sp)\n", addr2);
		printf("bgt $t0,$t1,%s\n", operator3);
	}
	else if (CmpString(operation, op[LESS], MAX_OP_LEN)) {
		addr1 = C0_SymTable.item[find_sym(operator1)].addr;
		addr2 = C0_SymTable.item[find_sym(operator2)].addr;
		printf("lw $t0,%d($sp)\n", addr1);
		printf("lw $t1,%d($sp)\n", addr2);
		printf("blt $t0,$t1,%s\n", operator3);
	}
	else if (CmpString(operation, op[LQL], MAX_OP_LEN)) {
		addr1 = C0_SymTable.item[find_sym(operator1)].addr;
		addr2 = C0_SymTable.item[find_sym(operator2)].addr;
		printf("lw $t0,%d($sp)\n", addr1);
		printf("lw $t1,%d($sp)\n", addr2);
		printf("ble $t0,$t1,%s\n", operator3);
	}
	else if (CmpString(operation, op[EQU], MAX_OP_LEN)) {
		addr1 = C0_SymTable.item[find_sym(operator1)].addr;
		addr2 = C0_SymTable.item[find_sym(operator2)].addr;
		printf("lw $t0,%d($sp)\n", addr1);
		printf("lw $t1,%d($sp)\n", addr2);
		printf("beq $t0,$t1,%s\n", operator3);
	}
	else if (CmpString(operation, op[NQL], MAX_OP_LEN)) {
		addr1 = C0_SymTable.item[find_sym(operator1)].addr;
		addr2 = C0_SymTable.item[find_sym(operator2)].addr;
		printf("lw $t0,%d($sp)\n", addr1);
		printf("lw $t1,%d($sp)\n", addr2);
		printf("bne $t0,$t1,%s\n", operator3);
	}
	else if (CmpString(operation, op[PRT], MAX_OP_LEN)) {
		if (CmpString(operator3, "1                        ", MAX_SYM_LENGTH)) {//only string
			printf("li $v0,4\n");
			printf("la $a0,%s\n",operator1);
			printf("syscall\n");
			printf("la $a0,$enter\n");
			printf("syscall\n");
		}
		else if (CmpString(operator3, "3                        ", MAX_SYM_LENGTH)) {//only expression
			addr = C0_SymTable.item[find_sym(operator2)].addr;
			if (CmpString(C0_SymTable.item[find_sym(operator2)].type, "INT                      ", MAX_ID_LENGTH)) {
				printf("li $v0,1\n");
			}
			else {
				printf("li $v0,11\n");
			}
			printf("lw $a0,%d($sp)\n", addr);
			printf("syscall\n");
			printf("li $v0,4\n");
			printf("la $a0,$enter\n");
			printf("syscall\n");
		}
		else {//string and expression
			addr = C0_SymTable.item[find_sym(operator2)].addr;
			printf("li $v0,4\n");
			printf("la $a0,%s\n", operator1);
			printf("syscall\n");
			if (CmpString(C0_SymTable.item[find_sym(operator2)].type, "INT                      ", MAX_ID_LENGTH)) {
				printf("li $v0,1\n");
			}
			else {
				printf("li $v0,11\n");
			}
			printf("lw $a0,%d($sp)\n", addr);
			printf("syscall\n");
			printf("li $v0,4\n");
			printf("la $a0,$enter\n");
			printf("syscall\n");
		}
	}
	else if (CmpString(operation, op[SCF], MAX_OP_LEN)) {
		//SCF NAME
		addr = C0_SymTable.item[find_sym(operator1)].addr;
		if (C0_SymTable.item[find_sym(operator1)].lev != 0) {
			//not global
			if (CmpString(C0_SymTable.item[find_sym(operator1)].type, "INT                      ", MAX_SYM_LENGTH)) {
				printf("li $v0,5\n");
			}
			else {
				printf("li $v0,12\n");
			}
			printf("syscall\n");
			printf("sw $v0,%d($sp)\n", addr);
		}
		else {
			if (CmpString(C0_SymTable.item[find_sym(operator1)].type, "INT                      ", MAX_SYM_LENGTH)) {
				printf("li $v0,5\n");
			}
			else {
				printf("li $v0,12\n");
			}
			printf("syscall\n");
			printf("la $t0,%s\n", operator1);
			printf("sw $v0,0($t0)\n");
		}
	}
	else if (CmpString(operation, op[CALL], MAX_OP_LEN)) {
		//call <func>
		printf("jal %s\n", operator1);
	}
	else if (CmpString(operation, op[LAB], MAX_OP_LEN)) {
		//lab <label>
		printf("%s:\n", operator1);
	}
	else if (CmpString(operation, op[JMP], MAX_OP_LEN)) {
		//JMP <label><empty><empty>
		printf("j %s\n", operator1);
	}
	else if (CmpString(operation, op[RET], MAX_OP_LEN)) {
		addr = C0_SymTable.item[find_sym(operator1)].addr;
		funcsize = C0_SymTable.item[curlev].value;
		if (CmpString(C0_SymTable.item[curlev].name, "MAIN                     ", MAX_ID_LENGTH)) {
			printf("li $v0,10\n");
			printf("syscall\n");
		}
		else {
			printf("lw $v0,%d($sp)\n",addr);
			printf("lw $ra,0($sp)\n");
			printf("addi $sp,$sp,%d\n", funcsize);
			printf("jr $ra\n");
		}
	}
	else if (CmpString(operation, op[GLBSTR], MAX_OP_LEN)) {
		printf(".data\n");
		return;
	}
	else if (CmpString(operation, op[GLBEND], MAX_OP_LEN)) {
		int i,j;
		
		for (i = 1; i <= str_num; i++) {
			for (j = 0; j < 6; j++) {
				temp[j] = string[i - 1][j];
			}
			temp[6] = '\0';
			printf("%s : .asciiz \"%s\"\n",temp, strset[i]);
		}
		printf("$enter : .asciiz \"\\n\"\n");
		printf(".text\n");
		printf("j MAIN\n");
	}
	else if (CmpString(operation, op[NEG], MAX_OP_LEN)) {
		//<NEG> <OP>
		addr = C0_SymTable.item[find_sym(operator2)].addr;
		printf("lw $t0,%d($sp)\n", addr);
		printf("sub $t0,$0,$t0\n");
		printf("sw $t0,%d($sp)\n", addr);
	}
	else if (CmpString(operation, op[LODR], MAX_OP_LEN)) {
		//LODR <TMP>,<REG>
		addr = C0_SymTable.item[find_sym(operator1)].addr;
		printf("sw %c%c%c,%d($sp)\n", operator2[0],operator2[1],operator2[2], addr);
	}
	
}
void gen_mips() {
	QuadItem q;
	int index;
	int num_of_quad = C0_Quad.top;
	mips_flag = 1;
	curlev = 0;
	global = 1;		//声明全局变量
	for (index = 0; index < num_of_quad; index++) {
		q = C0_Quad.item[index];
		Quad2Asm(q);
		if (CmpString(q.op, op[GLBEND], MAX_OP_LEN)) {
			global = 0;	//全局常变量声明结束
		}
	}
}