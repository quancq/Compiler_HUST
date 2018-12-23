#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "scanner.h"
#include "parser.h"

FILE *f;
TokenType Token;
symStack stack;

VarType declareType, passType;
int declareNumArgs, passNumArgs;

char ch = ' ';
int	      Num;		//Từ vựng khi Token là NUMBER
char  Id[MAX_IDENT_LEN + 1]; //Từ vựng khi Token là IDENT

char Token_Tab[][15] = {
	"NONE", "IDENT", "NUMBER",
	"BEGIN", "CALL", "CONST", "DO",  "ELSE", "END", "FOR", "IF", "ODD",
	"PROCEDURE", "PROGRAM", "THEN", "TO", "VAR", "WHILE",
	"PLUS", "MINUS", "TIMES", "SLASH", "EQU", "NEQ", "LSS", "LEQ", "GTR", "GEQ", "LPARENT", 
	"RPARENT", "LBRACK", "RBRACK", "PERIOD", "COMMA", "SEMICOLON",  "ASSIGN", "PERCENT"
};

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