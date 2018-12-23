#define MAX_IDENT_LEN 10
#define INT_SIZE 2
#define MAX_ARGUMENTS 10

typedef enum {
	CONSTANT = 60, VARIABLE, REFERENCE, SUBPROC, PROG
} VarKind;

typedef enum {
	INTEGER = 80, ARRAY
} VarType;

typedef struct node symNode;
typedef struct tab symTab;

typedef struct node {
	VarKind kind;
	VarType type;
	char name[MAX_IDENT_LEN + 1];
	int offset;
	int size;
	int num_args;
	VarType arguments[MAX_ARGUMENTS];
	symTab* subProc;

	struct node* next;
} symNode;

typedef struct tab {
	char name[MAX_IDENT_LEN + 1];
	symNode* top;
	int size;
	struct tab* parentTab;
} symTab;

typedef struct stack {
	symTab* top;
} symStack;

void printTab(symTab* table);
symNode* createNode();
symTab* createTab(char* name);
void freeTab(symTab* table);
void freeStack(symStack* stack);
symNode* pushNode(symTab* table, char* name, VarKind kind, int size);
void pushTab(symStack* stack, symTab* table);
symTab* popTab(symStack* stack);
symNode* getNodeCurrScope(symTab* table, char* name);
symNode* getNodeAllScope(symTab* table, char* name);