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

    fprintf(fp, "Name        Type        Level       Value       \n");

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

        fprintf(fp, "%-11s %-11s %-11d %-11d\n", symbolTable[i].name,
                                        nameKind,
                                        symbolTable[i].level,
                                        symbolTable[i].val);
    }

    fclose(fp);
}

void deleteTopSymbolLevel(){
    int i, maxSize = 0;

    // Find out what the max level is
    for(i = 0; i < MAX_SYMBOL_TABLE_SIZE; i++){
        if(symbolTable[i].level > maxSize){
            maxSize = symbolTable[i].level;
        }
    }

    // Remove it
    for(i = 0; i < MAX_SYMBOL_TABLE_SIZE; i++){
        if(symbolTable[i].level == maxSize){
            symbolTable[i].kind = 0;
            symbolTable[i].name = {0};
            symbolTable[i].val = 0;
            symbolTable[i].level = 0;
            symbolTable[i].addr = 0;

            numSymbols--;
        }
    }
}