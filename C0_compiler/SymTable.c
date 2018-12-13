#include<stdio.h>
#include<stdlib.h>
#include"compiler.h"
extern char itemname[MAX_ID_LENGTH], itemtype[MAX_ID_LENGTH], itemkind[MAX_ID_LENGTH];
extern int itemvalue, itemnumber, funcpara, itemaddress,curlev,mips_flag;
SymTable C0_SymTable;
/*void inittable
*/
void inittable() {
	int i;
	C0_SymTable.top = 1;
	//set the base symbol
	C0_SymTable.item[0].addr = 0;
	C0_SymTable.item[0].number = 0;
	C0_SymTable.item[0].value = 0;
	C0_SymTable.item[0].lev = 0;
	CpyString(C0_SymTable.item[0].type, "VOID                     ", MAX_SYM_LENGTH);
	CpyString(C0_SymTable.item[0].kind, "NOTYPE                   ", MAX_SYM_LENGTH);
	CpyString(C0_SymTable.item[0].name, "BASE                     ", MAX_SYM_LENGTH);
	for (i = 1; i < TABLESIZE; i++) {
		C0_SymTable.item[i].addr = 0;
		C0_SymTable.item[i].number = 0;
		C0_SymTable.item[i].value = 0;
		C0_SymTable.item[i].lev = 0;
		CpyString(C0_SymTable.item[i].type, "VOID                     ", MAX_SYM_LENGTH);
		CpyString(C0_SymTable.item[i].kind, "NOTYPE                   ", MAX_SYM_LENGTH);
		CpyString(C0_SymTable.item[i].name, "                         ", MAX_SYM_LENGTH);
	}
}
/*intable:将符号写入符号表前的判断:
function cannot redefine at any time.
Variable can be defined as it is.*/
int check_redefine() {
	int  i;
	int top = C0_SymTable.top;
	if (CmpString(itemkind, "FUNCTION                 ", MAX_ID_LENGTH)) {
		curlev = 0;
	}
	for (i = curlev + 1; i < top; i++) {
		if (CmpString(C0_SymTable.item[i].name, itemname, MAX_ID_LENGTH))
			return 1;
	}
	for (i = 1; i <= curlev; i++) {
		if (CmpString(C0_SymTable.item[i].name, itemname, MAX_ID_LENGTH) && CmpString(C0_SymTable.item[i].kind, "FUNCTION                 ", MAX_ID_LENGTH))
			return 1;
	}
	return 0;
}
int find_sym(char name[]) {
	int  i;
	int top = C0_SymTable.top;
	if (mips_flag) {
		for (i = curlev + 1; i < top; i++) {
			if (CmpString(C0_SymTable.item[i].name, name, MAX_ID_LENGTH) && (C0_SymTable.item[i].lev == curlev))
				return i;
		}
		for (i = 1; i < top; i++) {
			if (CmpString(C0_SymTable.item[i].name, name, MAX_ID_LENGTH) && (C0_SymTable.item[i].lev == 0))
				return i;
		}
		return 0;
	}
	else {
		for (i = curlev + 1; i < top; i++) {
			if (CmpString(C0_SymTable.item[i].name, name, MAX_ID_LENGTH) && (C0_SymTable.item[i].lev == curlev))
				return i;
		}
		for (i = 1; i <= curlev; i++) {
			if (CmpString(C0_SymTable.item[i].name, name, MAX_ID_LENGTH) && (C0_SymTable.item[i].lev == 0))
				return i;
		}
		return 0;
	}
}
void setfunsize() {
	C0_SymTable.item[curlev].value = itemaddress - C0_SymTable.item[curlev].addr;
}
/*enter table:
将符号写入符号表
*/
int entertable() {
	int top = C0_SymTable.top;
	if (check_redefine()) {
		return 1;
	}
	if (CmpString(itemkind, "FUNCTION                 ", MAX_ID_LENGTH)) {
		itemaddress = 0;
	}
	C0_SymTable.item[top].number = itemnumber;
	C0_SymTable.item[top].value = itemvalue;
	C0_SymTable.item[top].addr = itemaddress;
	C0_SymTable.item[top].lev = curlev;
	CpyString(C0_SymTable.item[top].type, itemtype, MAX_SYM_LENGTH);
	CpyString(C0_SymTable.item[top].kind, itemkind, MAX_SYM_LENGTH);
	CpyString(C0_SymTable.item[top].name, itemname, MAX_SYM_LENGTH);
	if ((itemnumber == 0)||(CmpString(itemkind,"FUNCTION                 ",MAX_ID_LENGTH)))
		itemaddress += 4;
	else
		itemaddress = itemaddress + itemnumber * 4;
	if (CmpString(itemkind, "FUNCTION                 ", MAX_ID_LENGTH)) {
		curlev = top;
	}
	itemnumber = 0;
	itemvalue = 0;
	C0_SymTable.top++;
	return 0;
}