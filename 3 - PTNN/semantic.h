typedef enum {
	CONSTANT = 60, VARIABLE, REFERENCE, ARRAY, SUBPROC, PROG
} VarType;

typedef struct node {
	VarType type;
	char name[11];

	struct node* next;
} symNode;

typedef struct tab {
	symNode* next;
	struct tab* parentTab;
} symTab;

typedef struct stack {
	symTab* parentTab;
} symStack;