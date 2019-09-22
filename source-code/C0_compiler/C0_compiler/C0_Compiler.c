#include<stdio.h>
#include<stdlib.h>
#include"compiler.h"
extern int EOF_FLAG;
int opt;
int opt_con;
int opt_peep;
int opt_DAG;
int main() {
	opt = 1;
	opt_con = 1;
	opt_peep = 1;
	opt_DAG = 1;
	setup();
	inittable();
	initop();
	initquad();
	//Reset input flow as a file.
	freopen("testchare.txt", "r", stdin);
	freopen("mips.asm", "w", stdout);
	//freopen("quad.txt", "w", stdout);
	//Get the first character.
	InSym();
	program();
	gen_mips();
	freopen("quad.txt", "w", stdout);
	getQuad();
	//opt part
	if (opt_con) {
		const_combination();
	}
	divide_basics();
	if (opt) {
		global_reg_alloc();
	}
	//if (opt_DAG) {
		DAG();
		C0_Quad = C0_Quad_opt;
		freopen("quad_opt.txt", "w", stdout);
		getQuad();
	//}
	
	//if (opt_peep) {
		freopen("mips.asm", "r", stdin);
		freopen("mips_opt.asm", "w", stdout);
		//getQuad();
		peep_hole();
	//}
	//getQuad();
	
	fclose(stdin);
	fclose(stdout);
	return 0;

}