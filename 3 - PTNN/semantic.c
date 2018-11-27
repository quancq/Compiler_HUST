#include <stdlib.h>
#include <string.h>
#include "semantic.h"

void printTab(symTab* table);

symNode* createNode(){
	symNode* node = (symNode*) malloc(sizeof(symNode));
	node->next = NULL;
	return node;
}

symTab* createTab(){
	symTab* tab = (symTab*) malloc(sizeof(symTab));
	tab->next = NULL;
	tab->parentTab = NULL;
	return tab;
}

void freeTab(symTab* table){
	symNode* node = table->next;
	while(node != NULL){
		symNode* next = node->next;
		free(node);
		node = next;
	}

	free(table);
}

void freeStack(symStack* stack){
	symTab* table = stack->parentTab;
	while(table != NULL){
		symTab* parentTab = table->parentTab;
		free(table);
		table = parentTab;
	}
}

// Thêm danh biểu vào bảng kí hiệu
void pushNode(symTab* table, char* name, VarType type){
	symNode* top = table->next;
	symNode* newNode = createNode();

	strcpy(newNode->name, name);
	newNode->type = type;
	newNode->next = top;

	table->next = newNode;
}

// Thêm bảng kí hiệu vào đỉnh stack
void pushTab(symStack* stack, symTab* table){
	symTab* top = stack->parentTab;
	stack->parentTab = table;
	table->parentTab = top;
}

symTab* popTab(symStack* stack){
	symTab* tab = stack->parentTab;
	if(tab != NULL){
		stack->parentTab = tab->parentTab;
	}

	return tab;
}

void printTab(symTab* table){
	printf("\nPrint current table\n");
	if(table != NULL){
		symNode* node = table->next;
		while(node != NULL){
			printf("(%s-%d)\t", node->name, node->type);
			node = node->next;
		}
	}
}

// Tìm node theo tên trong bảng kí hiệu tương ứng phạm vi hiện tại
symNode* getNodeCurrScope(symTab* table, char* name){
	// printTab(table);
	symNode* node = table->next;
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
		node = table->next;				// Node trên đỉnh của bảng kí hiệu hiện tại

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