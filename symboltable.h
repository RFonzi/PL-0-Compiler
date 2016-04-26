#ifndef SYMBOL_TABLE
#define SYMBOL_TABLE

#include <stdio.h>

#define MAX_SYMBOL_TABLE_SIZE 100

typedef struct symbol{
     int kind;      // const = 1, var = 2, proc = 3
     char name[12]; // name up to 11 chars
     int val;       // number (ASCII value)
     int level;     // L level
     int addr;      // M address
 } Symbol;

extern int numSymbols;
extern Symbol symbolTable[MAX_SYMBOL_TABLE_SIZE];

void addConst(int kind, char* name, int val);
void addProc(int kind, char* name, int level, int val);
void addVar(int kind, char*name, int val, int level, int addr);
void prepareSymbolTable (FILE *fp);
void printSymbol(FILE *fp);
void printSymbolTable(FILE *fp);
void deleteTopSymbolLevel();

#endif //SYMBOL_TABLE
