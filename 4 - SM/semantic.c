#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "semantic.h"

symNode* createNode(){
	symNode* node = (symNode*) malloc(sizeof(symNode));
	node->next = NULL;
	return node;
}

symTab* createTab(char* name){
	symTab* tab = (symTab*) malloc(sizeof(symTab));
	strcpy(tab->name, name);
	tab->parentTab = NULL;
	tab->top = NULL;
	tab->size = 0;
	return tab;
}

void freeTab(symTab* table){
	symNode* node = table->top;
	while(node != NULL){
		symNode* next = node->next;
		free(node);
		node = next;
	}

	free(table);
}

void freeStack(symStack* stack){
	symTab* table = stack->top;
	while(table != NULL){
		symTab* parentTab = table->parentTab;
		free(table);
		table = parentTab;
	}
}

// Thêm danh biểu vào bảng kí hiệu
symNode* pushNode(symTab* table, char* name, VarKind kind, int size){
	symNode* top = table->top;
	symNode* newNode = createNode();

	strcpy(newNode->name, name);
	newNode->kind = kind;
	newNode->size = size;
	newNode->offset = table->size;
	newNode->next = top;

	table->top = newNode;
	table->size = table->size + size;

	return newNode;
}

// symNode* pushNodeProc(symTab* table, char* name, symTab* childTab){
// 	symNode* top = table->top;
// 	symNode* newNode = createNode();

// 	strcpy(newNode->name, name);
// 	newNode->kind = SUBPROC;
// 	newNode->size = 0;
// 	newNode->offset = table->size;
// 	newNode->next = top;
// 	newNode->subProc = childTab;

// 	table->top = newNode;

// 	return newNode
// }

// Thêm bảng kí hiệu vào đỉnh stack
void pushTab(symStack* stack, symTab* table){
	symTab* top = stack->top;
	stack->top = table;
	table->parentTab = top;
}

symTab* popTab(symStack* stack){
	symTab* tab = stack->top;
	if(tab != NULL){
		stack->top = tab->parentTab;
		// freeTab(tab);
	}

	return tab;
}

void printTab(symTab* table){
	printf("\nPrint current table\n");
	if(table != NULL){
		symNode* node = table->top;
		while(node != NULL){
			printf("(%s-%d)\t", node->name, node->kind);
			node = node->next;
		}
	}
}

// Tìm node theo tên trong bảng kí hiệu tương ứng phạm vi hiện tại
symNode* getNodeCurrScope(symTab* table, char* name){
	// printTab(table);
	symNode* node = table->top;
	while(node != NULL){
		if(strcmp(node->name, name) == 0){
			break;
		}
		node = node->next;
	}
	return node;
}

// Tim node theo tên trong bảng kí hiệu tương ứng với phạm vi bên ngoài gần nhất
symNode* getNodeAllScope(symTab* table, char* name){
	symNode* node = NULL;
	while(table != NULL){
		// printTab(table);
		node = table->top;				// Node trên đỉnh của bảng kí hiệu hiện tại

		while(node != NULL){
			if(strcmp(node->name, name) == 0){
				return node;
			}
			node = node->next;
		}

		table = table->parentTab;		// Tìm trong bảng kí hiệu của phạm vi bên ngoài tiếp theo
	}
	return NULL;
}