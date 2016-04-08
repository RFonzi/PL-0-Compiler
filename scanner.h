#ifndef SCANNER_H
#define SCANNER_H

#include <stdio.h>
#include "tokenlist.h"

#define norw 15 /* number of reserved words */
#define imax 32767 /* maximum integer value */
#define cmax 11 /* maximum number of chars for idents */
#define nestmax 5 /* maximum depth of block nesting */
#define strmax 256 /* maximum length of strings */

extern const char *word[];
extern const int wsym[];
extern int ssym[256];
extern const char *lexemeList;
//extern int tokenListSize;
extern Token *tokenList;

typedef enum{
nulsym = 1,
identsym,
numbersym,
plussym,
minussym,
multsym,
slashsym,
oddsym,
eqsym,
neqsym,
lessym,
leqsym,
gtrsym,
geqsym,
lparentsym,
rparentsym,
commasym,
semicolonsym,
periodsym,
becomessym,
beginsym,
endsym,
ifsym,
thensym,
whilesym,
dosym,
callsym,
constsym,
varsym,
procsym,
writesym,
readsym,
elsesym
} token_type;

int scanFile(FILE *fp);
int cleanFile(FILE *input, FILE *cleanInput);
int genLexList(FILE *fp);
int genLexTable(FILE *fp);
void createToken(char* lexeme, int type);
int fpeek(FILE *fp);
int checkSymbols(FILE *fp, int tokenStart);
int checkReservedWords(FILE *fp, int tokenStart);
int checkVariable(FILE *fp, int tokenStart);
int checkNumber(FILE *fp, int tokenStart);

#endif //SCANNER_H
