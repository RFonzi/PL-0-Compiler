#include "symboltable.h"
#include <string.h>
#include <stdio.h>

int numSymbols = 0;

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

void printSymbolTable(FILE *fp){
    fp = fopen("symlist.txt", "w");

    fprintf(fp, "Name  Type  Level Value\n");

    int i;
    for(i = 0; i < numSymbols; i++){
        if(symbolTable[i].kind == 1)
            char* nameKind = "const";
        else if(symbolTable[i].kind == 2)
            char* nameKind = "var";
        else if(symbolTable[i].kind == 3)
            char* nameKind = "proc";


        fprintf(fp, "%-6s%-6s%-6d%-6d", symbolTable[i].name,
                                        nameKind,
                                        symbolTable[i].level,
                                        symbolTable[i].addr);
    }

    fclose(fp);
}
