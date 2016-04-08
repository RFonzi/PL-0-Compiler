#ifndef TOKEN_LIST
#define TOKEN_LIST

//Linked List to keep track of tokens generated

typedef struct token {
    char lexeme[20];
    int type;
    struct token *next;
} Token;

void insertToken(Token **root, Token *token);

#endif //TOKEN_LIST
