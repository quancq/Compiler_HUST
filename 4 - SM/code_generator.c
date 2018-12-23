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
				printf("\nLA %d, %d", p, q);
				break;
			case OP_LV:
				printf("\nLV %d, %d", p, q);
				break;
			case OP_LC:
				printf("\nLC %d", q);
				break;
			case OP_LI:
				printf("\nLI");
				break;
			case OP_INT:
				printf("\nINT %d", q);
				break;
			case OP_DCT:
				printf("\nDCT %d", q);
				break;
			case OP_J:
				printf("\nJ %d", q);
				break;
			case OP_FJ:
				printf("\nFJ %d", q);
				break;
			case OP_HLT:
				printf("\nHLT");
				break;
			case OP_ST:
				printf("\nST");
				break;
			case OP_CALL:
				printf("\nCALL %d, %d", p, q);
				break;
			case OP_EP:
				printf("\nEP");
				break;
			case OP_EF:
				printf("\nEF");
				break;
			case OP_RC:
				printf("\nRC");
				break;
			case OP_RI:
				printf("\nRI");
				break;
			case OP_WRC:
				printf("\nWRC");
				break;
			case OP_WRI:
				printf("\nWRI");
				break;
			case OP_WLN:
				printf("\nWLN");
				break;
			case OP_ADD:
				printf("\nADD");
				break;
			case OP_SUB:
				printf("\nSUB");
				break;
			case OP_MUL:
				printf("\nMUL");
				break;
			case OP_DIV:
				printf("\nDIV");
				break;
			case OP_NEG:
				printf("\nNEG");
				break;
			case OP_CV:
				printf("\nCV");
				break;
			case OP_EQ:
				printf("\nEQ");
				break;
			case OP_NE:
				printf("\nNE");
				break;
			case OP_GT:
				printf("\nGT");
				break;
			case OP_LT:
				printf("\nLT");
				break;
			case OP_GE:
				printf("\nGE");
				break;
			case OP_LE:
				printf("\nLE");
				break;
		}
	}
}