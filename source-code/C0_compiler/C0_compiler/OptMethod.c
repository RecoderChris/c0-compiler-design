#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include"compiler.h"

NodeTable nodetable;
DAGGraph DAG_graph;
extern int curlev;
int line_no;
char normal[25] = "DEFAULT                  ";
char notype[25] = "NO                       ";
int begin_point;
int end_point;
int s;
/*
 Quad is in C0_Quad
 Basics is in C0_Basic
*/
extern char op[OP_NUM][MAX_OP_LEN];
char vstr[25] = "                        ";
/*whether a name is a tmp*/
/*
	divide_basics
	divide the whole programs to some basics
*/
void divide_basics() {
	int i;
	int top;
	int flag = 0;
	QuadItem q;
	C0_Basic.top = 0;
	for (i = 0; i < C0_Quad.top; i++) {
		q = C0_Quad.item[i];
		top = C0_Basic.top;
		//lab kind
		if (i == 0) {
			C0_Basic.item[top].begin = i;
			C0_Basic.item[top].before[0] = 0;
			C0_Basic.top++;
			flag = 0;
		}
		else if (i == C0_Quad.top - 1) {
			C0_Basic.item[top].begin = i + 1;
			C0_Basic.item[top - 1].end = i ;
			C0_Basic.item[top - 1].after[0] = 0;
			C0_Basic.top++;
			flag = 0;
		}
		else if (CmpString(q.op, op[FUNC], MAX_OP_LEN) ||
			//CmpString(q.op, op[LAB], MAX_OP_LEN) ||
			CmpString(q.op, op[GLBSTR], MAX_OP_LEN)) {
			if (flag) {
				;
			}
			else {
				C0_Basic.item[top].begin = i;
				C0_Basic.item[top - 1].end = i - 1;
				C0_Basic.item[top].before[0] = top - 1;
				C0_Basic.item[top - 1].after[0] = top;
				C0_Basic.top++;
			}
		}
		/*else if (//CmpString(q.op, op[GQL], MAX_OP_LEN) ||
			//CmpString(q.op, op[GRT], MAX_OP_LEN) ||
			//CmpString(q.op, op[LESS], MAX_OP_LEN) ||
			//CmpString(q.op, op[LQL], MAX_OP_LEN) ||
			//CmpString(q.op, op[EQU], MAX_OP_LEN) ||
			//CmpString(q.op, op[NQL], MAX_OP_LEN) ||
			//CmpString(q.op, op[CALL], MAX_OP_LEN) ||
			//CmpString(q.op, op[JMP], MAX_OP_LEN) ||
			CmpString(q.op, op[END], MAX_OP_LEN)) {
			C0_Basic.item[top].begin = i + 1;
			C0_Basic.item[top - 1].end = i;
			C0_Basic.item[top].before[0] = top - 1;
			C0_Basic.item[top - 1].after[0] = top;
			C0_Basic.top++;
			flag = 1;
		}*/
		else {
			flag = 0;
		}
	}
}
/*
	global_reg_alloc()
	use the counting method to alloc the global regs
*/
void global_reg_alloc() {
	int i,j,k;
	global_var[0] = C0_SymTable.item[0];
	for (i = 1; i < MAX_GLOBAL_REG; i++) {
		for (j = i - 1; j >= 0; j--) {
			if (C0_SymTable.item[i].ref > global_var[j].ref);
			else
				break;
		}
		for (k = i; k > j; k--) {
			global_var[k] = global_var[k - 1];
		}
		global_var[j + 1] = C0_SymTable.item[i];
	}
	for (i = MAX_GLOBAL_REG; i < C0_SymTable.top; i++) {
		for (k = MAX_GLOBAL_REG - 1; k >= 0; k--) {
			if (C0_SymTable.item[i].ref > global_var[k].ref);
			else
				break;
		}
		for (j = MAX_GLOBAL_REG; j > k; j--) {
			global_var[j] = global_var[j - 1];
		}
		global_var[k + 1] = C0_SymTable.item[i];
	}
}
/*
	find_in_global_var()
	find the variable if in the global reg
*/
int find_in_global_var(char name[]) {
	int i;
	for (i = 0; i < MAX_GLOBAL_REG; i++) {
		if (CmpString(name, global_var[i].name, MAX_ID_LENGTH)) {
			if ((global_var[i].ref != 0)&&(!CmpString(global_var[i].kind,"PARA                     ", MAX_ID_LENGTH))
				&&(curlev == global_var[i].lev)) {
				return i;
			}
		}
		else;
	}
	return -1;
}
void get_str(int value) {
	char ch[26];
	int i = 0,j,neg;
	CpyString(vstr, "                        ", MAX_OP_LEN);
	if (value < 0) {
		neg = 1;
		value = 0 - value;
	}
	else {
		neg = 0;
	}
	do
	{
		ch[i] = value % 10 + '0';
		value = value / 10;
		i++;
	} while (value != 0);
	for (j = neg,i = i - 1; i >= 0; j++, i--) {
		vstr[j] = ch[i];
	}
	if (neg) {
		vstr[0] = '-';
	}
}
/*
	const combination
*/
int get_num(char num[]) {
	int neg;
	int i;
	int value = 0;
	if (num[0] == '-') {
		neg = 1;
	}
	else {
		neg = 0;
	}
	for (i = neg; num[i] >= '0'&& num[i] <= '9'; i++) {
		value = value * 10 + num[i] - '0';
	}
	if (neg) {
		value = 0 - value;
	}
	return value;
}
//const_init()
void const_init() {
	int i;
	int no = 0;
	QuadItem q;
	char *operation, *operator1, *operator2, *operator3;
	int found, found1, found2;
	curlev = 0;
	for (i = 0; i < C0_Quad.top; i++) {
		q = C0_Quad.item[i];
		operation = q.op;
		operator1 = q.operand1;
		operator2 = q.operand2;
		operator3 = q.operand3;
		if (CmpString(C0_Quad.item[i].op, op[PARA], MAX_OP_LEN)) {
			no++;
			found = find_sym(operator1);
			if (C0_SymTable.item[found].tmp == 1) {
				C0_SymTable.item[found].exp_num = no;
			}
		}
		else if (CmpString(C0_Quad.item[i].op, op[LODARR], MAX_OP_LEN)) {
			no++;
			found = find_sym(operator2);
			if (C0_SymTable.item[found].tmp == 1) {
				C0_SymTable.item[found].exp_num = no;
			}
		}
		else if (CmpString(C0_Quad.item[i].op, op[LODR], MAX_OP_LEN)) {
			no++;
			found = find_sym(operator1);
			if (C0_SymTable.item[found].tmp == 1) {
				C0_SymTable.item[found].exp_num = no;
			}
		}
		else if (CmpString(C0_Quad.item[i].op, op[LODV], MAX_OP_LEN)) {
			found = find_sym(operator1);
			found1 = find_sym(operator2);
			if (CmpString(C0_SymTable.item[found1].kind, "CONSTANT                 ", MAX_OP_LEN)||
				(C0_SymTable.item[found1].tmp!=0)){
				if((C0_SymTable.item[found].tmp == 0)||
					CmpString(C0_SymTable.item[found1].kind, "CONSTANT                 ", MAX_OP_LEN)) {
					C0_SymTable.item[found].exp_num = no;
				}
				else {
					C0_SymTable.item[found].exp_num = C0_SymTable.item[found1].exp_num;
				}
			}
		}
		else if (CmpString(C0_Quad.item[i].op, op[LODI], MAX_OP_LEN)) {
			found = find_sym(operator1);
			if (C0_SymTable.item[found].tmp == 1) {
				C0_SymTable.item[found].exp_num = no;
			}
		}
		else if (CmpString(operation, op[ADD], MAX_OP_LEN) || CmpString(operation, op[SUB], MAX_OP_LEN) ||
			CmpString(operation, op[DIVIDE], MAX_OP_LEN) || CmpString(operation, op[MUL], MAX_OP_LEN)) {
			found = find_sym(operator1);
			found1 = find_sym(operator2);
			found2 = find_sym(operator3);
			if ((C0_SymTable.item[found2].tmp == 1)&& 
				(CmpString(C0_SymTable.item[found].kind, "CONSTANT                 ", MAX_OP_LEN))&&
				(CmpString(C0_SymTable.item[found1].kind, "CONSTANT                 ", MAX_OP_LEN))) {
				C0_SymTable.item[found2].exp_num = no;
			}
		}
		else if (CmpString(C0_Quad.item[i].op, op[NEG], MAX_OP_LEN)) {
			found = find_sym(operator1);
			if (C0_SymTable.item[found].tmp == 1) {
				C0_SymTable.item[found].exp_num = no;
			}
		}
		else if (CmpString(C0_Quad.item[i].op, op[FUNC], MAX_OP_LEN)) {
			found = find_sym(operator1);
			curlev = found;
		}
	}
}
void const_combination() {
	char *operation, *operator1, *operator2, *operator3;
	char *aim[25];
	QuadItem q;
	int i,j;
	int found,found1,found2;
	int value;
	int length = 0;
	int curno = 0;
	curlev = 0;
	const_init();
	for (i = 0; i < C0_Quad.top; i++) {
		q = C0_Quad.item[i];
		operation = q.op;
		operator1 = q.operand1;
		operator2 = q.operand2;
		operator3 = q.operand3;
		if (CmpString(operation, op[LODV], MAX_OP_LEN)) {
			//LODV <TMP>,<SRC>
			found = find_sym(operator2);
			found1 = find_sym(operator1);
			if (CmpString(C0_SymTable.item[found].kind, "CONSTANT                 ", MAX_OP_LEN)&&
				(C0_SymTable.item[found1].tmp == 1)&&(C0_SymTable.item[found1].exp_num == curno)) {
				CpyString(C0_SymTable.item[found1].kind,C0_SymTable.item[found].kind, MAX_OP_LEN);
				C0_SymTable.item[found1].value = C0_SymTable.item[found].value;
				CpyString(aim, C0_SymTable.item[found1].name, MAX_OP_LEN);
				length++;
			}
			else {
				if (length != 0) {
					CpyString(C0_Quad.item[i - length].op, op[LODI], MAX_OP_LEN);
					CpyString(C0_Quad.item[i - length].operand1, aim, MAX_OP_LEN);
					found = find_sym(aim);
					get_str(C0_SymTable.item[found].value);
					CpyString(C0_Quad.item[i - length].operand2, vstr,MAX_OP_LEN);
					for (j = i; j < C0_Quad.top; j++) {
						C0_Quad.item[j - length + 1] = C0_Quad.item[j];
					}
					C0_Quad.top = C0_Quad.top - length + 1;
				}
				if (curno != C0_SymTable.item[found1].exp_num) {
					curno = C0_SymTable.item[found1].exp_num;
					i = i - 1;
				}
				length = 0;
			}
		}
		else if (CmpString(operation, op[LODI], MAX_OP_LEN)) {
			//LODI <NAME>,<VALUE>
			found = find_sym(operator1);
			found1 = find_sym(aim);
			if ((C0_SymTable.item[found].tmp == 1)&&(C0_SymTable.item[found].exp_num == curno)){
				length++;
				CpyString(C0_SymTable.item[found].kind, "CONSTANT                 ", MAX_OP_LEN);
				CpyString(aim, C0_SymTable.item[found].name, MAX_OP_LEN);
				if (!((operator2[0] >= '0') && (operator2[0] <= '9')) &&
					!((operator2[1] >= '0') && (operator2[1] <= '9'))) {
					C0_SymTable.item[found].value = operator2[0];
				}
				else {
					C0_SymTable.item[found].value = get_num(operator2);
				}
			}
			else {
				if (length != 0) {
					CpyString(C0_Quad.item[i - length].op, op[LODI], MAX_OP_LEN);
					CpyString(C0_Quad.item[i - length].operand1, aim, MAX_OP_LEN);
					found = find_sym(aim);
					get_str(C0_SymTable.item[found].value);
					CpyString(C0_Quad.item[i - length].operand2, vstr, MAX_OP_LEN);
					for (j = i; j < C0_Quad.top; j++) {
						C0_Quad.item[j - length + 1] = C0_Quad.item[j];
					}
					C0_Quad.top = C0_Quad.top - length + 1;
				}
				if (curno != C0_SymTable.item[found].exp_num) {
					curno = C0_SymTable.item[found].exp_num;
					i = i - 1;
				}
				length = 0;
			}
		}
		else if (CmpString(operation, op[ADD], MAX_OP_LEN) || CmpString(operation, op[SUB], MAX_OP_LEN) ||
			CmpString(operation, op[DIVIDE], MAX_OP_LEN) || CmpString(operation, op[MUL], MAX_OP_LEN)) {
			found = find_sym(operator1);
			found1 = find_sym(operator2);
			found2 = find_sym(operator3);
			if ((C0_SymTable.item[found2].tmp == 1) &&
				CmpString(C0_SymTable.item[found1].kind, "CONSTANT                 ", MAX_OP_LEN) &&
				CmpString(C0_SymTable.item[found].kind, "CONSTANT                 ", MAX_OP_LEN)&&
				(C0_SymTable.item[found2].exp_num == curno)) {
				length++;
				CpyString(C0_SymTable.item[found2].kind, "CONSTANT                 ", MAX_OP_LEN);
				CpyString(aim, C0_SymTable.item[found2].name, MAX_OP_LEN);
				if (CmpString(operation, op[ADD], MAX_OP_LEN)) {
					C0_SymTable.item[found2].value = C0_SymTable.item[found1].value + C0_SymTable.item[found].value;
				}
				else if (CmpString(operation, op[SUB], MAX_OP_LEN)) {
					C0_SymTable.item[found2].value = C0_SymTable.item[found].value - C0_SymTable.item[found1].value;
				}
				else if (CmpString(operation, op[MUL], MAX_OP_LEN)) {
					C0_SymTable.item[found2].value = C0_SymTable.item[found].value * C0_SymTable.item[found1].value;
				}
				else {
					C0_SymTable.item[found2].value = C0_SymTable.item[found].value / C0_SymTable.item[found1].value;
				}
			}
			else {
				if (length != 0) {
					CpyString(C0_Quad.item[i - length].op, op[LODI], MAX_OP_LEN);
					CpyString(C0_Quad.item[i - length].operand1, aim, MAX_OP_LEN);
					found = find_sym(aim);
					get_str(C0_SymTable.item[found].value);
					CpyString(C0_Quad.item[i - length].operand2, vstr, MAX_OP_LEN);
					for (j = i; j < C0_Quad.top; j++) {
						C0_Quad.item[j - length + 1] = C0_Quad.item[j];
					}
					C0_Quad.top = C0_Quad.top - length + 1;
				}
				if (curno != C0_SymTable.item[found2].exp_num) {
					curno = C0_SymTable.item[found2].exp_num;
				}
				length = 0;
			}
		}
		else if (CmpString(C0_Quad.item[i].op, op[NEG], MAX_OP_LEN)) {
			found = find_sym(operator1);
			if (CmpString(C0_SymTable.item[found].kind, "CONSTANT                 ", MAX_OP_LEN)&&
				(C0_SymTable.item[found].exp_num == curno)) {
				length++;
				C0_SymTable.item[found].value = 0 - C0_SymTable.item[found].value;
			}
			else {
				if (length != 0) {
					CpyString(C0_Quad.item[i - length].op, op[LODI], MAX_OP_LEN);
					CpyString(C0_Quad.item[i - length].operand1, aim, MAX_OP_LEN);
					found = find_sym(aim);
					get_str(C0_SymTable.item[found].value);
					CpyString(C0_Quad.item[i - length].operand2, vstr, MAX_OP_LEN);
					for (j = i; j < C0_Quad.top; j++) {
						C0_Quad.item[j - length + 1] = C0_Quad.item[j];
					}
					C0_Quad.top = C0_Quad.top - length + 1;
				}
				if (curno != C0_SymTable.item[found].exp_num) {
					curno = C0_SymTable.item[found].exp_num;
				}
				length = 0;
			}
		}
		else {
			if (length != 0) {
				CpyString(C0_Quad.item[i - length].op, op[LODI], MAX_OP_LEN);
				CpyString(C0_Quad.item[i - length].operand1, aim, MAX_OP_LEN);
				found = find_sym(aim);
				get_str(C0_SymTable.item[found].value);
				CpyString(C0_Quad.item[i - length].operand2, vstr, MAX_OP_LEN);
				for (j = i; j < C0_Quad.top; j++) {
					C0_Quad.item[j - length + 1] = C0_Quad.item[j];
				}
				C0_Quad.top = C0_Quad.top - length + 1;
			}
			if (CmpString(C0_Quad.item[i].op, op[FUNC], MAX_OP_LEN)) {
				found = find_sym(operator1);
				curlev = found;
			}
			else if (CmpString(C0_Quad.item[i].op, op[PARA], MAX_OP_LEN)) {
				found = find_sym(operator1);
				C0_SymTable.item[found].tmp = 0;
				curno = C0_SymTable.item[found].exp_num;
			}
			else if (CmpString(C0_Quad.item[i].op, op[LODARR], MAX_OP_LEN)) {
				found2 = find_sym(operator3);
				C0_SymTable.item[found2].tmp = 0;
				curno = C0_SymTable.item[found2].exp_num;
			}
			else if (CmpString(C0_Quad.item[i].op, op[LODR], MAX_OP_LEN)) {
				found = find_sym(operator1);
				C0_SymTable.item[found].tmp = 0;
				curno = C0_SymTable.item[found].exp_num;
			}
			length = 0;
		}
	}
}
//judge swlw()
int swlw(char str[],char str1[]) {
	int i;
	if ((str[0] == 's') && (str1[0] == 'l')) {
		for (i = 1; str[i- 1] != '\0'&&str1[i-1] != '\0'; i++) {
			if (str[i] == str1[i]) {
				;
			}
			else {//sw $t0,20($sp)
				return 0;
			}
		}
		return 1;
	}
	else
		return 0;
}
int swlw2(char str[], char str1[]) {
	int i;
	if ((str[0] == 's') && (str1[0] == 'l')) {
		if ((str[5] == str1[5])&&(str[4] == str[4])) {
			return 0;
		}
		else {
			for (i = 1; str[i] != '\0'; i++) {
				if ((i == 5)||(i == 4)) {
					;
				}
				else if (str[i] == str1[i]) {
					;
				}
				else {
					return 0;
				}
			}
			return 1;
		}
	}
	else {
		return 0;
	}
}
//judge jlab
int jlab(char str[], char str1[]) {
	int i;
	if (str[0] == 'j') {
		for (i = 2; str[i] != '\0'; i++) {
			if (str[i] == str1[i - 2]) {
				;
			}
			else {
				return 0;
			}
		}
		return 1;
	}
	else {
		return 0;
	}
}
/*peepwhole_opt*/
void peep_hole() {
	int i;
	int swlw_sym,jlab_sym,swlw2_sym;
	char str[500],str1[500];
	gets(str);
	while (gets(str1) != NULL) {
		//elimate sw-lw link
		swlw_sym = swlw(str, str1);
		jlab_sym = jlab(str, str1);
		swlw2_sym = swlw2(str, str1);
		if (swlw_sym) {
			puts(str);
			gets(str1);
		}
		//elimate j-lab link
		else if (jlab_sym) {
			puts(str);
			gets(str1);
		}
		else if (swlw2_sym) {
			puts(str);
			printf("move $%c%c,$%c%c\n", str1[4], str1[5], str[4], str[5]);
			gets(str1);
		}
		else {
			puts(str);
			
		}
		//elimate circle-none-move expression
		for (i = 0; i < 500; i++) {
			str[i] = str1[i];
		}
	}
	return;
}

