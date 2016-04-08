#include "scanner.h"
#include "tempListlist.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


void program(Token *tokenlist)
{
	Token * tempList = tokenList;
	block (&tempList);

	if (tempList->type != periodsym)
	{
		// period missing error
	}

}

void block (Token *tempList)
{
	if (tempList->type == constsym)
	{
		tempList = tempList->next;
		if (tempList->type != identsym)
		{
			// error
		}
		tempList = tempList->next;
		if (tempList->type != eqsym)
		{
			// error
		}
		tempList = tempList->next;
		if (tempList->type != numbersym)
		{
			//error
		}

		tempList = tempList->next;
		while (tempList->type == commasym)
		{
			tempList = tempList->next;
			if (tempList->type != identsym)
			{
				// error
			}
			tempList = tempList->next;
			if (tempList->type != eqsym)
			{
				// error
			}
			tempList = tempList->next;
			if (tempList->type != numbersym)
			{
				//error
			}
			tempList = tempList->next;
		}
		if (tempList->type != semicolonsym)
		{
			// missing semi colon error
		}
		tempList = tempList->next;
	}
	if (tempList->type = varsym)
	{
		tempList = tempList->next;

		if (tempList->type != identsym)
		{
			// error
		}

		tempList = tempList->next;

		while (tempList->type == commasym)
		{
			tempList = tempList->next;

			if (tempList->type != identsym)
			{
				// error
			}

			tempList = tempList->next;
		}

		if (tempList->type != semicolonsym)
		{
			// missing semi colon error
		}

		tempList = tempList->next;
	}
	while (tempList->type == procsym)
	{
		tempList = tempList->next;

		if (tempList->type != identsym)
		{
			// error
		}

		tempList = tempList->next;

		if (tempList->type != semicolonsym)
		{
			// error missing semi colon
		}

		tempList = tempList->next;

		block(&tempList);

		tempList = tempList->next;

		if (tempList->type != semicolonsym)
		{
			// error missing semi colon
		}

	}

	statement (&tempList);
}


<<<<<<< Updated upstream
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
			// error
		}

		tempList = tempList->next;
		expression(&tempList);
	}
}

=======
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
