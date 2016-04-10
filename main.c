#include <stdio.h>
#include "scanner.h"
#include "grammarparser.h"
#include "symboltable.h"

int main(int argc, char **argv){
    //Clear out existing files...
    FILE *clear = fopen("cleaninput.txt", "w");
    fclose(clear);
    clear = fopen("lexemetable.txt", "w");
    fclose(clear);
    clear = fopen("lexemelist.txt", "w");
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

    //Parsing grammar
    program(tokenList);
    printf("Done! No errors!\n");

    printSymbolTable(fp);
}
