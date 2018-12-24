#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "scanner.h"
#include "parser.h"
#include "code_generator.h"

extern symStack stack;
extern TokenType Token;
extern VarType declareType, passType;
extern int declareNumArgs, passNumArgs;
extern int Num;
extern char Id[11];
extern char Token_Tab[][15];
extern Instruction code[MAX_CODE_LEN];
extern int codeLen;

void copyArray(VarType* src, VarType* dst, int numElmss){
	int i;
	for(i = 0; i < numElmss; ++i){
		dst[i] = src[i];
	}
}

char* getTypeStr(int type){
	if(type == INTEGER){
		return "Integer";
	}
	if(type == ARRAY){
		return "Array";
	}
}

// Báo lỗi
void error(int errorCode){
	switch(errorCode){
		case 1:
			printf("\nThiếu toán tử quan hệ\n");
			exit(1);
		case 2:
			printf("\nThiếu dấu đóng ngoặc ]\n");
			exit(1);
		case 3:
			printf("\nThiếu dấu đóng ngoặc )\n");
			exit(1);
		case 4:
			printf("\nVị trí này phải là Ident hoặc Number hoặc (\n");
			exit(1);
		case 5:
			printf("\nThiếu toán tử gán\n");
			exit(1);
		case 6:
			printf("\nThiếu Ident sau CALL\n");
			exit(1);
		case 7:
			printf("\nThiếu từ khóa END\n");
			exit(1);
		case 8:
			printf("\nThiếu từ khóa THEN\n");
			exit(1);									
		case 9:
			printf("\nThiếu từ khóa DO\n");
			exit(1);
		case 10:
			printf("\nThiếu Ident sau FOR\n");
			exit(1);
		case 11:
			printf("\nThiếu từ khóa TO\n");
			exit(1);
		case 12:
			printf("\nThiếu Number sau toán tử gán\n");
			exit(1);									
		case 13:
			printf("\nVị trí này phải là , hoặc ;\n");
			exit(1);
		case 14:
			printf("\nThiếu Ident\n");
			exit(1);
		case 15:
			printf("\nThiếu Number\n");
			exit(1);
		case 16:
			printf("\nThiếu Ident sau PROCEDURE\n");
			exit(1);									
		case 17:
			printf("\nThiếu dấu ;\n");
			exit(1);
		case 18:
			printf("\nVị trí này phải là ) hoặc ;\n");
			exit(1);
		case 19:
			printf("\nThiếu từ khóa BEGIN\n");
			exit(1);
		case 20:
			printf("\nThiếu từ khóa PROGRAM\n");
			exit(1);
		case 21:
			printf("\nThiếu Ident sau PROGRAM\n");
			exit(1);
		case 22:
			printf("\nThiếu dấu .\n");
			exit(1);
		case 23:
			printf("\nThiếu toán tử =\n");
			exit(1);
		case 24:
			printf("\nIdent %s đã tồn tại\n", Id);
			exit(1);
		case 25:
			printf("\nIdent %s chưa được khai báo\n", Id);
			exit(1);
		case 26:
			printf("\nIdent %s không phải là Procedure\n", Id);
			exit(1);
		case 27:
			printf("\nIdent %s không phải là Array\n", Id);
			exit(1);
		case 28:
			printf("\nVế trái phép gán phải là Variable. Ident %s không thỏa mãn\n", Id);
			exit(1);
		case 29:
			printf("\nVế trái phép gán không thể có kiểu mảng. Ident %s không thỏa mãn (cần có kiểu Integer)\n", Id);
			exit(1);
		case 30:
			printf("\nVế trái của %s phải có kiểu Integer\n", Token_Tab[Token]);
			exit(1);
		case 31:
			printf("\nVế phải của %s phải có kiểu Integer\n", Token_Tab[Token]);
			exit(1);
		case 32:
			printf("\n Bên trong [...] phải có kiểu Integer (vì đây là chỉ số của mảng) \n");
			exit(1);
		case 33:
			printf("\nIdent %s phải có kiểu Integer\n", Id);
			exit(1);
		case 34:
			printf("\nSố lượng tham số truyền vào thủ tục (%d tham số) không giống với khai báo (%d tham số)\n", passNumArgs, declareNumArgs);
			exit(1);
		case 35:
			printf("\nSai kiểu của tham số thứ %d. Kiểu của tham số truyền vào thủ tục (kiểu %s) không giống kiểu trong khai báo (Kiểu %s)\n", passNumArgs+1, getTypeStr(passType), getTypeStr(declareType));
			exit(1);
		case 36:
			printf("\n\n");
			exit(1);
	}
}

