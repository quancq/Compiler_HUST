#include <stdlib.h>
#include "scanner.c"

void error(int errorCode);	
void factor();			
void term();			
void expression();		
void condition();		
void statement();		
void block();
void program();			

FILE *f;
TokenType Token;

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
	}
}

// Phân tích số hạng
void term(){
	factor();
	while(Token == TIMES || Token == SLASH || Token == PERCENT){
		Token = getToken();
		factor();
	}
}

// Phân tích biểu thức
void expression(){
	if(Token == PLUS || Token == MINUS){
		Token = getToken();
	}
	term();
	while(Token == PLUS || Token == MINUS){
		Token = getToken();
		term();
	}
}

// Phân tích điều kiện
void condition(){
	if(Token == ODD){
		Token = getToken();
		expression();
	}
	else{
		expression();
		if (Token == EQU || Token == NEQ || Token == LSS ||
			Token == LEQ || Token == GTR || Token == GEQ){
			Token = getToken();
			expression();
		}
		else{
			error(1);
		}
	}
}

// Phân tích nhân tử
void factor(){
	if(Token == IDENT){
		Token = getToken();
		if(Token == LBRACK){
			Token = getToken();
			expression();
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
		expression();
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
void statement(){
	if(Token == IDENT){
		Token = getToken();
		if(Token == LBRACK){
			Token = getToken();
			expression();
			if(Token == RBRACK){
				Token = getToken();
			}
			else{
				error(2);
			}
		}
		
		if(Token == ASSIGN){
			Token = getToken();
			expression();
		}else{
			error(5);
		}
	}
	else if(Token == CALL){
		Token = getToken();
		if(Token == IDENT){
			Token = getToken();
			if(Token == LPARENT){
				Token = getToken();
				expression();
				while(Token == COMMA){
					Token = getToken();
					expression();
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
		statement();
		while(Token == SEMICOLON){
			Token = getToken();
			statement();
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
		condition();
		if(Token == THEN){
			Token = getToken();
			statement();
			if(Token == ELSE){
				Token = getToken();
				statement();
			}
		}
		else{
			error(8);
		}
	}
	else if(Token == WHILE){
		Token = getToken();
		condition();
		if(Token == DO){
			Token = getToken();
			statement();
		}
		else{
			error(9);
		}
	}
	else if(Token == FOR){
		Token = getToken();
		if(Token == IDENT){
			Token = getToken();
			if(Token == ASSIGN){
				Token = getToken();
				expression();
				if(Token == TO){
					Token = getToken();
					expression();
					if(Token == DO){
						Token = getToken();
						statement();
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
void block(){
	if(Token == CONST){
		Token = getToken();
		while(1){
			if(Token == IDENT){
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
			Token = getToken();
			if(Token == LPARENT){
				Token = getToken();
				while(1){
					if(Token == VAR){
						Token = getToken();
					}
					if(Token == IDENT){
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
				block();
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
	
	if(Token ==  BEGIN){
		Token = getToken();
		statement();

		while(Token == SEMICOLON){
			Token = getToken();
			statement();
		}

		if(Token == END){
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
			Token = getToken();
			if(Token == SEMICOLON){
				Token = getToken();
				block();

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
    
    fclose(f);
    return 0;
}