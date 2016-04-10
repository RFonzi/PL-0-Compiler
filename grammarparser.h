#ifndef GRAMMAR_PARSER
#define GRAMMAR_PARSER

//extern int offset;        // AR offset
extern int tempKind;      // const = 1, var = 2, proc = 3
extern char tempName[12]; // name up to 11 chars
extern int tempVal;       // number (ASCII value)
extern int tempLevel;     // L level
extern int tempAddr;      // M address

void program(Token *tokenlist);
void block (Token **tempList);
void condition (Token **tempList);
void statement(Token **tempList);
void expression (Token **tempList);
void term (Token **tempList);
void factor(Token **tempList);
int relation (Token **tempList);

#endif //GRAMMAR_PARSER
