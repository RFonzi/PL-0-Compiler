#ifndef CPU_H
#define CPU_H

#include <stdio.h>
#include "stack.h"

#define MAX_CODE_LENGTH 500
#define MAX_LEXI_LEVELS 3 // Haven't used this (yet)

typedef struct {
    int op; // opcode
    int l;  // L - Lexicographical levels down
    int m;  // M - Value (how it's used depends on op)
} instruction;

int halt;
int sp; // Stack pointer
int bp; // Base pointer
int pc; // Program counter
instruction ir; // Instruction register
int lexi; // Current Lexicographical level
extern const char *opcodeNames[]; // List of opcode names
extern const char *oprcodeNames[]; // List of oprcode names
extern int arArray[MAX_STACK_SIZE / 5]; // Array to keep track of when activation records start.
instruction code[MAX_CODE_LENGTH]; // Code store
int ar;

enum OPCODES{
    LIT = 1,
    OPR = 2,
    LOD = 3,
    STO = 4,
    CAL = 5,
    INC = 6,
    JMP = 7,
    JPC = 8,
    SIO1 = 9,
    SIO2 = 10,
    SIO3 = 11
};

enum RTYPE{
    RET = 0,
    NEG = 1,
    ADD = 2,
    SUB = 3,
    MUL = 4,
    DIV = 5,
    ODD = 6,
    MOD = 7,
    EQL = 8,
    NEQ = 9,
    LSS = 10,
    LEQ = 11,
    GTR = 12,
    GEQ = 13
};



// Stores all code from "mcode.txt" and puts it into the code store
void storeCode();

// Fetches the next instruction
void fetch();

// Executes the fetched instruction
void execute();

// Returns the base of an AR that's l levels down
// Inputs: l - How many levels you want to go down
//         base - The base to start going down from (bp)
// Outputs: Index of the AR you travelled down to
int base(int l, int base);

void outputCode(FILE* fp);

#endif // CPU_H
