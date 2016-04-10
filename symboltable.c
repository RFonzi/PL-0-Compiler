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

void addProc(int kind, char* name, int level, int addr){
    symbolTable[numSymbols].kind = kind;
    strcpy(symbolTable[numSymbols].name, name);
    symbolTable[numSymbols].level = level;
    symbolTable[numSymbols].addr = addr;

    numSymbols++;
}

void addVar(int kind, char*name, int val, int level, int addr){
	symbolTable[numSymbols].kind = kind;
    strcpy(symbolTable[numSymbols].name, name);
    symbolTable[numSymbols].level = level;
    symbolTable[numSymbols].addr = addr;
	symbolTable[numSymbols].val = val;
	
    numSymbols++;
}

void printSymbolTable(FILE *fp){
    fp = fopen("symlist.txt", "w");

    fprintf(fp, "Name  Type  Level Value\n");

    int i;
    for(i = 0; i < numSymbols; i++){
        char nameKind[6];
        if(symbolTable[i].kind == 1){
            strcpy(nameKind, "const");
        }
        else if(symbolTable[i].kind == 2){
            strcpy(nameKind, "var");
        }
        else if(symbolTable[i].kind == 3){
            strcpy(nameKind, "proc");
        }

        fprintf(fp, "%-6s%-6s%-6d%-6d\n", symbolTable[i].name,
                                        nameKind,
                                        symbolTable[i].level,
                                        symbolTable[i].val);
    }

    fclose(fp);
}
