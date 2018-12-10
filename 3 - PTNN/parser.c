#include <stdlib.h>
#include "scanner.c"
#include "semantic.c"

void error(int errorCode);	
VarType factor(symTab* table);			
VarType term(symTab* table);			
VarType expression(symTab* table);		
VarType condition(symTab* table);		
void statement(symTab* table);		
void block(symTab* table);
void program();			

FILE *f;
TokenType Token;
symStack stack;

VarType declareType, passType;
int declareNumArgs, passNumArgs;

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
	}
	else{
		type = expression(table);
		if (Token == EQU || Token == NEQ || Token == LSS ||
			Token == LEQ || Token == GTR || Token == GEQ){

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

		Token = getToken();
		if(Token == LBRACK){
			if(node->type != ARRAY){
				error(27);
			}

			Token = getToken();
			type = expression(table);
			if(type != INTEGER){
				error(32);
			}

			if(Token == RBRACK){
				Token = getToken();
				type = INTEGER;
			}
			else{
				error(2);
			}
		}
		else{
			type = node->type;

		}
		
	}
	else if(Token == NUMBER){
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
			declareNumArgs = node->num_args;
			passNumArgs = 0;
			VarType* arguments = node->arguments;

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
		if(Token == THEN){
			Token = getToken();
			statement(table);
			if(Token == ELSE){
				Token = getToken();
				statement(table);
			}
		}
		else{
			error(8);
		}
	}
	else if(Token == WHILE){
		Token = getToken();
		condition(table);
		if(Token == DO){
			Token = getToken();
			statement(table);
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

				if(Token == TO){
					Token = getToken();
					VarType type = expression(table);
					if(type != INTEGER){
						Token = TO;
						error(31);
					}
					if(Token == DO){
						Token = getToken();
						statement(table);
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
		while(1){
			if(Token == IDENT){
				// Kiểm tra khai báo Ident có hợp lệ không
				symNode* node = getNodeCurrScope(table, Id);
				if(node == NULL){
					node = pushNode(table, Id, VARIABLE, -1);
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
							node->type = ARRAY;
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
	}
	
	while(Token == PROCEDURE){
		Token = getToken();
		if(Token == IDENT){
			// Kiểm tra khai báo Ident có hợp lệ không
			symNode* nodeProc = getNodeCurrScope(table, Id);
			if(nodeProc == NULL){
				nodeProc = pushNode(table, Id, SUBPROC, -1);
			}
			else{
				error(24);
			}

			// Tạo bảng kí hiệu tương ứng phạm vi mới
			symTab* newTab = createTab();
			pushTab(&stack, newTab);

			Token = getToken();
			int num_args = 0;
			if(Token == LPARENT){
				Token = getToken();
				while(1){
					VarKind kind = REFERENCE;
					if(Token == VAR){
						Token = getToken();
						kind = VARIABLE;
					}

					if(Token == IDENT){
						// Nếu Ident là tham chiếu thì tìm xem Ident đã khai báo chưa
						if(kind == REFERENCE){
							symNode* node = getNodeAllScope(table, Id);
							if(node == NULL){
								error(25);
							}
							symNode* newNode =  pushNode(newTab, Id, REFERENCE, node->size);
							newNode->type = node->type;
							nodeProc->arguments[num_args] = node->type;
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
			// Tạo bảng kí hiệu mới
			symTab* firstTab = createTab();
			pushTab(&stack, firstTab);

			// Thêm ident vào bảng
			pushNode(firstTab, Id, PROG, -1);

			Token = getToken();
			if(Token == SEMICOLON){
				Token = getToken();
				block(firstTab);

				if(Token == PERIOD){
					printf("\n\n Thành công !!! \n\n");
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

int main(int argc, char *argv[]){
	char* file_path = "./test.pl0";
	if(argc == 2){
		file_path = argv[1];
	}
	f = fopen(file_path, "rt");
    
    Token = getToken(f);
    program(f);
    freeStack(&stack);
    
    fclose(f);
    return 0;
}