/*
	origin
*/
void stoarr() {
	int i;
	for (i = begin_point; i <= end_point; i++) {
		CpyString(C0_Quad_opt.item[C0_Quad_opt.top].op, C0_Quad.item[i].op, MAX_OP_LEN);
		CpyString(C0_Quad_opt.item[C0_Quad_opt.top].operand1, C0_Quad.item[i].operand1, MAX_OP_LEN);
		CpyString(C0_Quad_opt.item[C0_Quad_opt.top].operand2, C0_Quad.item[i].operand2, MAX_OP_LEN);
		CpyString(C0_Quad_opt.item[C0_Quad_opt.top].operand3, C0_Quad.item[i].operand3, MAX_OP_LEN);
		C0_Quad_opt.top++;
	}
}
//init DAG:get a  empty nodetable and DAG_graph
void init_DAG() {
	int i;
	for (i = 0; i < 1000; i++) {
		nodetable.item[i].num = 0;
		nodetable.item[i].temp = 1;
	}
	for (i = 0; i < 1000; i++) {
		DAG_graph.item[i].father_len = 0;
		DAG_graph.item[i].name_len = 0;
		DAG_graph.item[i].Lchild = -1;
		DAG_graph.item[i].Rchild = -1;
		DAG_graph.item[i].already = 0;
		CpyString(DAG_graph.item[i].range, normal, MAX_OP_LEN);
	}
	nodetable.top = 0;
	DAG_graph.top = 0;
}
//find the name's number in nodetable
int find_node_in_table(char name[]) {
	int num;
	int i;
	num = -1;
	for (i = 0; i < nodetable.top; i++) {
		if (CmpString(name, nodetable.item[i].name, MAX_OP_LEN)) {
			if (CmpString(DAG_graph.item[nodetable.item[i].num].range, normal, MAX_OP_LEN)) {
				num = i;
				return num;
			}
		}
	}
	return num;
}
//find op in DAG.
//input:op,left node,rightnode.-1 means no left or right
int find_op_in_DAG(char op[], int left, int right) {
	int i;
	int k;
	int num = -1;
	for (i = 0; i < DAG_graph.top; i++) {
		if (CmpString(DAG_graph.item[i].op, op, MAX_OP_LEN) &&
			(DAG_graph.item[i].Lchild == left) && (DAG_graph.item[i].Rchild == right)) {
			if (CmpString(DAG_graph.item[i].range, normal, MAX_OP_LEN)) {
				k = find_node_in_table(DAG_graph.item[i].exp_name);
				if(nodetable.item[k].num == i)
				num = i;
				return num;
			}
		}
	}
	return num;
}
/*
	create a node in table
	input:name,num
	output:node's number.
*/
int create_node_table(char name[], int num) {
	int tmp;
	int found;
	int top;
	found = find_sym(name);
	tmp = C0_SymTable.item[found].tmp;
	top = nodetable.top;
	nodetable.item[top].num = num;
	DAG_graph.item[num].name[DAG_graph.item[num].name_len] = top;
	DAG_graph.item[num].name_len++;
	
	CpyString(DAG_graph.item[num].exp_name, name,MAX_OP_LEN);
	CpyString(nodetable.item[top].name, name,MAX_OP_LEN);
	nodetable.item[top].temp = tmp;
	nodetable.top++;
	return num;
}
//not exist op in DAG,then create one
/*
	input:opname,leftchild.right child
*/
//output:the number of this node
int create_op_DAG(char op[], int left, int right) {
	int top;
	top = DAG_graph.top;
	CpyString(DAG_graph.item[top].op, op, MAX_OP_LEN);
	DAG_graph.item[top].Lchild = left;
	DAG_graph.item[top].Rchild = right;
	if (left != -1) {
		DAG_graph.item[left].father[DAG_graph.item[left].father_len] = top;
		DAG_graph.item[left].father_len ++;
	}
	if (right != -1) {
		DAG_graph.item[right].father[DAG_graph.item[right].father_len] = top;
		DAG_graph.item[right].father_len ++;
	}
	DAG_graph.top++;
	return top;
}
//not exisit the node:create one£¬and add one in the node table.return the node number 
int create_node_DAG(char name[]) {
	int top1, top2;
	int tmp;
	int found;
	found = find_sym(name);
	tmp = C0_SymTable.item[found].tmp;
	top1 = DAG_graph.top;
	top2 = nodetable.top;
	//set the DAG graph
	DAG_graph.item[top1].name[DAG_graph.item[top1].name_len] = top2;
	DAG_graph.item[top1].name_len++;
	CpyString(DAG_graph.item[top1].exp_name, name, MAX_OP_LEN);
	CpyString(nodetable.item[top2].name, name, MAX_OP_LEN);
	//set the node number
	nodetable.item[top2].num = top1;
	nodetable.item[top2].temp = tmp;
	nodetable.top++;
	DAG_graph.top++;
	return top1;
}
//get opt quad
void get_line(int line[],int begin) {
	int i,j,k,m;
	int flag = 0;
	if ((DAG_graph.item[begin].Lchild == -1)) {
		for (i = 0; i < DAG_graph.top; i++) {
			if ((DAG_graph.item[i].Lchild != -1)&&(DAG_graph.item[i].already==0)) {
				//not in the line
				flag = 1;
				for (j = 0; j < line_no; j++) {
					if (i == line[j]) {
						flag = 0;
						break;
					}
				}
				if (flag == 0) {
					;
				}
				else {
					for (k = 0; k < DAG_graph.item[i].father_len; k++) {
						flag = 0;
						for (m = 0; m < line_no; m++) {
							if (line[m] == DAG_graph.item[i].father[k]) {
								flag = 1;
							}
						}
						if (flag == 0)
							break;
					}
					if (flag) {
						line[line_no] = i;
						line_no++;
						get_line(line, i);
					}
				}
			}
		}
		return;
	}
	else {
		i = DAG_graph.item[begin].Lchild;
		if ((DAG_graph.item[i].Lchild != -1)&&(DAG_graph.item[i].already == 0)) {
			flag = 0;
			for (k = 0; k < line_no; k++) {
				if (i == line[k]) {
					flag = 1;
					break;
				}
			}
			if (flag == 1) {
				;
			}
			else {
				for (k = 0; k < DAG_graph.item[i].father_len; k++) {
					flag = 1;
					for (m = 0; m < line_no; m++) {
						if (line[m] == DAG_graph.item[i].father[k]) {
							flag = 0;
						}
					}
					if (flag == 1)
						break;
				}
			}
			if (flag == 0) {
				line[line_no] = i;
				line_no++;
			}
		}
		get_line(line,  i);
		return;
	}
}
//decide final name
void decide_name() {
	int i,j;
	//int max;
	/*for (i = 0; i < DAG_graph.top; i++) {
		max = -1;
		if (DAG_graph.item[i].name_len > 1) {
			for (j = 0; j < DAG_graph.item[i].name_len; j++) {
				if (nodetable.item[DAG_graph.item[i].name[j]].num > max) {
					max = nodetable.item[DAG_graph.item[i].name[j]].num;
					CpyString(DAG_graph.item[i].exp_name, nodetable.item[DAG_graph.item[i].name[j]].name, MAX_OP_LEN);
				}
			}
		}
	}*/
	for (i = 0; i < DAG_graph.top; i++) {
		if (DAG_graph.item[i].name_len > 1) {
			CpyString(DAG_graph.item[i].exp_name, nodetable.item[DAG_graph.item[i].name[0]].name, MAX_OP_LEN);
		}
	}
}
void get_opt_quad_noinit(char lab[]) {
	int top = C0_Quad_opt.top;
	char *operation;
	int i,k;
	int com;
	int left, right;
	int op1_num, op2_num, result_num;
	int line[400];
	begin_point = end_point+1;
	line_no = 0;
	// travel all the node in DAG_graph to find one
	for (i = 0; i < DAG_graph.top; i++) {
		if ((DAG_graph.item[i].father_len == 0) && (DAG_graph.item[i].Lchild != -1)&&(DAG_graph.item[i].already != 1)) {
			break;
		}
	}
	if (i == DAG_graph.top)
		return;
	line[line_no] = i;
	line_no++;
	//initial is over
	//recursive to get line
	get_line(line, i);
	decide_name();
	//get the quad
	for (k = line_no - 1; k >= 0; k--) {
		com = line[k];
		DAG_graph.item[com].already = 1;
		left = DAG_graph.item[com].Lchild;
		right = DAG_graph.item[com].Rchild;
		CpyString(DAG_graph.item[com].range, lab, MAX_OP_LEN);
		if (CmpString(DAG_graph.item[com].op, op[ADD], MAX_OP_LEN)) {
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].op, op[ADD], MAX_OP_LEN);
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].operand1, DAG_graph.item[left].exp_name, MAX_OP_LEN);
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].operand2, DAG_graph.item[right].exp_name, MAX_OP_LEN);
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].operand3, DAG_graph.item[com].exp_name, MAX_OP_LEN);
			C0_Quad_opt.top++;
		}
		else if(CmpString(DAG_graph.item[com].op, op[SUB], MAX_OP_LEN)) {
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].op, op[SUB], MAX_OP_LEN);
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].operand1, DAG_graph.item[left].exp_name, MAX_OP_LEN);
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].operand2, DAG_graph.item[right].exp_name, MAX_OP_LEN);
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].operand3, DAG_graph.item[com].exp_name, MAX_OP_LEN);
			C0_Quad_opt.top++;
		}
		else if (CmpString(DAG_graph.item[com].op, op[MUL], MAX_OP_LEN)) {
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].op, op[MUL], MAX_OP_LEN);
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].operand1, DAG_graph.item[left].exp_name, MAX_OP_LEN);
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].operand2, DAG_graph.item[right].exp_name, MAX_OP_LEN);
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].operand3, DAG_graph.item[com].exp_name, MAX_OP_LEN);
			C0_Quad_opt.top++;
		}
		else if (CmpString(DAG_graph.item[com].op, op[DIVIDE], MAX_OP_LEN)) {
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].op, op[DIVIDE], MAX_OP_LEN);
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].operand1, DAG_graph.item[left].exp_name, MAX_OP_LEN);
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].operand2, DAG_graph.item[right].exp_name, MAX_OP_LEN);
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].operand3, DAG_graph.item[com].exp_name, MAX_OP_LEN);
			C0_Quad_opt.top++;
		}
		else if (CmpString(DAG_graph.item[com].op, op[NEG], MAX_OP_LEN)) {
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].op, op[NEG], MAX_OP_LEN);
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].operand1, DAG_graph.item[left].exp_name, MAX_OP_LEN);
			C0_Quad_opt.top++;
		}
		else if (CmpString(DAG_graph.item[com].op, op[LODV], MAX_OP_LEN)) {
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].op, op[LODV], MAX_OP_LEN);
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].operand1, DAG_graph.item[com].exp_name, MAX_OP_LEN);
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].operand2, DAG_graph.item[left].exp_name, MAX_OP_LEN);
			C0_Quad_opt.top++;
		}
		else if (CmpString(DAG_graph.item[com].op, op[LODI], MAX_OP_LEN)) {
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].op, op[LODI], MAX_OP_LEN);
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].operand1, DAG_graph.item[com].exp_name, MAX_OP_LEN);
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].operand2, DAG_graph.item[left].exp_name, MAX_OP_LEN);
			C0_Quad_opt.top++;
		}
		else if (CmpString(DAG_graph.item[com].op, op[LODR], MAX_OP_LEN)) {
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].op, op[LODR], MAX_OP_LEN);
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].operand1, DAG_graph.item[com].exp_name, MAX_OP_LEN);
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].operand2, DAG_graph.item[left].exp_name, MAX_OP_LEN);
			C0_Quad_opt.top++;
		}
		else if (CmpString(DAG_graph.item[com].op, op[PARA], MAX_OP_LEN)) {
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].op, DAG_graph.item[com].op, MAX_OP_LEN);
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].operand1, DAG_graph.item[left].exp_name, MAX_OP_LEN);
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].operand2, DAG_graph.item[right].exp_name, MAX_OP_LEN);
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].operand3, DAG_graph.item[com].exp_name, MAX_OP_LEN);
			C0_Quad_opt.top++;
		}
		else if (CmpString(DAG_graph.item[com].op, op[PRT], MAX_OP_LEN)) {
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].op, DAG_graph.item[com].op, MAX_OP_LEN);
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].operand1, DAG_graph.item[left].exp_name, MAX_OP_LEN);
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].operand2, DAG_graph.item[right].exp_name, MAX_OP_LEN);
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].operand3, DAG_graph.item[com].exp_name, MAX_OP_LEN);
			C0_Quad_opt.top++;
		}
		else if (CmpString(DAG_graph.item[com].op, op[SCF], MAX_OP_LEN)) {
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].op, DAG_graph.item[com].op, MAX_OP_LEN);
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].operand1, DAG_graph.item[left].exp_name, MAX_OP_LEN);
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].operand2, DAG_graph.item[right].exp_name, MAX_OP_LEN);
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].operand3, DAG_graph.item[com].exp_name, MAX_OP_LEN);
			C0_Quad_opt.top++;
		}
		else if (CmpString(DAG_graph.item[com].op, op[CALL], MAX_OP_LEN)) {
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].op, DAG_graph.item[com].op, MAX_OP_LEN);
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].operand1, DAG_graph.item[left].exp_name, MAX_OP_LEN);
			C0_Quad_opt.top++;
		}
		else if (CmpString(DAG_graph.item[com].op, op[JMP], MAX_OP_LEN)) {
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].op, DAG_graph.item[com].op, MAX_OP_LEN);
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].operand1, DAG_graph.item[left].exp_name, MAX_OP_LEN);
			C0_Quad_opt.top++;
		}
		else if (CmpString(DAG_graph.item[com].op, op[RET], MAX_OP_LEN)) {
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].op, DAG_graph.item[com].op, MAX_OP_LEN);
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].operand1, DAG_graph.item[left].exp_name, MAX_OP_LEN);
			C0_Quad_opt.top++;
		}
		else if (CmpString(DAG_graph.item[com].op, op[GQL], MAX_OP_LEN) ||
			CmpString(DAG_graph.item[com].op, op[GRT], MAX_OP_LEN) ||
			CmpString(DAG_graph.item[com].op, op[LESS], MAX_OP_LEN) ||
			CmpString(DAG_graph.item[com].op, op[LQL], MAX_OP_LEN) ||
			CmpString(DAG_graph.item[com].op, op[EQU], MAX_OP_LEN) ||
			CmpString(DAG_graph.item[com].op, op[NQL], MAX_OP_LEN)) {
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].op, DAG_graph.item[com].op, MAX_OP_LEN);
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].operand1, DAG_graph.item[left].exp_name, MAX_OP_LEN);
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].operand2, DAG_graph.item[right].exp_name, MAX_OP_LEN);
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].operand3, DAG_graph.item[com].exp_name, MAX_OP_LEN);
			C0_Quad_opt.top++;
		}
		else if (CmpString(DAG_graph.item[com].op, op[LODARR], MAX_OP_LEN)||
			CmpString(DAG_graph.item[com].op, op[STOARR], MAX_OP_LEN)) {
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].op, DAG_graph.item[com].op, MAX_OP_LEN);
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].operand1, DAG_graph.item[left].exp_name, MAX_OP_LEN);
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].operand2, DAG_graph.item[right].exp_name, MAX_OP_LEN);
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].operand3, DAG_graph.item[com].exp_name, MAX_OP_LEN);
			C0_Quad_opt.top++;
		}
	}
}
void get_opt_quad() {
	int top = C0_Quad_opt.top;
	char *operation;
	int i, k;
	int com;
	int left, right;
	int op1_num, op2_num, result_num;
	int line[400];
	begin_point = end_point+1;
	line_no = 0;
	// travel all the node in DAG_graph to find one
	for (i = 0; i < DAG_graph.top; i++) {
		if ((DAG_graph.item[i].father_len == 0) && (DAG_graph.item[i].Lchild != -1)&&(DAG_graph.item[i].already!=1)) {
			break;
		}
	}
	if (i == DAG_graph.top)
		return;
	line[line_no] = i;
	line_no++;
	//initial is over
	//recursive to get line
	get_line(line, i);
	decide_name();
	//get the quad
	for (k = line_no - 1; k >= 0; k--) {
		com = line[k];
		DAG_graph.item[com].already = 1;
		left = DAG_graph.item[com].Lchild;
		right = DAG_graph.item[com].Rchild;
		if (CmpString(DAG_graph.item[com].op, op[ADD], MAX_OP_LEN)) {
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].op, op[ADD], MAX_OP_LEN);
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].operand1, DAG_graph.item[left].exp_name, MAX_OP_LEN);
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].operand2, DAG_graph.item[right].exp_name, MAX_OP_LEN);
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].operand3, DAG_graph.item[com].exp_name, MAX_OP_LEN);
			C0_Quad_opt.top++;
		}
		else if (CmpString(DAG_graph.item[com].op, op[SUB], MAX_OP_LEN)) {
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].op, op[SUB], MAX_OP_LEN);
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].operand1, DAG_graph.item[left].exp_name, MAX_OP_LEN);
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].operand2, DAG_graph.item[right].exp_name, MAX_OP_LEN);
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].operand3, DAG_graph.item[com].exp_name, MAX_OP_LEN);
			C0_Quad_opt.top++;
		}
		else if (CmpString(DAG_graph.item[com].op, op[MUL], MAX_OP_LEN)) {
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].op, op[MUL], MAX_OP_LEN);
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].operand1, DAG_graph.item[left].exp_name, MAX_OP_LEN);
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].operand2, DAG_graph.item[right].exp_name, MAX_OP_LEN);
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].operand3, DAG_graph.item[com].exp_name, MAX_OP_LEN);
			C0_Quad_opt.top++;
		}
		else if (CmpString(DAG_graph.item[com].op, op[DIVIDE], MAX_OP_LEN)) {
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].op, op[DIVIDE], MAX_OP_LEN);
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].operand1, DAG_graph.item[left].exp_name, MAX_OP_LEN);
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].operand2, DAG_graph.item[right].exp_name, MAX_OP_LEN);
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].operand3, DAG_graph.item[com].exp_name, MAX_OP_LEN);
			C0_Quad_opt.top++;
		}
		else if (CmpString(DAG_graph.item[com].op, op[NEG], MAX_OP_LEN)) {
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].op, op[NEG], MAX_OP_LEN);
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].operand1, DAG_graph.item[left].exp_name, MAX_OP_LEN);
			C0_Quad_opt.top++;
		}
		else if (CmpString(DAG_graph.item[com].op, op[LODV], MAX_OP_LEN)) {
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].op, op[LODV], MAX_OP_LEN);
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].operand1, DAG_graph.item[com].exp_name, MAX_OP_LEN);
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].operand2, DAG_graph.item[left].exp_name, MAX_OP_LEN);
			C0_Quad_opt.top++;
		}
		else if (CmpString(DAG_graph.item[com].op, op[LODI], MAX_OP_LEN)) {
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].op, op[LODI], MAX_OP_LEN);
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].operand1, DAG_graph.item[com].exp_name, MAX_OP_LEN);
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].operand2, DAG_graph.item[left].exp_name, MAX_OP_LEN);
			C0_Quad_opt.top++;
		}
		else if (CmpString(DAG_graph.item[com].op, op[LODR], MAX_OP_LEN)) {
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].op, op[LODR], MAX_OP_LEN);
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].operand1, DAG_graph.item[com].exp_name, MAX_OP_LEN);
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].operand2, DAG_graph.item[left].exp_name, MAX_OP_LEN);
			C0_Quad_opt.top++;
		}
		else if (CmpString(DAG_graph.item[com].op, op[PARA], MAX_OP_LEN)) {
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].op, DAG_graph.item[com].op, MAX_OP_LEN);
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].operand1, DAG_graph.item[left].exp_name, MAX_OP_LEN);
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].operand2, DAG_graph.item[right].exp_name, MAX_OP_LEN);
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].operand3, DAG_graph.item[com].exp_name, MAX_OP_LEN);
			C0_Quad_opt.top++;
		}
		else if (CmpString(DAG_graph.item[com].op, op[PRT], MAX_OP_LEN)) {
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].op, DAG_graph.item[com].op, MAX_OP_LEN);
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].operand1, DAG_graph.item[left].exp_name, MAX_OP_LEN);
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].operand2, DAG_graph.item[right].exp_name, MAX_OP_LEN);
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].operand3, DAG_graph.item[com].exp_name, MAX_OP_LEN);
			C0_Quad_opt.top++;
		}
		else if (CmpString(DAG_graph.item[com].op, op[SCF], MAX_OP_LEN)) {
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].op, DAG_graph.item[com].op, MAX_OP_LEN);
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].operand1, DAG_graph.item[left].exp_name, MAX_OP_LEN);
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].operand2, DAG_graph.item[right].exp_name, MAX_OP_LEN);
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].operand3, DAG_graph.item[com].exp_name, MAX_OP_LEN);
			C0_Quad_opt.top++;
		}
		else if (CmpString(DAG_graph.item[com].op, op[CALL], MAX_OP_LEN)) {
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].op, DAG_graph.item[com].op, MAX_OP_LEN);
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].operand1, DAG_graph.item[left].exp_name, MAX_OP_LEN);
			C0_Quad_opt.top++;
		}
		else if (CmpString(DAG_graph.item[com].op, op[JMP], MAX_OP_LEN)) {
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].op, DAG_graph.item[com].op, MAX_OP_LEN);
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].operand1, DAG_graph.item[left].exp_name, MAX_OP_LEN);
			C0_Quad_opt.top++;
		}
		else if (CmpString(DAG_graph.item[com].op, op[RET], MAX_OP_LEN)) {
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].op, DAG_graph.item[com].op, MAX_OP_LEN);
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].operand1, DAG_graph.item[left].exp_name, MAX_OP_LEN);
			C0_Quad_opt.top++;
		}
		else if (CmpString(DAG_graph.item[com].op, op[GQL], MAX_OP_LEN)||
			CmpString(DAG_graph.item[com].op,op[GRT],MAX_OP_LEN)||
			CmpString(DAG_graph.item[com].op,op[LESS],MAX_OP_LEN)||
			CmpString(DAG_graph.item[com].op, op[LQL], MAX_OP_LEN) || 
			CmpString(DAG_graph.item[com].op, op[EQU], MAX_OP_LEN) || 
			CmpString(DAG_graph.item[com].op, op[NQL], MAX_OP_LEN)) {
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].op, DAG_graph.item[com].op, MAX_OP_LEN);
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].operand1, DAG_graph.item[left].exp_name, MAX_OP_LEN);
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].operand2, DAG_graph.item[right].exp_name, MAX_OP_LEN);
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].operand3, DAG_graph.item[com].exp_name, MAX_OP_LEN);
			C0_Quad_opt.top++;
		}
		else if (CmpString(DAG_graph.item[com].op, op[LODARR], MAX_OP_LEN) ||
			CmpString(DAG_graph.item[com].op, op[STOARR], MAX_OP_LEN)) {
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].op, DAG_graph.item[com].op, MAX_OP_LEN);
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].operand1, DAG_graph.item[left].exp_name, MAX_OP_LEN);
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].operand2, DAG_graph.item[right].exp_name, MAX_OP_LEN);
			CpyString(C0_Quad_opt.item[C0_Quad_opt.top].operand3, DAG_graph.item[com].exp_name, MAX_OP_LEN);
			C0_Quad_opt.top++;
		}
	}
	init_DAG();
}
void not_value_type(char operation[], char operator1[], char operator2[], char operator3[]) {
	int top = C0_Quad_opt.top;
	CpyString(C0_Quad_opt.item[top].op, operation, MAX_OP_LEN);
	CpyString(C0_Quad_opt.item[top].operand1, operator1, MAX_OP_LEN);
	CpyString(C0_Quad_opt.item[top].operand2, operator2, MAX_OP_LEN);
	CpyString(C0_Quad_opt.item[top].operand3, operator3, MAX_OP_LEN);
	C0_Quad_opt.top++;
}
/*
	void DAG()
	Input:C0_Quad.C0_Basics
	Output:C0_Quad_opt
*/
void DAG() {
	int t;
	int i,j;
	int count;
	//Basic begin and end
	int begin, end;
	char *operation, *operator1, *operator2, *operator3; 
	char *n;
	int nodenum1,nodenum2,nodenum3,nodenum4;
	nodetable.top = 0;
	DAG_graph.top = 0;
	begin_point = 0;
	end_point = 0;
	for (i = 0; i < C0_Basic.top; i++) {
		begin = C0_Basic.item[i].begin;
		end = C0_Basic.item[i].end;
		//init the table and the graph
		init_DAG();
		s = 0;
		for (j = begin; j <= end; j++) {
			end_point = j;
			//get op operator1 operator2 operator3
			operation = C0_Quad.item[j].op;
			operator1 = C0_Quad.item[j].operand1;
			operator2 = C0_Quad.item[j].operand2;
			operator3 = C0_Quad.item[j].operand3;
			
			//this is a cal operation
			
			if (CmpString(operation, op[ADD], MAX_OP_LEN)) {
				//ADD <OP1>,<OP2>,<RESULT>
				//find op1 in table
				nodenum1 = find_node_in_table(operator1);
				if (nodenum1 == -1) {//not find
					nodenum1 = create_node_DAG(operator1);
				}
				else {
					nodenum1 = nodetable.item[nodenum1].num;
				}
				//find op2 in table
				nodenum2 = find_node_in_table(operator2);
				if (nodenum2 == -1) {//not find
					nodenum2 = create_node_DAG(operator2);
				}
				else {
					nodenum2 = nodetable.item[nodenum2].num;
				}
				//find op in DAG
				nodenum3 = find_op_in_DAG(operation, nodenum1, nodenum2);
				if (nodenum3 == -1) {
					nodenum3 = create_op_DAG(operation, nodenum1, nodenum2);
				}//not found
				// find result in table
				nodenum4 = find_node_in_table(operator3);
				if (nodenum4 != -1) {
					nodetable.item[nodenum4].num = nodenum3;
					DAG_graph.item[nodenum3].name[DAG_graph.item[nodenum3].name_len] = nodenum4;
					DAG_graph.item[nodenum3].name_len++;
					CpyString(DAG_graph.item[nodenum3].exp_name, operator3, MAX_OP_LEN);
				}
				else {
					nodenum4 = create_node_table(operator3, nodenum3);
				}
			}
			else if(CmpString(operation, op[SUB], MAX_OP_LEN)) {
				//ADD <OP1>,<OP2>,<RESULT>
				//find op1 in table
				nodenum1 = find_node_in_table(operator1);
				if (nodenum1 == -1) {//not find
					nodenum1 = create_node_DAG(operator1);
				}
				else {
					nodenum1 = nodetable.item[nodenum1].num;
				}
				//find op2 in table
				nodenum2 = find_node_in_table(operator2);
				if (nodenum2 == -1) {//not find
					nodenum2 = create_node_DAG(operator2);
				}
				else {
					nodenum2 = nodetable.item[nodenum2].num;
				}
				//find op in DAG
				nodenum3 = find_op_in_DAG(operation, nodenum1, nodenum2);
				if (nodenum3 == -1) {
					nodenum3 = create_op_DAG(operation, nodenum1, nodenum2);
				}//not found
				// find result in table
				nodenum4 = find_node_in_table(operator3);
				if (nodenum4 != -1) {
					nodetable.item[nodenum4].num = nodenum3;
					DAG_graph.item[nodenum3].name[DAG_graph.item[nodenum3].name_len] = nodenum4;
					DAG_graph.item[nodenum3].name_len++;
					CpyString(DAG_graph.item[nodenum3].exp_name, operator3, MAX_OP_LEN);
				}
				else {
					nodenum4 = create_node_table(operator3, nodenum3);
				}
			}
			else if (CmpString(operation, op[DIVIDE], MAX_OP_LEN)) {
				//ADD <OP1>,<OP2>,<RESULT>
				//find op1 in table
				nodenum1 = find_node_in_table(operator1);
				if (nodenum1 == -1) {//not find
					nodenum1 = create_node_DAG(operator1);
				}
				else {
					nodenum1 = nodetable.item[nodenum1].num;
				}
				//find op2 in table
				nodenum2 = find_node_in_table(operator2);
				if (nodenum2 == -1) {//not find
					nodenum2 = create_node_DAG(operator2);
				}
				else {
					nodenum2 = nodetable.item[nodenum2].num;
				}
				//find op in DAG
				nodenum3 = find_op_in_DAG(operation, nodenum1, nodenum2);
				if (nodenum3 == -1) {
					nodenum3 = create_op_DAG(operation, nodenum1, nodenum2);
				}//not found
				// find result in table
				nodenum4 = find_node_in_table(operator3);
				if (nodenum4 != -1) {
					nodetable.item[nodenum4].num = nodenum3;
					DAG_graph.item[nodenum3].name[DAG_graph.item[nodenum3].name_len] = nodenum4;
					DAG_graph.item[nodenum3].name_len++;
					CpyString(DAG_graph.item[nodenum3].exp_name, operator3, MAX_OP_LEN);
				}
				else {
					nodenum4 = create_node_table(operator3, nodenum3);
				}
			}
			else if (CmpString(operation, op[MUL], MAX_OP_LEN)) {
				//ADD <OP1>,<OP2>,<RESULT>
				//find op1 in table
				nodenum1 = find_node_in_table(operator1);
				if (nodenum1 == -1) {//not find
					nodenum1 = create_node_DAG(operator1);
				}
				else {
					nodenum1 = nodetable.item[nodenum1].num;
				}
				//find op2 in table
				nodenum2 = find_node_in_table(operator2);
				if (nodenum2 == -1) {//not find
					nodenum2 = create_node_DAG(operator2);
				}
				else {
					nodenum2 = nodetable.item[nodenum2].num;
				}
				//find op in DAG
				nodenum3 = find_op_in_DAG(operation, nodenum1, nodenum2);
				if (nodenum3 == -1) {
					nodenum3 = create_op_DAG(operation, nodenum1, nodenum2);
				}//not found
				// find result in table
				nodenum4 = find_node_in_table(operator3);
				if (nodenum4 != -1) {
					nodetable.item[nodenum4].num = nodenum3;
					DAG_graph.item[nodenum3].name[DAG_graph.item[nodenum3].name_len] = nodenum4;
					DAG_graph.item[nodenum3].name_len++;
					CpyString(DAG_graph.item[nodenum3].exp_name, operator3, MAX_OP_LEN);
				}
				else {
					nodenum4 = create_node_table(operator3, nodenum3);
				}
			}
			else if (CmpString(operation, op[NEG], MAX_OP_LEN)) {
				//NEG <OP>
				//find op1 in table
				nodenum1 = find_node_in_table(operator1);
				if (nodenum1 == -1) {//not find
					nodenum1 = create_node_DAG(operator1);
				}
				else {
					nodenum1 = nodetable.item[nodenum1].num;
				}
				//find op in DAG
				nodenum2 = find_op_in_DAG(operation, nodenum1, -1);
				if (nodenum2 == -1) {
					nodenum2 = create_op_DAG(operation, nodenum1, -1);
				}//not found
				// find result in table
				nodenum3 = find_node_in_table(operator1);
				if (nodenum3 != -1) {
					nodetable.item[nodenum3].num = nodenum2;
					DAG_graph.item[nodenum2].name[DAG_graph.item[nodenum2].name_len] = nodenum3;
					DAG_graph.item[nodenum2].name_len++;
					CpyString(DAG_graph.item[nodenum2].exp_name, operator1, MAX_OP_LEN);
				}
				else {
					nodenum3 = create_node_table(operator1, nodenum2);
				}
			}
			else if (CmpString(operation, op[LODI], MAX_OP_LEN)) {
				//LODI <NAME>,<VALUE>
				//find node in table
				nodenum1 = find_node_in_table(operator2);
				if (nodenum1 == -1) {//not find
					nodenum1 = create_node_DAG(operator2);
				}
				else {
					nodenum1 = nodetable.item[nodenum1].num;
				}
				//find op in DAG
				nodenum2 = find_op_in_DAG(operation, nodenum1, -1);
				if (nodenum2 == -1) {
					nodenum2 = create_op_DAG(operation, nodenum1, -1);
				}//not found
				// find result in table
				nodenum3 = find_node_in_table(operator1);
				if (nodenum3 != -1) {
					CpyString(DAG_graph.item[nodenum2].exp_name, operator1, MAX_OP_LEN);
					DAG_graph.item[nodenum2].name[DAG_graph.item[nodenum2].name_len] = nodenum3;
					DAG_graph.item[nodenum2].name_len++;
					nodetable.item[nodenum3].num = nodenum2;
				}
				else {
					nodenum3 = create_node_table(operator1, nodenum2);
				}
			}
			else if (CmpString(operation, op[LODV], MAX_OP_LEN)) {
				//LODV <TMP>,<SRC>
				//find node in table
				nodenum1 = find_node_in_table(operator2);
				if (nodenum1 == -1) {//not found
					nodenum1 = create_node_DAG(operator2);
				}
				else {
					nodenum1 = nodetable.item[nodenum1].num;
				}
				//find op in DAG
				nodenum2 = find_op_in_DAG(operation, nodenum1, -1);
				if (nodenum2 == -1) {//not found
					nodenum2 = create_op_DAG(operation, nodenum1, -1);
				}
				//find result in table
				nodenum3 = find_node_in_table(operator1);
				if (nodenum3 != -1) {
					nodetable.item[nodenum3].num = nodenum2;
					DAG_graph.item[nodenum2].name[DAG_graph.item[nodenum2].name_len] = nodenum3;
					DAG_graph.item[nodenum2].name_len++;
					CpyString(DAG_graph.item[nodenum2].exp_name, operator1, MAX_OP_LEN);
				}
				else {
					nodenum3 = create_node_table(operator1, nodenum2);
				}
			}
			else if (CmpString(operation, op[LODR], MAX_OP_LEN)) {
				//LODR <NAME>,<REG>
				//find node in table
				nodenum1 = find_node_in_table(operator2);
				if (nodenum1 == -1) {//not find
					nodenum1 = create_node_DAG(operator2);
				}
				else {
					nodenum1 = nodetable.item[nodenum1].num;
				}
				//find op in DAG
				nodenum2 = create_op_DAG(operation, nodenum1, -1);
				//not found
				// find result in table
				nodenum3 = find_node_in_table(operator1);
				if (nodenum3 != -1) {
					nodetable.item[nodenum3].num = nodenum2;
					DAG_graph.item[nodenum2].name[DAG_graph.item[nodenum2].name_len] = nodenum3;
					DAG_graph.item[nodenum2].name_len++;
					CpyString(DAG_graph.item[nodenum2].exp_name, operator1, MAX_OP_LEN);
				}
				else {
					nodenum3 = create_node_table(operator1, nodenum2);
				}
			}
			else if (CmpString(operation, op[PARA], MAX_OP_LEN)) {
				//para <name>,<num>,<func>
				//ADD <OP1>,<OP2>,<RESULT>
				//find op1 in table
				nodenum1 = find_node_in_table(operator1);
				if (nodenum1 == -1) {//not find
					nodenum1 = create_node_DAG(operator1);
				}
				else {
					nodenum1 = nodetable.item[nodenum1].num;
				}
				//find op2 in table
				nodenum2 = find_node_in_table(operator2);
				if (nodenum2 == -1) {//not find
					nodenum2 = create_node_DAG(operator2);
				}
				else {
					nodenum2 = nodetable.item[nodenum2].num;
				}
				//find op in DAG
				nodenum3 = find_op_in_DAG(operation, nodenum1, nodenum2);
				if (nodenum3 == -1) {
					nodenum3 = create_op_DAG(operation, nodenum1, nodenum2);
				}//not found
				// find result in table
				nodenum4 = find_node_in_table(operator3);
				if (nodenum4 != -1) {
					nodetable.item[nodenum4].num = nodenum3;
					DAG_graph.item[nodenum3].name[DAG_graph.item[nodenum3].name_len] = nodenum4;
					DAG_graph.item[nodenum3].name_len++;
					CpyString(DAG_graph.item[nodenum3].exp_name, operator3, MAX_OP_LEN);
				}
				else {
					nodenum4 = create_node_table(operator3, nodenum3);
				}
			}
			else if (CmpString(operation, op[GQL], MAX_OP_LEN) ||
				CmpString(operation, op[GRT], MAX_OP_LEN) ||
				CmpString(operation, op[LESS], MAX_OP_LEN) ||
				CmpString(operation, op[LQL], MAX_OP_LEN) ||
				CmpString(operation, op[EQU], MAX_OP_LEN) ||
				CmpString(operation, op[NQL], MAX_OP_LEN)) {
				//GQL <OP1>,<OP2>,<LAB>
				//ADD <OP1>,<OP2>,<RESULT>
				//find op1 in table
				get_opt_quad_noinit(normal);
				nodenum1 = find_node_in_table(operator1);
				if (nodenum1 == -1) {//not find
					nodenum1 = create_node_DAG(operator1);
				}
				else {
					nodenum1 = nodetable.item[nodenum1].num;
				}
				//find op2 in table
				nodenum2 = find_node_in_table(operator2);
				if (nodenum2 == -1) {//not find
					nodenum2 = create_node_DAG(operator2);
				}
				else {
					nodenum2 = nodetable.item[nodenum2].num;
				}
				//find op in DAG
				nodenum3 = find_op_in_DAG(operation, nodenum1, nodenum2);
				if (nodenum3 == -1) {
					nodenum3 = create_op_DAG(operation, nodenum1, nodenum2);
				}//not found
				// find result in table
				nodenum4 = find_node_in_table(operator3);
				if (nodenum4 != -1) {
					nodetable.item[nodenum4].num = nodenum3;
					DAG_graph.item[nodenum3].name[DAG_graph.item[nodenum3].name_len] = nodenum4;
					DAG_graph.item[nodenum3].name_len++;
					CpyString(DAG_graph.item[nodenum3].exp_name, operator3, MAX_OP_LEN);
				}
				else {
					nodenum4 = create_node_table(operator3, nodenum3);
				}
				get_opt_quad_noinit(normal);
			}
			else if (CmpString(operation, op[PRT], MAX_OP_LEN)) {
				get_opt_quad_noinit(normal);
				nodenum1 = find_node_in_table(operator1);
				if (nodenum1 == -1) {//not find
					nodenum1 = create_node_DAG(operator1);
				}
				else {
					nodenum1 = nodetable.item[nodenum1].num;
				}
				//find op2 in table
				nodenum2 = find_node_in_table(operator2);
				if (nodenum2 == -1) {//not find
					nodenum2 = create_node_DAG(operator2);
				}
				else {
					nodenum2 = nodetable.item[nodenum2].num;
				}
				//find op in DAG
				nodenum3 = find_op_in_DAG(operation, nodenum1, nodenum2);
				if (nodenum3 == -1) {
					nodenum3 = create_op_DAG(operation, nodenum1, nodenum2);
				}//not found
				// find result in table
				nodenum4 = find_node_in_table(operator3);
				if (nodenum4 != -1) {
					nodetable.item[nodenum4].num = nodenum3;
					DAG_graph.item[nodenum3].name[DAG_graph.item[nodenum3].name_len] = nodenum4;
					DAG_graph.item[nodenum3].name_len++;
					CpyString(DAG_graph.item[nodenum3].exp_name, operator3, MAX_OP_LEN);
				}
				else {
					nodenum4 = create_node_table(operator3, nodenum3);
				}
				get_opt_quad_noinit(normal);
			}
			else if (CmpString(operation, op[SCF], MAX_OP_LEN)) {
				//SCF <OP>
				//find op1 in table
				get_opt_quad_noinit(normal);
				nodenum1 = find_node_in_table(operator1);
				if (nodenum1 == -1) {//not find
					nodenum1 = create_node_DAG(operator1);
				}
				else {
					nodenum1 = nodetable.item[nodenum1].num;
				}
				//find op in DAG
				nodenum2 = find_op_in_DAG(operation, nodenum1, -1);
				if (nodenum2 == -1) {
					nodenum2 = create_op_DAG(operation, nodenum1, -1);
				}//not found
				// find result in table
				nodenum3 = find_node_in_table(operator1);
				if (nodenum3 != -1) {
					nodetable.item[nodenum3].num = nodenum2;
					DAG_graph.item[nodenum2].name[DAG_graph.item[nodenum2].name_len] = nodenum3;
					DAG_graph.item[nodenum2].name_len++;
					CpyString(DAG_graph.item[nodenum2].exp_name, operator1, MAX_OP_LEN);
				}
				else {
					nodenum3 = create_node_table(operator1, nodenum2);
				}
				get_opt_quad_noinit(normal);
			}
			else if (CmpString(operation, op[CALL], MAX_OP_LEN)) {
				//NEG <OP>
				//find op1 in table
				get_opt_quad_noinit(normal);
				nodenum1 = find_node_in_table(operator1);
				if (nodenum1 == -1) {//not find
					nodenum1 = create_node_DAG(operator1);
				}
				else {
					nodenum1 = nodetable.item[nodenum1].num;
				}
				//find op in DAG
				nodenum2 = find_op_in_DAG(operation, nodenum1, -1);
				if (nodenum2 == -1) {
					nodenum2 = create_op_DAG(operation, nodenum1, -1);
				}//not found
				// find result in table
				nodenum3 = find_node_in_table(operator1);
				if (nodenum3 != -1) {
					nodetable.item[nodenum3].num = nodenum2;
					DAG_graph.item[nodenum2].name[DAG_graph.item[nodenum2].name_len] = nodenum3;
					DAG_graph.item[nodenum2].name_len++;
					CpyString(DAG_graph.item[nodenum2].exp_name, operator1, MAX_OP_LEN);
				}
				else {
					nodenum3 = create_node_table(operator1, nodenum2);
				}
				get_opt_quad_noinit(normal);
			}
			else if(CmpString(operation,op[JMP],MAX_OP_LEN)){
				//JMP <OP>
				//find op1 in table
				get_opt_quad_noinit(operator1);
				nodenum1 = find_node_in_table(operator1);
				if (nodenum1 == -1) {//not find
					nodenum1 = create_node_DAG(operator1);
				}
				else {
					nodenum1 = nodetable.item[nodenum1].num;
				}
				//find op in DAG
				nodenum2 = find_op_in_DAG(operation, nodenum1, -1);
				if (nodenum2 == -1) {
					nodenum2 = create_op_DAG(operation, nodenum1, -1);
				}//not found
				// find result in table
				nodenum3 = find_node_in_table(operator1);
				if (nodenum3 != -1) {
					nodetable.item[nodenum3].num = nodenum2;
					DAG_graph.item[nodenum2].name[DAG_graph.item[nodenum2].name_len] = nodenum3;
					DAG_graph.item[nodenum2].name_len++;
					CpyString(DAG_graph.item[nodenum2].exp_name, operator1, MAX_OP_LEN);
				}
				else {
					nodenum3 = create_node_table(operator1, nodenum2);
				}
				get_opt_quad_noinit(operator1);
			}
			else if (CmpString(operation, op[RET], MAX_OP_LEN)) {
				//Nret <OP>
				//find op1 in table
				get_opt_quad_noinit(notype);
				nodenum1 = find_node_in_table(operator1);
				if (nodenum1 == -1) {//not find
					nodenum1 = create_node_DAG(operator1);
				}
				else {
					nodenum1 = nodetable.item[nodenum1].num;
				}
				//find op in DAG
				nodenum2 = find_op_in_DAG(operation, nodenum1, -1);
				if (nodenum2 == -1) {
					nodenum2 = create_op_DAG(operation, nodenum1, -1);
				}//not found
				// find result in table
				nodenum3 = find_node_in_table(operator1);
				if (nodenum3 != -1) {
					nodetable.item[nodenum3].num = nodenum2;
					DAG_graph.item[nodenum2].name[DAG_graph.item[nodenum2].name_len] = nodenum3;
					DAG_graph.item[nodenum2].name_len++;
					CpyString(DAG_graph.item[nodenum2].exp_name, operator1, MAX_OP_LEN);
				}
				else {
					nodenum3 = create_node_table(operator1, nodenum2);
				}
				get_opt_quad_noinit(notype);
			}
			else if (CmpString(operation, op[LODARR], MAX_OP_LEN)) {
				get_opt_quad_noinit(normal);
				nodenum1 = find_node_in_table(operator1);
				if (nodenum1 == -1) {//not find
					nodenum1 = create_node_DAG(operator1);
				}
				else {
					nodenum1 = nodetable.item[nodenum1].num;
				}
				//find op2 in table
				nodenum2 = find_node_in_table(operator2);
				if (nodenum2 == -1) {//not find
					nodenum2 = create_node_DAG(operator2);
				}
				else {
					nodenum2 = nodetable.item[nodenum2].num;
				}
				//find op in DAG
				nodenum3 = find_op_in_DAG(operation, nodenum1, nodenum2);
				if (nodenum3 == -1) {
					nodenum3 = create_op_DAG(operation, nodenum1, nodenum2);
				}//not found
				// find result in table
				nodenum4 = find_node_in_table(operator3);
				if (nodenum4 != -1) {
					nodetable.item[nodenum4].num = nodenum3;
					DAG_graph.item[nodenum3].name[DAG_graph.item[nodenum3].name_len] = nodenum4;
					DAG_graph.item[nodenum3].name_len++;
					CpyString(DAG_graph.item[nodenum3].exp_name, operator3, MAX_OP_LEN);
				}
				else {
					nodenum4 = create_node_table(operator3, nodenum3);
				}
				get_opt_quad_noinit(normal);
			}
			else if (CmpString(operation, op[STOARR], MAX_OP_LEN)) {
				/*
				get_opt_quad_noinit(normal);
				nodenum1 = find_node_in_table(operator1);
				if (nodenum1 == -1) {//not find
					nodenum1 = create_node_DAG(operator1);
				}
				else {
					nodenum1 = nodetable.item[nodenum1].num;
				}
				//find op2 in table
				nodenum2 = find_node_in_table(operator2);
				if (nodenum2 == -1) {//not find
					nodenum2 = create_node_DAG(operator2);
				}
				else {
					nodenum2 = nodetable.item[nodenum2].num;
				}
				//find op in DAG
				t = find_node_in_table(operator3);

				nodenum3 = find_op_in_DAG(operation, nodenum1, nodenum2);
				if (nodenum3 == -1) {
					nodenum3 = create_op_DAG(operation, nodenum1, nodenum2);
				}//not found
				// find result in table
				nodenum4 = find_node_in_table(operator3);
				if (nodenum4 != -1) {
					DAG_graph.item[nodenum3].name[DAG_graph.item[nodenum3].name_len] = nodenum4;
					DAG_graph.item[nodenum3].name_len++;
					CpyString(DAG_graph.item[nodenum3].exp_name, DAG_graph.item[nodetable.item[nodenum4].num].exp_name, MAX_OP_LEN);
					nodetable.item[nodenum4].num = nodenum3;
				}
				else {
					nodenum4 = create_node_table(operator3, nodenum3);
				}
				get_opt_quad_noinit(normal);*/
				s = 1;
				end_point = end;
				stoarr();
				break;
			}
			else {
				//get out of the code
				if (CmpString(operation,op[LAB], MAX_OP_LEN)) {

					for (count = 0; count < DAG_graph.top; count++) {
						if (CmpString(DAG_graph.item[count].range, operator1, MAX_OP_LEN)) {
							CpyString(DAG_graph.item[count].range, normal, MAX_OP_LEN);
						}
					}
				}
				get_opt_quad_noinit(normal);
				not_value_type(operation, operator1, operator2, operator3);
			
			}
		}
		if (s == 0) {
			get_opt_quad();
		}
	}
}