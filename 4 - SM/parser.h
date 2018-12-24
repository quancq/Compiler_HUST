#include "semantic.h"

void error(int errorCode);	
VarType factor(symTab* table);			
VarType term(symTab* table);			
VarType expression(symTab* table);		
VarType condition(symTab* table);		
void statement(symTab* table);		
void block(symTab* table);
void program();
void copyArray(VarType* src, VarType* dst, int numElmss);