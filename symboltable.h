#ifndef SYMBOL_TABLE
#define SYMBOL_TABLE

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
void addVarOrProc(int kind, char* name, int level, int addr);

#endif //SYMBOL_TABLE