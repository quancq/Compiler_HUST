#include <stdlib.h>
#include "scanner.c"
#include "semantic.c"

void error(int errorCode);	
void factor(symTab* table);			
void term(symTab* table);			
void expression(symTab* table);		
void condition(symTab* table);		
void statement(symTab* table);		
void block(symTab* table);
void program();			

FILE *f;
TokenType Token;
symStack stack;

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
			printf("\nVế trái phép gán phải là Variable hoặc là Array. Ident %s không thỏa mãn\n", Id);
			exit(1);
		case 29:
			printf("\n\n");
			exit(1);
		case 30:
			printf("\n\n");
			exit(1);
		case 31:
			printf("\n\n");
			exit(1);
	}
}

// Phân tích số hạng
void term(symTab* table){
	factor(table);
	while(Token == TIMES || Token == SLASH || Token == PERCENT){
		Token = getToken();
		factor(table);
	}
}

// Phân tích biểu thức
void expression(symTab* table){
	if(Token == PLUS || Token == MINUS){
		Token = getToken();
	}
	term(table);
	while(Token == PLUS || Token == MINUS){
		Token = getToken();
		term(table);
	}
}

// Phân tích điều kiện
void condition(symTab* table){
	if(Token == ODD){
		Token = getToken();
		expression(table);
	}
	else{
		expression(table);
		if (Token == EQU || Token == NEQ || Token == LSS ||
			Token == LEQ || Token == GTR || Token == GEQ){
			Token = getToken();
			expression(table);
		}
		else{
			error(1);
		}
	}
}

// Phân tích nhân tử
void factor(symTab* table){
	if(Token == IDENT){
		symNode* node = getNodeAllScope(table, Id);
		if(node == NULL){
			error(25);
		}

		Token = getToken();
		if(Token == LBRACK){
			Token = getToken();
			expression(table);
			if(Token == RBRACK){
				Token = getToken();
			}
			else{
				error(2);
			}
		}
	}
	else if(Token == NUMBER){
		Token = getToken();
	}
	else if(Token == LPARENT){
		Token = getToken();
		expression(table);
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
		if(Token == LBRACK){
			if(node->type != ARRAY){
				strcpy(Id, copyId);
				error(27);
			}
			Token = getToken();
			expression(table);
			if(Token == RBRACK){
				Token = getToken();
			}
			else{
				error(2);
			}
		}
		
		if(Token == ASSIGN){
			if(node->type == VARIABLE || node->type == REFERENCE || node->type == ARRAY){

			}
			else{
				strcpy(Id, copyId);
				error(28);
			}

			Token = getToken();
			expression(table);
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
			if(node->type != SUBPROC){
				error(26);
			}

			Token = getToken();
			if(Token == LPARENT){
				Token = getToken();
				expression(table);
				while(Token == COMMA){
					Token = getToken();
					expression(table);
				}
				if(Token == RPARENT){
					Token = getToken();
				}
				else{
					error(3);
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
				Token = getToken();
				expression(table);
				if(Token == TO){
					Token = getToken();
					expression(table);
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
					pushNode(table, Id, CONSTANT);
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
					pushNode(table, Id, VARIABLE);
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
						}
						else{
							error(2);
						}
					}
					else{
						error(15);
					}
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
			symNode* node = getNodeCurrScope(table, Id);
			if(node == NULL){
				pushNode(table, Id, SUBPROC);
			}
			else{
				error(24);
			}

			// Tạo bảng kí hiệu tương ứng phạm vi mới
			symTab* newTab = createTab();
			pushTab(&stack, newTab);

			Token = getToken();
			if(Token == LPARENT){
				Token = getToken();
				while(1){
					VarType type = REFERENCE;
					if(Token == VAR){
						Token = getToken();
						type = VARIABLE;
					}

					if(Token == IDENT){
						// Nếu Ident là tham chiếu thì tìm xem Ident đã khai báo chưa
						if(type == REFERENCE){
							symNode* node = getNodeAllScope(table, Id);
							if(node == NULL){
								error(25);
							}
							pushNode(newTab, Id, REFERENCE);
						}
						else {
						// Nếu Ident là tham trị thì thêm Ident vào bảng kí hiệu mới		
							pushNode(newTab, Id, VARIABLE);
						}

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
			pushNode(firstTab, Id, PROG);

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