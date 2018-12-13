#include<stdio.h>
#include<stdlib.h>
#include"compiler.h"
extern int EOF_FLAG;
int main() {
	setup();
	inittable();
	initop();
	initquad();
	
	//Reset input flow as a file.
	freopen("16231053_test.txt", "r", stdin);
	freopen("mips.asm", "w", stdout);
	//Get the first character.
	InSym();
	program();
	
	//getQuad();
	gen_mips();
	fclose(stdin);
	fclose(stdout);
	return 0;

}