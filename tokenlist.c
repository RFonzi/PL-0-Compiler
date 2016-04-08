#include <stdlib.h>
#include <string.h>
#include "tokenlist.h"

void insertToken(Token **root, Token *token){
    //Create new token
    Token *newToken = malloc(sizeof(Token));
    strcpy(newToken->lexeme, token->lexeme);
    newToken->type = token->type;
    newToken->next = NULL;

    //If this is the start of the list, make the first element
    if(*root == NULL){
        *root = malloc(sizeof(Token));
        *(*root) = *newToken;
    }
    else{
        Token *traversal = *root;

        //Go to the end of the current list
        while(traversal->next != NULL){
            traversal = traversal->next;
        }

        //Put new token at the end
        traversal->next = newToken;

    }



}
