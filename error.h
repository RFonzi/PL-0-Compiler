#ifndef ERROR
#define ERROR

enum ERRORS{
    EQ_INSTEAD_OF_BECOMES = 1,
    NUM_MUST_FOLLOW_EQ = 2,
    EQ_MUST_FOLLOW_IDENT = 3,
    IDENT_MUST_FOLLOW_CONST_VAR_PROC = 4,
    SEMICOLON_COMMA_MISSING = 5,
    INCORRECT_SYM_AFTER_PROC_DECLARATION = 6,
    STATEMENT_EXPECTED = 7,
    INCORRECT_SYM_AFTER_STATEMENT_IN_BLOCK = 8,
    PERIOD_EXPECTED = 9,
    SEMICOLON_MISSING_BETWEEN_STATEMENTS = 10,
    UNDECLARED_IDENTIFIER = 11,
    ASSIGNMENT_TO_CONST_OR_PROC = 12,
    ASSIGNMENT_OP_EXPECTED = 13,
    IDENT_MUST_FOLLOW_CALL = 14,
    CONST_OR_VAR_MEANINGLESS = 15,
    THEN_EXPECTED = 16,
    SEMICOLON_OR_RIGHTBRACE_EXPECTED = 17,
    DO_EXPECTED = 18,
    INCORRECT_SYM_FOLLOWING_STATEMENT = 19,
    RELATIONAL_OP_EXPECTED = 20,
    EXPRESSION_CANNOT_CONTAIN_PROC_IDENTIFIER = 21,
    RIGHTPAREN_MISSING = 22,
    PRECEDING_FACTOR_CANNOT_BEGIN_WITH_SYM = 23,
    EXPRESSION_CANNOT_BEGIN_WITH_SYM = 24,
    NUM_TOO_LARGE = 25
    IDENT_EXPECTED_IN_VAR = 26;
}

void error(int code);

#endif //ERROR
