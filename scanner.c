//TODO Make symbol table


#include "scanner.h"
#include "tokenlist.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

const char *word[] = { "const", "var", "procedure", "call", "begin", "end", "if",
                        "then", "else", "while", "do", "read", "write", "odd"};

const int wsym[] = {constsym, varsym, procsym, callsym, beginsym, endsym, ifsym,
                     thensym, elsesym, whilesym, dosym, readsym, writesym, oddsym};


Token *tokenList = NULL;



int scanFile(FILE *fp){
    if(fp == NULL){
        printf("File could not be found.\n");
        return -1;
    }

    printf("Reading in tokens...");

    while(fpeek(fp) != EOF){
        int tokenStart = fgetc(fp);
        int error = 0;

        //If we read whitespace, continue;
        if(tokenStart == '\n' || tokenStart == '\r' || tokenStart == '\t' ||
           tokenStart == '\v' || tokenStart == ' '){
               continue;
        }

        error = checkSymbols(fp, tokenStart);

        if(error == 4){
           printf("ERROR Invalid symbol '%c'\n", tokenStart);
           exit(4);
        }
        else if(error == 0){
           continue;
        }

        error = checkReservedWords(fp, tokenStart);

        if(error == -1){

           error = checkVariable(fp, tokenStart);

           if(error == 3){
               printf("ERROR Name exceeds 11 characters\n");
               exit(3);
           }
           else if(error == 0){
               continue;
           }
        }
        else if(error == 0){
           continue;
        }

        error = checkNumber(fp, tokenStart);

        if(error == 2){
           printf("ERROR number exceeds 5 digits\n");
           exit(2);
        }
        else if(error == 1){
           printf("ERROR Variables can't start with a letter");
           exit(1);
        }
        else if(error == 0){
           continue;
        }
    }

    createToken("null", 1);
    printf("COMPLETE\n");
    return 0;


}

int cleanFile(FILE *input, FILE *cleanInput){
    printf("Cleaning input...");

    while(fpeek(input) != EOF){
        char nextChar = fgetc(input); //Grab the next char from input

        if(nextChar == '/'){ //Possibly a comment
            if(fpeek(input) == '*'){ //Start comment block
                fgetc(input); // "/*"
                while(fpeek(input) != '*') // "/*..."
                    fgetc(input);

                fgetc(input); // "/*...*"
                fgetc(input); // "/*...*/"

                continue; //Comment block over
            }
        }

        fputc(nextChar, cleanInput); //Copy the char over
    }

    printf("COMPLETE\n");
}

int genLexList(FILE *fp){
    printf("Generating lex list...");

    Token *traversal = tokenList;
    while(traversal != NULL){
        fprintf(fp, "%d ", traversal->type);
        if(traversal->type == 2 || traversal->type == 3)
            fprintf(fp, "%s ", traversal->lexeme);

        traversal = traversal->next;
    }

    printf("COMPLETE\n");
}

int genLexTable(FILE *fp){
    printf("Generating lex table...");

    fprintf(fp, "Lexeme       Token Type\n");
    fprintf(fp, "-----------------------\n");

    Token *traversal = tokenList;
    while(traversal != NULL){
        fprintf(fp, "%-11s  %d\n", traversal->lexeme, traversal->type);
        traversal = traversal->next;
    }

    printf("COMPLETE\n");
}

void createToken(char* lexeme, int type){

    Token *tok = malloc(sizeof(Token));

    char lexarray[20] = {'\0'};
    int i;

    //Convert pointer array to char array
    for(i = 0; i < 20; i++){
        lexarray[i] = *(lexeme + sizeof(char) * i);
        tok->lexeme[i] = lexarray[i];
    }

    tok->type = type;

    insertToken(&tokenList, tok);

    free(tok);

}

int fpeek(FILE *fp){
    int c;

    c = fgetc(fp);
    ungetc(c, fp);

    return c;
}

