#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"
#include "stack.h"
#include "grammarparser.h"

const char *opcodeNames[] = {"   ", "lit", "opr", "lod", "sto", "cal", "inc", "jmp", "jpc", "sio", "sio", "sio"};
const char *oprcodeNames[] = {"ret", "neg", "add", "sub", "mul", "div", "odd", "mod", "eql", "neq", "lss", "leq", "gtr", "geq"};
int halt = 0;
int sp = 0;
int bp = 1;
int pc = 0;
int lexi = 0;
int arArray[] = {0};
int ar = 0;
instruction ir;

void storeCode(){
    FILE *ifp = fopen("mcode.txt", "r");

    if(ifp == NULL){
        printf("Can't open 'mcode.txt'!\n");
        exit(1);
    }

    int codeCount = 0;
    while(!feof(ifp)){
        if(codeCount >= MAX_CODE_LENGTH){
            printf("Max code length reached. Cannot continue.\n");
            exit(1);
        }

        if(fscanf(ifp, "%d %d %d", &code[codeCount].op, &code[codeCount].l, &code[codeCount].m) != 3){
            printf("End of code detected. Continuing...\n");
        }

        codeCount++;
    }

    fclose(ifp);
}

void fetch(){
    ir = code[pc];
    pc++;
}

void execute(){

    // Begin NON R-Type
    switch(ir.op){
        case LIT:
            sp = sp + 1;
            stack[sp] = ir.m;
            break;
        case OPR:
            // Begin R-Type
            switch(ir.m){
                case RET:
                    sp = bp - 1;
                    pc = stack[sp + 4];
                    bp = stack[sp + 3];
                    arArray[ar - 1] = 0;
                    ar = ar - 1;
                    break;
                case NEG:
                    stack[sp] = -stack[sp];
                    break;
                case ADD:
                    sp = sp - 1;
                    stack[sp] = stack[sp] + stack[sp + 1];
                    break;
                case SUB:
                    sp = sp - 1;
                    stack[sp] = stack[sp] - stack[sp + 1];
                    break;
                case MUL:
                    sp = sp - 1;
                    stack[sp] = stack[sp] * stack[sp + 1];
                    break;
                case DIV:
                    sp = sp - 1;
                    stack[sp] = stack[sp] / stack[sp + 1];
                    break;
                case ODD:
                    stack[sp] = stack[sp] % 2;
                    break;
                case MOD:
                    sp = sp - 1;
                    stack[sp] = stack [sp] % stack[sp + 1];
                    break;
                case EQL:
                    sp = sp - 1;
                    stack[sp] = stack[sp] == stack[sp + 1];
                    break;
                case NEQ:
                    sp = sp - 1;
                    stack[sp] = stack[sp] != stack[sp + 1];
                    break;
                case LSS:
                    sp = sp - 1;
                    stack[sp] = stack[sp] < stack[sp + 1];
                    break;
                case LEQ:
                    sp = sp - 1;
                    stack[sp] = stack[sp] <= stack[sp + 1];
                    break;
                case GTR:
                    sp = sp - 1;
                    stack[sp] = stack[sp] > stack[sp + 1];
                    break;
                case GEQ:
                    sp = sp - 1;
                    stack[sp] = stack[sp] >= stack[sp + 1];
                    break;
            }
            // End R-Type
            break;
        case LOD:
            sp = sp + 1;
            stack[sp] = stack[base(ir.l, bp) + ir.m];
            break;
        case STO:
            stack[base(ir.l, bp) + ir.m] = stack[sp];
            sp = sp - 1;
            break;
        case CAL:
            lexi = lexi + 1;
            stack[sp + 1] = 0;
            stack[sp + 2] = base(ir.l, bp);
            stack[sp + 3] = bp;
            stack[sp + 4] = pc;
            bp = sp + 1;
            arArray[ar] = bp;
            ar = ar + 1;
            pc = ir.m;
            break;
        case INC:
            sp = sp + ir.m;
            break;
        case JMP:
            pc = ir.m;
            break;
        case JPC:
            if(stack[sp] == 0){
                pc = ir.m;
            }
            sp = sp - 1;
            break;
        case SIO1:
            printf("%d\n", stack[sp]);
            sp = sp - 1;
            break;
        case SIO2:
            sp = sp + 1;
            printf("Input required: ");
            scanf("%d", &stack[sp]);
            break;
        case SIO3:
            halt = 1;
            sp = 0;
            break;

        // End NON R-Type
    }
}

int base(int l, int base)
{
    int b1; //find base L levels down
    b1 = base;
    while (l > 0)
    {
        b1 = stack[b1 + 1];
        l--;
    }
    return b1;
}

void outputCode(FILE* fp){
    int i;
    for(i = 0; i < codeCounter; i++){
        fprintf(fp, "%d %d %d\n", code[i].op, code[i].l, code[i].m);
    }
    
}