// Phân tích số hạng
VarType term(symTab* table){
	VarType type = factor(table);
	if(Token == TIMES || Token == SLASH || Token == PERCENT){
		if(type != INTEGER){
			error(30);
		}
	}
	while(Token == TIMES || Token == SLASH || Token == PERCENT){
		TokenType archiveToken = Token;
		Token = getToken();
		VarType type = factor(table);
		if(type != INTEGER){
			Token = archiveToken;
			error(31);
		}
		if(archiveToken == TIMES){
			genCode(OP_MUL, 0, 0);
		}
		else if(archiveToken == SLASH){
			genCode(OP_DIV, 0, 0);
		}
	}

	return type;
}

// Phân tích biểu thức
VarType expression(symTab* table){
	TokenType archiveToken;
	if(Token == PLUS || Token == MINUS){
		archiveToken = Token;
		Token = getToken();
	}
	VarType type = term(table);
	if(archiveToken == PLUS || archiveToken == MINUS){
		if(type != INTEGER){
			Token = archiveToken;
			error(31);
		}
		if(archiveToken == MINUS){
			genCode(OP_NEG, 0, 0);
		}
	}

	if(Token == PLUS || Token == MINUS){
		if(type != INTEGER){
			error(30);
		}
	}

	while(Token == PLUS || Token == MINUS){
		archiveToken = Token;
		Token = getToken();
		type = term(table);
		if(type != INTEGER){
			Token = archiveToken;
			error(31);
		}
		if(archiveToken == PLUS){
			genCode(OP_ADD, 0, 0);
		}
		else{
			genCode(OP_SUB, 0, 0);
		}
		
	}

	return type;
}

// Phân tích điều kiện
VarType condition(symTab* table){
	VarType type;
	TokenType archiveToken;
	if(Token == ODD){
		Token = getToken();
		type = expression(table);
		if(type != INTEGER){
			Token = ODD;
			error(31);
		}

		// Kiểm tra gía trị trên đỉnh stack là lẻ hay chẵn
		// Nếu lẻ thì ghi gía trị 1 vào đỉnh stack
		genCode(OP_CV, 0, 0);
		genCode(OP_LC, 0, 2);
		genCode(OP_DIV, 0, 0);
		genCode(OP_LC, 0, 2);
		genCode(OP_MUL, 0, 0);
		genCode(OP_NE, 0, 0);
	}
	else{
		type = expression(table);
		if (Token == EQU || Token == NEQ || Token == LSS ||
			Token == LEQ || Token == GRT || Token == GEQ){

			if(type != INTEGER){
				error(30);
			}
			
			archiveToken = Token;
			Token = getToken();
			type = expression(table);

			if(type != INTEGER){
				Token = archiveToken;
				error(31);
			}

			switch(archiveToken){
	            case EQU:
	                genCode(OP_EQ, 0, 0);
	                break;
	            case NEQ:
	                genCode(OP_NE, 0, 0);
	                break;
	            case LSS:
	                genCode(OP_LT, 0, 0);
	                break;
	            case LEQ:
	                genCode(OP_LE, 0, 0);
	                break;
	            case GRT:
	                genCode(OP_GT, 0, 0);
	                break;
	            case GEQ:
	                genCode(OP_GE, 0, 0);
	                break;
	        }

		}
		else{
			error(1);
		}
	}

	return type;
}

