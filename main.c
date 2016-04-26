#include <stdio.h>
#include "scanner.h"
#include "grammarparser.h"
#include "symboltable.h"
#include "cpu.h"

int main(int argc, char **argv){
    //Clear out existing files...
    FILE *clear = fopen("cleaninput.txt", "w");
    fclose(clear);
    clear = fopen("lexemetable.txt", "w");
    fclose(clear);
    clear = fopen("lexemelist.txt", "w");
    fclose(clear);
    clear = fopen("symlist.txt", "w");
    fclose(clear);
    clear = fopen("mcode.txt", "w");
    fclose(clear);
    clear = NULL;


    //Clearing input
    FILE *input = fopen("input.txt", "r");
    FILE *cleanInput = fopen("cleaninput.txt", "w+");
    cleanFile(input, cleanInput);

    fclose(cleanInput);
    fclose(input);

    //Reading tokens from clean input
    cleanInput = fopen("cleaninput.txt", "r");
    scanFile(cleanInput);
    fclose(cleanInput);

    //Generating lexeme table
    FILE *fp = fopen("lexemetable.txt", "w");
    genLexTable(fp);
    fprintf(fp, "\n");
    fclose(fp);

    //Generating lexeme list
    fp = fopen("lexemelist.txt", "w");
    genLexList(fp);
    fprintf(fp, "\n");
    fclose(fp);
	
	//Outputting header for file
	prepareSymbolTable(fp);
    //Parsing grammar
    program(tokenList);

    //Generating mcode
    fp = fopen("mcode.txt", "w");
    outputCode(fp);
    fclose(fp);

    //Run program
    fp = fopen("stacktrace.txt", "w");

    fprintf(fp, "Line  OP    L     M\n");

    int i = 0;

    // Begin outputting code store
    for(i = 0; i < MAX_CODE_LENGTH; i++){
        if(code[i].op == 0 && code[i].l == 0 && code[i].m == 0){
            printf("End of code output.\n");
            break;
        }

        fprintf(fp, "%2d    ", i);
        fprintf(fp, "%s   ", opcodeNames[code[i].op]);
        fprintf(fp, "%d     ", code[i].l);
        fprintf(fp, "%d  ", code[i].m);

        if(code[i].op == OPR){
            fprintf(fp, "(%s)", oprcodeNames[code[i].m]);
        }

        fprintf(fp, "\n");
    }
    // End outputting code store

    printf("Code store complete. Executing instructions...\n");
    fprintf(fp, "\n\n\n");
    fprintf(fp, "Line  OP    L     M     pc    bp    sp    stack\n");

    while(!halt){

        fetch();

        // Begin printing before state change
        fprintf(fp, "%2d    ", pc - 1);
        fprintf(fp, "%s   ", opcodeNames[ir.op]);
        fprintf(fp, "%-2d    ", ir.l);
        fprintf(fp, "%-2d    ", ir.m);
        // End printing before state change


        execute();

        // Begin printing after state change
        fprintf(fp, "%-2d    ", pc);
        fprintf(fp, "%-2d    ", bp);
        fprintf(fp, "%-2d    ", sp);

        int i;
        int j;

        for(i = 1; i <= sp; i++){

            for(j = 0; j <= sp; j++){
                if(i == arArray[j]){
                    fprintf(fp, "| ");
                }
            }

            fprintf(fp, "%d ", stack[i]);

        }

        fprintf(fp, "\n");
        // End printing after state change

    }

    if(halt){
        printf("Program halted successfully.\n");
    }


    fclose(fp);


    printf("Done! No errors!\n");

    
}