int checkSymbols(FILE *fp, int tokenStart){

    switch(tokenStart){
        case '+': //Plus
            createToken("+", plussym);
            return(0);

        case '-': //Minus
            createToken("-", minussym);
            return(0);

        case '*': //Mult
            createToken("*", multsym);
            return(0);

        case '/': //Slash
            createToken("/", slashsym);
            return(0);

        case '(': //LParen
            createToken("(", lparentsym);
            return(0);

        case ')': //RParen
            createToken(")", rparentsym);
            return(0);

        case '=': //Equals
            createToken("=", eqsym);
            return(0);

        case ',': //Comma
            createToken(",", commasym);
            return(0);

        case '.': //Period
            createToken(".", periodsym);
            return(0);

        case '<': //Less, leq, neq
            if(fpeek(fp) == '='){
                fgetc(fp);
                createToken("<=", leqsym);
            }
            else if(fpeek(fp) == '>'){
                fgetc(fp);
                createToken("<>", neqsym);
            }
            else{
                createToken("<", lessym);
            }
            return(0);

        case '>': //gtr & geq
            if(fpeek(fp) == '='){
                fgetc(fp);
                createToken(">=", geqsym);
            }
            else
                createToken(">", gtrsym);
            return(0);

        case ';': //Semicolon
            createToken(";", semicolonsym);
            return(0);

        case ':': //Becomes
            if(fpeek(fp) == '='){
                createToken(":=", becomessym);
                fgetc(fp);
                return(0);
            }
            break;
        default:
            if(!(isalnum(tokenStart) || tokenStart == '_'))
                return 4; //Not a valid symbol


        return -1;

    }

}

