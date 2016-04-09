#ifndef GRAMMAR_PARSER
#define GRAMMAR_PARSER

void program(Token *tokenlist);
void block (Token *tempList);
void condition (Token *tempList);
void statement(Token *tempList);
void expression (Token *tempList);
void term (Token *tempList);
void factor(Token *tempList);
int relation (Token *tempList);

#endif //GRAMMAR_PARSER
