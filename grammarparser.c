#include <string.h>
#include <stdlib.h>
#include "scanner.h"
#include "symboltable.h"
#include "grammarparser.h"
#include "error.h"

Symbol symbolTable[MAX_SYMBOL_TABLE_SIZE];
int tempKind;      // const = 1, var = 2, proc = 3
char tempName[12]; // name up to 11 chars
int tempVal;       // number (ASCII value)
int tempLevel;     // L level
int tempAddr;      // M address


void program(Token *tokenlist)
{
	Token * tempList = tokenList;
	block (&tempList);

	if (tempList->type != periodsym)
	{
		error(PERIOD_EXPECTED); // period missing error
	}

}

void block (Token **tempList)
{
	if ((*tempList)->type == constsym)
	{
		tempKind = 1; //1 for Const
		(*tempList) = (*tempList)->next;
		
		if ((*tempList)->type != identsym)
		{
			error(IDENT_MUST_FOLLOW_CONST_VAR_PROC); // identifier missing error
		}

		strcpy(tempName, (*tempList)->lexeme); //Store name of const
		(*tempList) = (*tempList)->next;

		if ((*tempList)->type != eqsym)
		{
			error (EQ_MUST_FOLLOW_IDENT); // equals must follow identifier error
		}

		(*tempList) = (*tempList)->next;

		if ((*tempList)->type != numbersym)
		{
			error (NUM_MUST_FOLLOW_EQ); // equals must be followed by number error
		}

		tempVal = atoi((*tempList)->lexeme); //Store value of const
		(*tempList) = (*tempList)->next;

		addConst(tempKind, tempName, tempVal); //Add const to symbol table

		while ((*tempList)->type == commasym)
		{
			tempKind = 1; //1 for Const
			(*tempList) = (*tempList)->next;

			if ((*tempList)->type != identsym)
			{
				error(IDENT_MUST_FOLLOW_CONST_VAR_PROC); // identifier missing error
			}

			strcpy(tempName, (*tempList)->lexeme); //Store name of const
			(*tempList) = (*tempList)->next;

			if ((*tempList)->type != eqsym)
			{
				error (EQ_MUST_FOLLOW_IDENT); // equals must follow identifier error
			}

			(*tempList) = (*tempList)->next;

			if ((*tempList)->type != numbersym)
			{
				error (NUM_MUST_FOLLOW_EQ); // equals must be followed by number error
			}

			tempVal = atoi((*tempList)->lexeme); //Store value of const
			(*tempList) = (*tempList)->next;

			addConst(tempKind, tempName, tempVal); //Add const to symbol table
		}
		if ((*tempList)->type != semicolonsym)
		{
			error(SEMICOLON_COMMA_MISSING); // missing semi colon error
		}
		(*tempList) = (*tempList)->next;
	}
	if ((*tempList)->type == varsym)
	{
		(*tempList) = (*tempList)->next;
		if ((*tempList)->type != identsym)
		{
			error(IDENT_MUST_FOLLOW_CONST_VAR_PROC); // identifier missing error
		}

		//(*tempList) = (*tempList)->next;  //SKIP VAR NAME
		(*tempList) = (*tempList)->next;

		while ((*tempList)->type == commasym)
		{
			(*tempList) = (*tempList)->next;

			if ((*tempList)->type != identsym)
			{
				error(IDENT_EXPECTED_IN_VAR); // identifier missing error
			}

			//(*tempList) = (*tempList)->next;  //SKIP VAR NAME
			(*tempList) = (*tempList)->next;
		}

		if ((*tempList)->type != semicolonsym)
		{
			error(SEMICOLON_COMMA_MISSING); // missing semi colon error
		}

		(*tempList) = (*tempList)->next;
	}
	while ((*tempList)->type == procsym)
	{
		(*tempList) = (*tempList)->next;

		if ((*tempList)->type != identsym)
		{
			error(IDENT_MUST_FOLLOW_CONST_VAR_PROC); // identifier missing error
		}

		(*tempList) = (*tempList)->next;

		if ((*tempList)->type != semicolonsym)
		{
			error(INCORRECT_SYM_AFTER_PROC_DECLARATION); // might change this
		}

		(*tempList) = (*tempList)->next;

		block(tempList);

		(*tempList) = (*tempList)->next;

		if ((*tempList)->type != semicolonsym)
		{
			error(INCORRECT_SYM_AFTER_STATEMENT_IN_BLOCK); // might change this
		}

	}

	statement (tempList);
}