// Phân tích nhân tử
VarType factor(symTab* table){
	VarType type;
	if(Token == IDENT){
		symNode* node = getNodeAllScope(table, Id);
		if(node == NULL){
			error(25);
		}

		char* copyId = (char*)malloc(MAX_IDENT_LEN);
		strcpy(copyId, Id);
		int p = getScopeDepth(table, Id);
		Token = getToken();
		
		if(Token == LBRACK){
			if(node->type != ARRAY){
				error(27);
			}
			genCode(OP_LA, p, node->offset);

			Token = getToken();
			type = expression(table);
			if(type != INTEGER){
				error(32);
			}

			if(Token == RBRACK){
				Token = getToken();
				type = INTEGER;

				genCode(OP_ADD, 0, 0);
				genCode(OP_LI, 0, 0);
			}
			else{
				error(2);
			}
		}
		else{
			type = node->type;
			
			if(node->kind == VARIABLE){
				genCode(OP_LV, p, node->offset);
			}
			else if(node->kind == REFERENCE){
				genCode(OP_LA, p, node->offset);
			}
		}
		
	}
	else if(Token == NUMBER){
		genCode(OP_LC, 0, Num);
		Token = getToken();
		type = INTEGER;
	}
	else if(Token == LPARENT){
		Token = getToken();
		type = expression(table);
		if(Token == RPARENT){
			Token = getToken();
		}
		else{
			error(3);
		}
	}
	else{
		error(4);
	}

	return type;
}

// Phân tích câu lệnh
void statement(symTab* table){
	if(Token == IDENT){
		symNode* node = getNodeAllScope(table, Id);
		if(node == NULL){
			error(25);
		}
		char* copyId = (char*)malloc(MAX_IDENT_LEN);
		strcpy(copyId, Id);

		// Load address of ident
		int p = getScopeDepth(table, Id);
		genCode(OP_LA, p, node->offset);

		Token = getToken();
		int has_brack = 0;
		if(Token == LBRACK){
			if(node->type != ARRAY){
				strcpy(Id, copyId);
				error(27);
			}
			Token = getToken();
			VarType type = expression(table);
			if(type != INTEGER){
				error(32);
			}

			if(Token == RBRACK){
				has_brack = 1;
				Token = getToken();
			}
			else{
				error(2);
			}

			// Add index of array to pointer (arr[i] -> arr + i)
			genCode(OP_ADD, 0, 0);
		}
		
		if(Token == ASSIGN){
			if(has_brack == 0 && node->type == ARRAY){
				strcpy(Id, copyId);
				error(29);
			}

			if(node->kind == VARIABLE || node->kind == REFERENCE){

			}
			else{
				strcpy(Id, copyId);
				error(28);
			}

			Token = getToken();
			VarType type = expression(table);
			if(type != INTEGER){
				Token = ASSIGN;
				error(31);
			}
			genCode(OP_ST, 0, 0);

		}else{
			error(5);
		}
		free(copyId);
	}
	else if(Token == CALL){
		Token = getToken();
		if(Token == IDENT){
			symNode* node = getNodeAllScope(table, Id);
			if(node == NULL){
				error(25);
			}
			if(node->kind != SUBPROC){
				error(26);
			}
			int p = getScopeDepth(table, Id);
			declareNumArgs = node->num_args;
			passNumArgs = 0;
			VarType* arguments = node->arguments;

			genCode(OP_INT, 0, 4);

			Token = getToken();
			if(Token == LPARENT){
				// if(declareNumArgs == 0){
				// 	error(34);
				// }

				Token = getToken();
				VarType type = expression(table);
				if(type != arguments[passNumArgs]){
					passType = type;
					declareType = arguments[passNumArgs];
					error(35);
				}
				++passNumArgs;
				if(passNumArgs > declareNumArgs){
					error(34);
				}

				while(Token == COMMA){

					Token = getToken();
					VarType type = expression(table);
					if(type != arguments[passNumArgs]){
						passType = type;
						declareType = arguments[passNumArgs];
						error(35);
					}
					++passNumArgs;
					if(passNumArgs > declareNumArgs){
						error(34);
					}
					
				}
				if(Token == RPARENT){
					if(passNumArgs < declareNumArgs){
						error(34);
					}
					Token = getToken();
				}
				else{
					error(3);
				}
			}
			else{
				if(declareNumArgs > 0){
					error(34);
				}
			}
			genCode(OP_DCT, 0, 4 + INT_SIZE * (passNumArgs));
			genCode(OP_CALL, p, node->offset);
		}
		else{
			error(6);
		}
	}
	else if(Token == BEGIN){
		Token = getToken();
		statement(table);
		while(Token == SEMICOLON){
			Token = getToken();
			statement(table);
		}
		if(Token == END){
			Token = getToken();
		}
		else{
			error(7);
		}
	}
	else if(Token == IF){
		Token = getToken();
		condition(table);

		int l1 = codeLen;
		genCode(OP_FJ, 0, 0);

		if(Token == THEN){
			Token = getToken();
			statement(table);

			code[l1].q = codeLen;

			if(Token == ELSE){
				code[l1].q = code[l1].q + 1;
				int l2 = codeLen;
				genCode(OP_J, 0, 0);

				Token = getToken();
				statement(table);

				code[l2].q = codeLen;
			}
		}
		else{
			error(8);
		}
	}
	else if(Token == WHILE){
		Token = getToken();
		int l1 = codeLen;
		condition(table);
		if(Token == DO){
			int l2 = codeLen;
			genCode(OP_FJ, 0, 0);
			Token = getToken();
			statement(table);
			genCode(OP_J, 0, l1);
			code[l2].q = codeLen;
		}
		else{
			error(9);
		}
	}
	else if(Token == FOR){
		Token = getToken();
		if(Token == IDENT){
			symNode* node = getNodeAllScope(table, Id);
			if(node == NULL){
				error(25);
			}
			int p = getScopeDepth(table, Id);
			genCode(OP_LA, p, node->offset);
			genCode(OP_CV, 0, 0);

			Token = getToken();
			if(Token == ASSIGN){
				if(node->type != INTEGER){
					error(33);
				}

				Token = getToken();
				VarType type = expression(table);
				if(type != INTEGER){
					Token = ASSIGN;
					error(31);
				}
				genCode(OP_ST, 0, 0);

				if(Token == TO){
					int l1 = codeLen;
					genCode(OP_CV, 0, 0);
					genCode(OP_LI, 0, 0);

					Token = getToken();
					VarType type = expression(table);
					if(type != INTEGER){
						Token = TO;
						error(31);
					}
					genCode(OP_LE, 0, 0);
					int l2 = codeLen;
					genCode(OP_FJ, 0, 0);

					if(Token == DO){
						Token = getToken();
						statement(table);

						genCode(OP_CV, 0, 0);
						genCode(OP_CV, 0, 0);
						genCode(OP_LI, 0, 0);
						genCode(OP_LC, 0, 1);
						genCode(OP_ADD, 0, 0);
						genCode(OP_ST, 0, 0);
						genCode(OP_J, 0, l1);
						code[l2].q = codeLen;
						genCode(OP_DCT, 0, 1);
					}
					else{
						error(9);
					}
				}
				else{
					error(11);
				}
			}
			else{
				error(5);
			}
		}
		else{
			error(10);
		}
	}

}

