#include "scanner.h"
#include "tokenlist.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


void program(Token *tokenlist)
{
	block (&Token);
	
	if (Token->type != periodsym)
	{
		// period missing error
	}
		
}

void block (Token *tokenlist)
{
	if (Token->type == constsym)
	{
		Token = Token->next;
		if (Token->type != identsym)
		{
			// error
		}
		Token = Token->next;
		if (Token->type != eqsym)
		{
			// error
		}
		Token = Token->next;
		if (Token->type != numbersym)
		{
			//error
		}
			
		Token = Token->next;
		while (Token->type == commasym)
		{
			Token = Token->next;
			if (Token->type != identsym)
			{
				// error
			}
			Token = Token->next;
			if (Token->type != eqsym)
			{
				// error
			}
			Token = Token->next;
			if (Token->type != numbersym)
			{
				//error
			}
			Token = Token->next;
		}
		if (Token->type != semicolonsym)
		{
			// missing semi colon error
		}
		Token = Token->next;
	}
	if (Token->type = varsym)
	{
		Token = Token->next;
		
		if (Token->type != identsym)
		{
			// error
		}
		
		Token = Token->next;
		
		while (Token->type == commasym)
		{
			Token = Token->next;
		
			if (Token->type != identsym)
			{
				// error
			}
		
			Token = Token->next;
		}
		
		if (Token->type != semicolonsym)
		{
			// missing semi colon error
		}
	}
	
}