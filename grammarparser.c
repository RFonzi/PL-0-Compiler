#include "Tokenlist.h"
#include "symboltable.h"




void program(Token *tokenlist)
{
	Token * tempList = tokenList;
	block (&tempList);

	if (tempList->type != periodsym)
	{
		error(PERIOD_EXPECTED); // period missing error
	}

}

void block (Token *tempList)
{
	if (tempList->type == constsym)
	{
		tempList = tempList->next;
		if (tempList->type != identsym)
		{
			error(IDENT_MUST_FOLLOW_CONST_VAR_PROC); // identifier missing error
		}
		tempList = tempList->next;
		if (tempList->type != eqsym)
		{
			error (EQ_MUST_FOLLOW_IDENT); // equals must follow identifier error
		}
		tempList = tempList->next;
		if (tempList->type != numbersym)
		{
			error (NUM_MUST_FOLLOW_EQ); // equals must be followed by number error
		}

		tempList = tempList->next;
		while (tempList->type == commasym)
		{
			tempList = tempList->next;
			if (tempList->type != identsym)
			{
				error(IDENT_MUST_FOLLOW_CONST_VAR_PROC); // identifier missing error
			}
			tempList = tempList->next;
			if (tempList->type != eqsym)
			{
				error (EQ_MUST_FOLLOW_IDENT); // equals must follow identifier error
			}
			tempList = tempList->next;
			if (tempList->type != numbersym)
			{
				error (NUM_MUST_FOLLOW_EQ); // equals must be followed by number error
			}
			tempList = tempList->next;
		}
		if (tempList->type != semicolonsym)
		{
			error(SEMICOLON_COMMA_MISSING); // missing semi colon error
		}
		tempList = tempList->next;
	}
	if (tempList->type = varsym)
	{
		tempList = tempList->next;

		if (tempList->type != identsym)
		{
			error(IDENT_MUST_FOLLOW_CONST_VAR_PROC); // identifier missing error
		}

		tempList = tempList->next;

		while (tempList->type == commasym)
		{
			tempList = tempList->next;

			if (tempList->type != identsym)
			{
				error(IDENT_MUST_FOLLOW_CONST_VAR_PROC); // identifier missing error
			}

			tempList = tempList->next;
		}

		if (tempList->type != semicolonsym)
		{
			error(SEMICOLON_COMMA_MISSING); // missing semi colon error
		}

		tempList = tempList->next;
	}
	while (tempList->type == procsym)
	{
		tempList = tempList->next;

		if (tempList->type != identsym)
		{
			error(IDENT_MUST_FOLLOW_CONST_VAR_PROC); // identifier missing error
		}

		tempList = tempList->next;

		if (tempList->type != semicolonsym)
		{
			error(SEMICOLON_COMMA_MISSING); // missing semi colon error
		}

		tempList = tempList->next;

		block(&tempList);

		tempList = tempList->next;

		if (tempList->type != semicolonsym)
		{
			error(SEMICOLON_COMMA_MISSING); // missing semi colon error
		}

	}

	statement (&tempList);
}


void condition (Token *tempList)
{
	tempList = tempList->next;

	if (tempList->type == oddsym)
	{
		tempList = tempList->next;
		expression(&tempList);
	}
	else
	{
		expression(&tempList);
		if (relation(&tempList) == 0)
		{
			error (RELATIONAL_OP_EXPECTED); // missing relational op error
		}

		tempList = tempList->next;
		expression(&tempList);
	}
}

void statement(Token *tempList){
	if(tempList->type == identsym){
		tempList = tempList->next;

		if(tempList->type != becomessym) //EQ instead of becomes??

		tempList = tempList->next;

		expression(tempList);
	}
	else if(tempList->type == callsym){
		tempList = tempList->next;

		if(tempList->type != identsym)
			error(IDENT_MUST_FOLLOW_CALL);

		tempList = tempList->next;
	}
	else if(tempList->type == beginsym){
		tempList = tempList->next;

		statement(tempList);

		while(tempList->type == semicolonsym){
			tempList = tempList->next;
			statement(tempList);
		}

		if(tempList->type != endsym)
			error(INCORRECT_SYM_FOLLOWING_STATEMENT) //Might need to change this

		tempList = tempList->next; //MAY BE THE PERIOD AFTER endsym
	}
	else if(tempList->type == ifsym){
		tempList = tempList->next;

		condition(tempList);

		if(tempList->type != thensym)
			error(THEN_EXPECTED);

		tempList = tempList->next;

		statement(tempList);
	}
	else if(tempList->type == whilesym){
		tempList = tempList->next;

		condition(tempList);

		if(tempList != dosym)
			error(DO_EXPECTED);

		tempList = tempList->next;

		statement(tempList);
	}
}


void expression (Token *tempList)
{
	if (tempList->type == plussym || tempList->type == minussym)
	{
		tempList = tempList->next;
	}

	term (&tempList);

	while (tempList->type == plussym || tempList->type == minussym)
	{
		tempList = tempList->next;
		term (&tempList);
	}

}

void term (Token *tempList)
{
	factor (&tempList);

	while (tempList->type == multsym || tempList->type == slashsym)
	{
		tempList = tempList->next;
		factor (&tempList);
	}

}

void factor(Token *tempList){
	if(tempList->type == identsym){
		tempList = tempList->next;
	}
	else if(tempList->type == numbersym){
		tempList = tempList->next;
	}
	else if(tempList->type == lparentsym){
		tempList = tempList->next;

		expression(tempList);

		if(tempList->type != rparentsym)
			error(RIGHTPAREN_MISSING);

		tempList = tempList->next;
	}
	else{
		error(PRECEDING_FACTOR_CANNOT_BEGIN_WITH_SYM);
	}
}

int relation (Token *tempList)
{
	if (tempList->type == eqsym)
		return 1;
	if (tempList->type == neqsym)
		return 1;
	if (tempList->type == lessym)
		return 1;
	if (tempList->type == leqsym)
		return 1;
	if (tempList->type == gtrsym)
		return 1;
	if (tempList->type == geqsym)
		return 1;

	return 0;
}