// Phân tích các khối câu lệnh
void block(symTab* table){
	if(Token == CONST){
		Token = getToken();
		while(1){
			if(Token == IDENT){
				// Kiểm tra khai báo Ident có hợp lệ không
				symNode* node = getNodeCurrScope(table, Id);
				if(node == NULL){
					node = pushNode(table, Id, CONSTANT, INT_SIZE);
					node->type = INTEGER;
				}
				else{
					error(24);
				}

				Token = getToken();
				if(Token == EQU){
					Token = getToken();
					if(Token == NUMBER){
						genCode(OP_LC, 0, Num);
						Token = getToken();
						if(Token == COMMA){
							Token = getToken();
						}
						else if(Token == SEMICOLON){
							Token = getToken();
							break;
						}
						else{
							error(13);
						}
					}
					else{
						error(12);
					}
				}
				else{
					error(23);
				}
			}
			else{
				error(14);
			}
		}
	}
	if(Token == VAR){
		Token = getToken();
		int totalVarSize = 0;
		while(1){
			if(Token == IDENT){
				// Kiểm tra khai báo Ident có hợp lệ không
				symNode* node = getNodeCurrScope(table, Id);
				if(node == NULL){
					node = pushNode(table, Id, VARIABLE, INT_SIZE);
				}
				else{
					error(24);
				}

				Token = getToken();
				if(Token == LBRACK){
					Token = getToken();
					if(Token == NUMBER){
						Token = getToken();
						if(Token == RBRACK){
							Token = getToken();
							node->size = Num * INT_SIZE;
							node->arrLen = Num;
							node->type = ARRAY;
							// genCode(OP_INT, 0, Num*INT_SIZE);
							totalVarSize += Num * INT_SIZE;
						}
						else{
							error(2);
						}
					}
					else{
						error(15);
					}
				}
				else{
					node->size = INT_SIZE;
					node->type = INTEGER;
					// genCode(OP_INT, 0, INT_SIZE);
					totalVarSize += INT_SIZE;
				}

				if(Token == COMMA){
					Token = getToken();
				}
				else if(Token == SEMICOLON){
					Token = getToken();
					break;
				}
				else{
					error(13);
				}
			}
			else{
				error(14);
			}
		}
		if(totalVarSize > 0){
			genCode(OP_INT, 0, totalVarSize);
		}
	}
	
	while(Token == PROCEDURE){
		Token = getToken();
		if(Token == IDENT){
			// Kiểm tra khai báo Ident có hợp lệ không
			symNode* nodeProc = getNodeCurrScope(table, Id);
			if(nodeProc == NULL){
				nodeProc = pushNode(table, Id, SUBPROC, 0);				
			}
			else{
				error(24);
			}

			// Tạo bảng kí hiệu tương ứng phạm vi mới
			symTab* newTab = createTab(Id);
			pushTab(&stack, newTab);
			nodeProc->subProc = newTab;
			// printf("\nThêm subProc %s vào bảng %s \n", Id, table->name);
			// char* copyId = (char*)malloc(MAX_IDENT_LEN);
			// strcpy(copyId, Id);

			Token = getToken();
			int num_args = 0;
			VarType arguments[MAX_ARGUMENTS];
			if(Token == LPARENT){
				Token = getToken();
				while(1){
					VarKind kind = VARIABLE;
					if(Token == VAR){
						Token = getToken();
						kind = REFERENCE;
					}

					if(Token == IDENT){
						// Nếu Ident là tham chiếu thì tìm xem Ident đã khai báo chưa
						if(kind == REFERENCE){
							symNode* newNode =  pushNode(newTab, Id, REFERENCE, INT_SIZE);
							newNode->type = INTEGER;
							nodeProc->arguments[num_args] = INTEGER;
						}
						else {
						// Nếu Ident là tham trị thì thêm Ident vào bảng kí hiệu mới		
							symNode* newNode = pushNode(newTab, Id, VARIABLE, INT_SIZE);
							newNode->type = INTEGER;
							nodeProc->arguments[num_args] = INTEGER;
						}
						++num_args;

						Token = getToken();
						if(Token == RPARENT){
							Token = getToken();
							break;
						}
						else if(Token == SEMICOLON){
							Token = getToken();
							continue;
						}
						else{
							error(18);
						}
					}
					else{
						error(14);
					}
				}
			}
			nodeProc->num_args = num_args;

			if(Token == SEMICOLON){
				Token = getToken();
				block(newTab);
				genCode(OP_EP, 0, 0);
				if(Token == SEMICOLON){
					Token = getToken();
					continue;
				}
				else{
					error(17);
				}
			}
			else{
				error(17);
			}
			
		}
		else{
			error(16);
		}
	}
	
	if(Token == BEGIN){
		Token = getToken();
		statement(table);

		while(Token == SEMICOLON){
			Token = getToken();
			statement(table);
		}

		if(Token == END){
			popTab(&stack);
			Token = getToken();
		}
		else{
			error(7);
		}
	}
	else{
		error(19);
	}
}

// Phân tích chương trình
void program(){
	if(Token == PROGRAM){

		Token = getToken();
		if(Token == IDENT){
			genCode(OP_J, 0, 1);
			genCode(OP_INT, 0, 4);

			// Tạo bảng kí hiệu mới
			symTab* firstTab = createTab(Id);
			pushTab(&stack, firstTab);

			// Thêm ident vào bảng
			pushNode(firstTab, Id, PROG, 0);
			Token = getToken();
			if(Token == SEMICOLON){
				Token = getToken();
				block(firstTab);

				if(Token == PERIOD){
					genCode(OP_HLT, 0, 0);
					printf("\n\n Thành công !!! \n\n");

					printCode();
				}
				else{
					error(22);
				}
			}
			else{
				error(17);
			}
		}
		else{
			error(21);
		}
	}
	else{
		error(20);
	}
}