void condition (Token **tempList)
{
	(*tempList) = (*tempList)->next;

	if ((*tempList)->type == oddsym)
	{
		(*tempList) = (*tempList)->next;
		expression(tempList);
	}
	else
	{
		expression(tempList);
		if (relation(tempList) == 0)
		{
			error (RELATIONAL_OP_EXPECTED); // missing relational op error
		}

		(*tempList) = (*tempList)->next;
		expression(tempList);
	}
}

void statement(Token **tempList){
	if((*tempList)->type == identsym){
		(*tempList) = (*tempList)->next;

		//printf("%d \n", (*tempList)->type); // testing

		if((*tempList)->type != becomessym)
		 	error(BECOMES_MUST_FOLLOW_IDENT_IN_STATEMENT);

		(*tempList) = (*tempList)->next;

		expression(tempList);
	}
	else if((*tempList)->type == callsym){
		(*tempList) = (*tempList)->next;

		if((*tempList)->type != identsym)
			error(IDENT_MUST_FOLLOW_CALL);

		(*tempList) = (*tempList)->next;
	}
	else if((*tempList)->type == beginsym){


		(*tempList) = (*tempList)->next;

		statement(tempList);

		while((*tempList)->type == semicolonsym){
			(*tempList) = (*tempList)->next;
			statement(tempList);
		}

		if((*tempList)->type != endsym)
			error(INCORRECT_SYM_FOLLOWING_STATEMENT); //Might need to change this

		(*tempList) = (*tempList)->next;
	}
	else if((*tempList)->type == ifsym){
		(*tempList) = (*tempList)->next;

		condition(tempList);

		if((*tempList)->type != thensym)
			error(THEN_EXPECTED);

		(*tempList) = (*tempList)->next;

		statement(tempList);

		if ((*tempList)->type == elsesym)
		{
			(*tempList) = (*tempList)->next;

			statement (tempList);
		}
	}
	else if((*tempList)->type == whilesym){
		(*tempList) = (*tempList)->next;

		condition(tempList);

		if((*tempList)->type != dosym)
			error(DO_EXPECTED);

		(*tempList) = (*tempList)->next;

		statement(tempList);
	}
	else if ((*tempList)->type == readsym)
	{
		(*tempList) = (*tempList)->next;

		if ((*tempList)->type != identsym)
			error(IDENT_MUST_FOLLOW_READ);

		(*tempList) = (*tempList)->next;
	}
	else if ((*tempList)->type == writesym)
	{
		(*tempList) = (*tempList)->next;

		if ((*tempList)->type != identsym)
			error(IDENT_MUST_FOLLOW_WRITE);

		(*tempList) = (*tempList)->next;
	}
}


void expression (Token **tempList)
{
	if ((*tempList)->type == plussym || (*tempList)->type == minussym)
	{
		(*tempList) = (*tempList)->next;
	}

	term (tempList);

	while ((*tempList)->type == plussym || (*tempList)->type == minussym)
	{
		(*tempList) = (*tempList)->next;
		term (tempList);
	}

}

void term (Token **tempList)
{
	factor (tempList);

	while ((*tempList)->type == multsym || (*tempList)->type == slashsym)
	{
		(*tempList) = (*tempList)->next;
		factor (tempList);
	}

}

void factor(Token **tempList){

	if((*tempList)->type == identsym){
		(*tempList) = (*tempList)->next;
	}
	else if((*tempList)->type == numbersym){
		(*tempList) = (*tempList)->next;
		//(*tempList) = (*tempList)->next; 	//SKIP NUMBER AFTER NUMBERSYM
	}
	else if((*tempList)->type == lparentsym){
		(*tempList) = (*tempList)->next;

		expression(tempList);

		if((*tempList)->type != rparentsym)
			error(RIGHTPAREN_MISSING);

		(*tempList) = (*tempList)->next;
	}
	else{
		error(PRECEDING_FACTOR_CANNOT_BEGIN_WITH_SYM);
	}
}

int relation (Token **tempList)
{
	if ((*tempList)->type == eqsym)
		return 1;
	if ((*tempList)->type == neqsym)
		return 1;
	if ((*tempList)->type == lessym)
		return 1;
	if ((*tempList)->type == leqsym)
		return 1;
	if ((*tempList)->type == gtrsym)
		return 1;
	if ((*tempList)->type == geqsym)
		return 1;

	return 0;
}
