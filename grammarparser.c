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






