#include "symboltable.h"
#include <string.h>

int numSymbols = 0;
Symbol symbolTable[MAX_SYMBOL_TABLE_SIZE];

void addConst(int kind, char* name, int val){
    symbolTable[numSymbols].kind = kind;
    strcpy(symbolTable[numSymbols].name, name);
    symbolTable[numSymbols].val = val;

    numSymbols++;
}

void addVarOrProc(int kind, char* name, int level, int addr){
    symbolTable[numSymbols].kind = kind;
    strcpy(symbolTable[numSymbols].name, name);
    symbolTable[numSymbols].level = level;
    symbolTable[numSymbols].addr = addr;

    numSymbols++;
}
