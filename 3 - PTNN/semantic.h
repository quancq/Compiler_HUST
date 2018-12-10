typedef enum {
	CONSTANT = 60, VARIABLE, REFERENCE, SUBPROC, PROG
} VarKind;

typedef enum {
	INTEGER = 80, ARRAY
} VarType;

#define INT_SIZE 2
#define MAX_ARGUMENTS 10

typedef struct node {
	VarKind kind;
	VarType type;
	char name[MAX_IDENT_LEN+1];
	int offset;
	int size;
	int num_args;
	VarType arguments[MAX_ARGUMENTS];

	struct node* next;
} symNode;

typedef struct tab {
	symNode* top;
	int size;
	struct tab* parentTab;
} symTab;

typedef struct stack {
	symTab* top;
} symStack;