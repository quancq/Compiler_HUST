#include <stdio.h>
#include "code_generator.h"

extern Instruction code[MAX_CODE_LEN];
extern int codeLen;

void genCode(OpCode op, int p, int q){
	if(codeLen < MAX_CODE_LEN){
		code[codeLen].op = op;
		code[codeLen].p = p;
		code[codeLen].q = q;
		++codeLen;
	}
	else{
		printf("\nCode length is more than MAX_CODE_LEN\n");
	}
}

void printCode(){
	int i;
	printf("\n------- Code -------");
	for(i = 0; i < codeLen; ++i){
		int op = code[i].op;
		int p = code[i].p;
		int q = code[i].q;
		switch(op){
			case OP_LA:
				printf("\n%4d. LA %d, %d", i, p, q);
				break;
			case OP_LV:
				printf("\n%4d. LV %d, %d", i, p, q);
				break;
			case OP_LC:
				printf("\n%4d. LC %d", i, q);
				break;
			case OP_LI:
				printf("\n%4d. LI", i);
				break;
			case OP_INT:
				printf("\n%4d. INT %d", i, q);
				break;
			case OP_DCT:
				printf("\n%4d. DCT %d", i, q);
				break;
			case OP_J:
				printf("\n%4d. J %d", i, q);
				break;
			case OP_FJ:
				printf("\n%4d. FJ %d", i, q);
				break;
			case OP_HLT:
				printf("\n%4d. HLT", i);
				break;
			case OP_ST:
				printf("\n%4d. ST", i);
				break;
			case OP_CALL:
				printf("\n%4d. CALL %d, %d", i, p, q);
				break;
			case OP_EP:
				printf("\n%4d. EP", i);
				break;
			case OP_EF:
				printf("\n%4d. EF", i);
				break;
			case OP_RC:
				printf("\n%4d. RC", i);
				break;
			case OP_RI:
				printf("\n%4d. RI", i);
				break;
			case OP_WRC:
				printf("\n%4d. WRC", i);
				break;
			case OP_WRI:
				printf("\n%4d. WRI", i);
				break;
			case OP_WLN:
				printf("\n%4d. WLN", i);
				break;
			case OP_ADD:
				printf("\n%4d. ADD", i);
				break;
			case OP_SUB:
				printf("\n%4d. SUB", i);
				break;
			case OP_MUL:
				printf("\n%4d. MUL", i);
				break;
			case OP_DIV:
				printf("\n%4d. DIV", i);
				break;
			case OP_NEG:
				printf("\n%4d. NEG", i);
				break;
			case OP_CV:
				printf("\n%4d. CV", i);
				break;
			case OP_EQ:
				printf("\n%4d. EQ", i);
				break;
			case OP_NE:
				printf("\n%4d. NE", i);
				break;
			case OP_GT:
				printf("\n%4d. GT", i);
				break;
			case OP_LT:
				printf("\n%4d. LT", i);
				break;
			case OP_GE:
				printf("\n%4d. GE", i);
				break;
			case OP_LE:
				printf("\n%4d. LE", i);
				break;
		}
	}
	printf("\n");
}