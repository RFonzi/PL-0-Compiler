#include <stdio.h>
#include <stdlib.h>
#include "error.h"

void error(int code){
    switch(code){
        case EQ_INSTEAD_OF_BECOMES:
            printf("ERROR CODE %d: Use = instead of :=\n", EQ_INSTEAD_OF_BECOMES);
            exit(0);
        case NUM_MUST_FOLLOW_EQ:
            printf("ERROR CODE %d: = must be followed by a number\n", NUM_MUST_FOLLOW_EQ);
            exit(0);
        case EQ_MUST_FOLLOW_IDENT:
            printf("ERROR CODE %d: Identifier must be followed by =\n", EQ_MUST_FOLLOW_IDENT);
            exit(0);
        case IDENT_MUST_FOLLOW_CONST_VAR_PROC:
            printf("ERROR CODE %d: 'const', 'var', 'procedure' must be followed by identifier\n", IDENT_MUST_FOLLOW_CONST_VAR_PROC);
            exit(0);
        case SEMICOLON_COMMA_MISSING:
            printf("ERROR CODE %d: Semicolon or comma missing\n", SEMICOLON_COMMA_MISSING);
            exit(0);
        case INCORRECT_SYM_AFTER_PROC_DECLARATION:
            printf("ERROR CODE %d: Incorrect symbol after procedure declaration\n", INCORRECT_SYM_AFTER_PROC_DECLARATION);
            exit(0);
        case STATEMENT_EXPECTED:
            printf("ERROR CODE %d: Statement expected\n", STATEMENT_EXPECTED);
            exit(0);
        case INCORRECT_SYM_AFTER_STATEMENT_IN_BLOCK:
            printf("ERROR CODE %d: Incorrect symbol after statement part in block\n", INCORRECT_SYM_AFTER_STATEMENT_IN_BLOCK);
            exit(0);
        case PERIOD_EXPECTED:
            printf("ERROR CODE %d: Period expected\n", PERIOD_EXPECTED);
            exit(0);
        case SEMICOLON_MISSING_BETWEEN_STATEMENTS:
            printf("ERROR CODE %d: Semicolon between statements missing\n", SEMICOLON_MISSING_BETWEEN_STATEMENTS);
            exit(0);
        case UNDECLARED_IDENTIFIER:
            printf("ERROR CODE %d: Undeclared identifier\n", UNDECLARED_IDENTIFIER);
            exit(0);
        case ASSIGNMENT_TO_CONST_OR_PROC:
            printf("ERROR CODE %d: Assignment to constant or procedure is not allowed\n", ASSIGNMENT_TO_CONST_OR_PROC);
            exit(0);
        case ASSIGNMENT_OP_EXPECTED:
            printf("ERROR CODE %d: Assignment operator expected\n", ASSIGNMENT_OP_EXPECTED);
            exit(0);
        case IDENT_MUST_FOLLOW_CALL:
            printf("ERROR CODE %d: 'call' must be followed by an identifier\n", IDENT_MUST_FOLLOW_CALL);
            exit(0);
        case CONST_OR_VAR_MEANINGLESS:
            printf("ERROR CODE %d: Call of a constant or variable is meaningless\n", CONST_OR_VAR_MEANINGLESS);
            exit(0);
        case THEN_EXPECTED:
            printf("ERROR CODE %d: 'then' expected\n", THEN_EXPECTED);
            exit(0);
        case SEMICOLON_OR_RIGHTBRACE_EXPECTED:
            printf("ERROR CODE %d: Semicolon or } expected\n", SEMICOLON_OR_RIGHTBRACE_EXPECTED);
            exit(0);
        case DO_EXPECTED:
            printf("ERROR CODE %d: 'do' expected\n", DO_EXPECTED);
            exit(0);
        case INCORRECT_SYM_FOLLOWING_STATEMENT:
            printf("ERROR CODE %d: Incorrect symbol following statement\n", INCORRECT_SYM_FOLLOWING_STATEMENT);
            exit(0);
        case RELATIONAL_OP_EXPECTED:
            printf("ERROR CODE %d: Relational operator expected\n", RELATIONAL_OP_EXPECTED);
            exit(0);
        case EXPRESSION_CANNOT_CONTAIN_PROC_IDENTIFIER:
            printf("ERROR CODE %d: Expression must not contain a procedure identifier\n", EXPRESSION_CANNOT_CONTAIN_PROC_IDENTIFIER);
            exit(0);
        case RIGHTPAREN_MISSING:
            printf("ERROR CODE %d: Right parenthesis missing\n", RIGHTPAREN_MISSING);
            exit(0);
        case PRECEDING_FACTOR_CANNOT_BEGIN_WITH_SYM:
            printf("ERROR CODE %d: The preceding factor cannot begin with this symbol\n", PRECEDING_FACTOR_CANNOT_BEGIN_WITH_SYM);
            exit(0);
        case EXPRESSION_CANNOT_BEGIN_WITH_SYM:
            printf("ERROR CODE %d: An expression cannot begin with this symbol\n", EXPRESSION_CANNOT_BEGIN_WITH_SYM);
            exit(0);
        case NUM_TOO_LARGE:
            printf("ERROR CODE %d: This number is too large\n", NUM_TOO_LARGE);
            exit(0);
        case IDENT_EXPECTED_IN_VAR:
            printf("ERROR CODE %d: Identifier expected after 'var'\n", IDENT_EXPECTED_IN_VAR);
            exit(0);
        case IDENT_MUST_FOLLOW_READ:
            printf("ERROR CODE %d: Identifier expected after 'read'\n", IDENT_MUST_FOLLOW_READ);
            exit(0);
        case IDENT_MUST_FOLLOW_WRITE:
            printf("ERROR CODE %d: Identifier expected after 'write'\n", IDENT_MUST_FOLLOW_WRITE);
            exit(0);
        case BECOMES_MUST_FOLLOW_IDENT_IN_STATEMENT:
            printf("ERROR CODE %d: ':=' must follow identifier in statement\n", BECOMES_MUST_FOLLOW_IDENT_IN_STATEMENT);
            exit(0);


    }
}
