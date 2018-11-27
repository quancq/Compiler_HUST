#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "scanner.h"


FILE *f;
char ch = ' ';
TokenType Token;	//Token nhận dạng được
int	      Num;		//Từ vựng khi Token là NUMBER
char  Id[MAX_IDENT_LEN + 1]; //Từ vựng khi Token là IDENT

int checkKeyWord(char* s){
	if(strcmp(s, "BEGIN") == 0){
		return BEGIN;
	}else if(strcmp(s, "CALL") == 0){
		return CALL;
	}else if(strcmp(s, "CONST") == 0){
		return CONST;
	}else if(strcmp(s, "DO") == 0){
		return DO;
	}else if(strcmp(s, "ELSE") == 0){
		return ELSE;
	}else if(strcmp(s, "END") == 0){
		return END;
	}else if(strcmp(s, "FOR") == 0){
		return FOR;
	}else if(strcmp(s, "IF") == 0){
		return IF;
	}else if(strcmp(s, "ODD") == 0){
		return ODD;
	}else if(strcmp(s, "PROCEDURE") == 0){
		return PROCEDURE;
	}else if(strcmp(s, "PROGRAM") == 0){
		return PROGRAM;
	}else if(strcmp(s, "THEN") == 0){
		return THEN;
	}else if(strcmp(s, "TO") == 0){
		return TO;
	}else if(strcmp(s, "VAR") == 0){
		return VAR;
	}else if(strcmp(s, "WHILE") == 0){
		return WHILE;
	}
	return IDENT;
}

char getCH(){
	char c = fgetc(f);
	// printf("Read %c\n", c);
	return c;
}

TokenType getToken(){
	// Loại bỏ kí hiệu vô ích
	while(ch == ' ' || ch == '\t' || ch == '\n'){
		ch = getCH();
	}
	if(isdigit(ch)){
		Num = 0;
		do{
			Num = Num * 10 + ch - 48;
			ch = getCH();
		}while(isdigit(ch));
		return NUMBER;
	}else if(isalpha(ch)){
		int i = 0;
		do{
			if(i < MAX_IDENT_LEN){
				Id[i] = toupper(ch);
				++i;
			}
			ch = getCH();
		}while(isalnum(ch));
		Id[i] = '\0';
		return checkKeyWord(Id);
	}else if(ch == '+'){
		ch = getCH();
		return PLUS;
	}else if(ch == '-'){
		ch = getCH();
		return MINUS;
	}else if(ch == '*'){
		ch = getCH();
		return TIMES;
	}else if(ch == '/'){
		ch = getCH();
		return SLASH;
	}else if(ch == '%'){
		ch = getCH();
		return PERCENT;
	}else if(ch == '='){
		ch = getCH();
		return EQU;
	}else if(ch == '<'){
		ch = getCH();
		if(ch == '=') {
			ch = getCH();
			return LEQ;
		}
		else if(ch == '>'){
			ch = getCH();
			return NEQ;
		}
		return LSS;
	}else if(ch == '>'){
		ch = getCH();
		if(ch == '=') {
			ch = getCH();
			return GEQ;
		}
		return GTR;
	}else if(ch == '('){
		ch = getCH();
		return LPARENT;
	}else if(ch == ')'){
		ch = getCH();
		return RPARENT;
	}else if(ch == '.'){
		ch = getCH();
		return PERIOD;
	}else if(ch == ','){
		ch = getCH();
		return COMMA;
	}else if(ch == ';'){
		ch = getCH();
		return SEMICOLON;
	}else if(ch == '['){
		ch = getCH();
		return LBRACK;
	}else if(ch == ']'){
		ch = getCH();
		return RBRACK;
	}else if(ch == ':'){
		ch = getCH();
		if(ch == '='){
			ch = getCH();
			return ASSIGN;
		}
		return NONE;
	}
	return NONE;
}


int main(int argc, char *argv[]){
	char* file_path = "./test.pl0";
	if(argc == 2){
		file_path = argv[1];
	}
	f = fopen(file_path, "rt");

	while((Token = getToken()) != NONE){
		if(Token == NUMBER){
			printf("NUMBER %d\n", Num);
		}else if(Token == IDENT){
			printf("IDENT %s\n", Id);
		}else{
			printf("%s\n", Token_Tab[Token]);
		}
	}
	fclose(f);
	return 0;
}