int checkReservedWords(FILE *fp, int tokenStart){

    fpos_t resetpos;
    fgetpos(fp, &resetpos);

    if(tokenStart == 'b'){
        if(fgetc(fp) == 'e'){
            if(fgetc(fp) == 'g'){
                if(fgetc(fp) == 'i'){
                    if(fgetc(fp) == 'n'){
                        if(!isalnum(fpeek(fp))){
                            createToken("begin", beginsym);
                            return 0;
                        }
                    }
                }
            }
        }
    }

    fsetpos(fp, &resetpos);

    if(tokenStart == 'c'){
        if(fgetc(fp) == 'a'){
            if(fgetc(fp) == 'l'){
                if(fgetc(fp) == 'l'){
                    if(!isalnum(fpeek(fp))){
                        createToken("call", callsym);
                        return 0;
                    }
                }
            }
        }
    }

    fsetpos(fp, &resetpos);

    if(tokenStart == 'c'){
        if(fgetc(fp) == 'o'){
            if(fgetc(fp) == 'n'){
                if(fgetc(fp) == 's'){
                    if(fgetc(fp) == 't'){
                        if(!isalnum(fpeek(fp))){
                            createToken("const", constsym);
                            return 0;
                        }
                    }
                }
            }
        }
    }

    fsetpos(fp, &resetpos);

    if(tokenStart == 'd'){
        if(fgetc(fp) == 'o'){
            if(!isalnum(fpeek(fp))){
                createToken("do", dosym);
                return 0;
            }
        }
    }

    fsetpos(fp, &resetpos);

    if(tokenStart == 'e'){
        if(fgetc(fp) == 'l'){
            if(fgetc(fp) == 's'){
                if(fgetc(fp) == 'e'){
                    if(!isalnum(fpeek(fp))){
                        createToken("else", elsesym);
                        return 0;
                    }
                }
            }
        }
    }

    fsetpos(fp, &resetpos);

    if(tokenStart == 'e'){
        if(fgetc(fp) == 'n'){
            if(fgetc(fp) == 'd'){
                if(!isalnum(fpeek(fp))){
                    createToken("end", endsym);
                    return 0;
                }
            }
        }
    }

    fsetpos(fp, &resetpos);

    if(tokenStart == 'i'){
        if(fgetc(fp) == 'f'){
            if(!isalnum(fpeek(fp))){
                createToken("if", ifsym);
                return 0;
            }
        }
    }

    fsetpos(fp, &resetpos);

    if(tokenStart == 'o'){
        if(fgetc(fp) == 'd'){
            if(fgetc(fp) == 'd'){
                if(!isalnum(fpeek(fp))){
                    createToken("odd", oddsym);
                    return 0;
                }
            }
        }
    }

    fsetpos(fp, &resetpos);

    if(tokenStart == 'p'){
        if(fgetc(fp) == 'r'){
            if(fgetc(fp) == 'o'){
                if(fgetc(fp) == 'c'){
                    if(fgetc(fp) == 'e'){
                        if(fgetc(fp) == 'd'){
                            if(fgetc(fp) == 'u'){
                                if(fgetc(fp) == 'r'){
                                    if(fgetc(fp) == 'e'){
                                        if(!isalnum(fpeek(fp))){
                                            createToken("procedure", procsym);
                                            return 0;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    fsetpos(fp, &resetpos);

    if(tokenStart == 'r'){
        if(fgetc(fp) == 'e'){
            if(fgetc(fp) == 'a'){
                if(fgetc(fp) == 'd'){
                    if(!isalnum(fpeek(fp))){
                        createToken("read", readsym);
                        return 0;
                    }
                }
            }
        }
    }

    fsetpos(fp, &resetpos);

    if(tokenStart == 't'){
        if(fgetc(fp) == 'h'){
            if(fgetc(fp) == 'e'){
                if(fgetc(fp) == 'n'){
                    if(!isalnum(fpeek(fp))){
                        createToken("then", thensym);
                        return 0;
                    }
                }
            }
        }
    }

    fsetpos(fp, &resetpos);


    if(tokenStart == 'v'){
        if(fgetc(fp) == 'a'){
            if(fgetc(fp) == 'r'){
                if(!isalnum(fpeek(fp))){
                    createToken("var", varsym);
                    return 0;
                }
            }
        }
    }

    fsetpos(fp, &resetpos);

    if(tokenStart == 'w'){
        if(fgetc(fp) == 'h'){
            if(fgetc(fp) == 'i'){
                if(fgetc(fp) == 'l'){
                    if(fgetc(fp) == 'e'){
                        if(!isalnum(fpeek(fp))){
                            createToken("while", whilesym);
                            return 0;
                        }
                    }
                }
            }
        }
    }

    fsetpos(fp, &resetpos);

    if(tokenStart == 'w'){
        if(fgetc(fp) == 'r'){
            if(fgetc(fp) == 'i'){
                if(fgetc(fp) == 't'){
                    if(fgetc(fp) == 'e'){
                        if(!isalnum(fpeek(fp))){
                            createToken("write", writesym);
                            return 0;
                        }
                    }
                }
            }
        }
    }

    fsetpos(fp, &resetpos);

    return -1;

}

int checkVariable(FILE *fp, int tokenStart){
    if(isalpha(tokenStart) || tokenStart == '_'){
        int i;
        char lexeme[20] = {'\0'};
        lexeme[0] = tokenStart;

        //Loop through the next 1-10 characters to build lexeme
        for(i = 1; i < 11; i++){
            char workingChar = fpeek(fp);
            if(isalnum(workingChar) || workingChar == '_'){
                lexeme[i] = fgetc(fp);
            }
            else{
                break;
            }
        }

        //If there's a 12th alphanumeric character after the 11th, name is too long
        if(i == 11 && (isalnum(fpeek(fp)) || fpeek(fp) == '_')){
            return 3; //Error, name is too long
        }

        createToken(lexeme, identsym);

        return 0;

    }
    else{
        return -1;
    }
}

int checkNumber(FILE *fp, int tokenStart){
    if(isdigit(tokenStart)){
        int i;
        char lexeme[20] = {'\0'};
        lexeme[0] = tokenStart;

        //Loop through the next 1-4 digits to build lexeme
        for(i = 1; i < 5; i++){
            char workingDigit = fpeek(fp);
            if(isdigit(workingDigit)){
                lexeme[i] = fgetc(fp);
            }
            else if(isalpha(workingDigit) || workingDigit == '_'){
                return 1; //Error, Variable does not start with letter
            }
            else{
                break;
            }
        }

        if(i == 5 && isdigit(fpeek(fp))){
            return 2; //Error, number is too long
        }

        createToken(lexeme, numbersym);

        return 0;

    }
}
