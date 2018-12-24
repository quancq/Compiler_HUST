#define MAX_IDENT_LEN 10

typedef enum {//Các loại Token được sử dụng trong PL/0
	NONE=0, IDENT, NUMBER,
	BEGIN, CALL, CONST, DO,  ELSE, END, FOR, IF, ODD,
	PROCEDURE, PROGRAM, THEN, TO, VAR, WHILE,
	PLUS, MINUS, TIMES, SLASH, EQU, NEQ, LSS, LEQ, GRT, GEQ, LPARENT, RPARENT, LBRACK, 
	RBRACK, PERIOD, COMMA, SEMICOLON, ASSIGN, PERCENT
} TokenType;

int checkKeyWord(char* s);
char getCH();
TokenType getToken();