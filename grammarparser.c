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
int tempLevel = -1; // L level
int tempAddr = 0;  // M address
int codeCounter = 0; // Global code counter




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
	tempLevel++;
	int offset = 4;    // AR offset
	int jumpAddr = codeCounter;
	gen(JMP, 0, 0); // M is placeholder
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
		tempKind = 2; // 2 for var

		(*tempList) = (*tempList)->next;
		if ((*tempList)->type != identsym)
		{
			error(IDENT_MUST_FOLLOW_CONST_VAR_PROC); // identifier missing error
		}

		strcpy(tempName, (*tempList)->lexeme); //Store name of var

		//(*tempList) = (*tempList)->next;  //SKIP VAR NAME
		(*tempList) = (*tempList)->next;


		addVar(tempKind, tempName, offset, tempLevel, tempAddr);
		offset++;

		while ((*tempList)->type == commasym)
		{
			(*tempList) = (*tempList)->next;

			if ((*tempList)->type != identsym)
			{
				error(IDENT_EXPECTED_IN_VAR); // identifier missing error
			}

			strcpy(tempName, (*tempList)->lexeme); //Store name of var

			//(*tempList) = (*tempList)->next;  //SKIP VAR NAME
			(*tempList) = (*tempList)->next;

			addVar(tempKind, tempName, offset, tempLevel, tempAddr);
			offset++;
		}

		if ((*tempList)->type != semicolonsym)
		{
			error(SEMICOLON_COMMA_MISSING); // missing semi colon error
		}

		(*tempList) = (*tempList)->next;
	}
	while ((*tempList)->type == procsym)
	{
		tempKind = 3; // 3 for Procedure

		(*tempList) = (*tempList)->next;

		if ((*tempList)->type != identsym)
		{
			error(IDENT_MUST_FOLLOW_CONST_VAR_PROC); // identifier missing error
		}

		strcpy(tempName, (*tempList)->lexeme); //Store name of Procedure

		(*tempList) = (*tempList)->next;

		addProc(tempKind, tempName, tempLevel, jumpAddr);

		if ((*tempList)->type != semicolonsym)
		{
			error(INCORRECT_SYM_AFTER_PROC_DECLARATION); // might change this
		}

		(*tempList) = (*tempList)->next;


		block(tempList);



		if ((*tempList)->type != semicolonsym)
		{
			error(INCORRECT_SYM_AFTER_STATEMENT_IN_BLOCK); // might change this
		}

		(*tempList) = (*tempList)->next;

	}
	code[jumpAddr]->m = jumpAddr;
	gen(INC, 0, offset);

	statement (tempList);

	//gen(OPR, 0, 0); //Return
	tempLevel--;
}


void condition (Token **tempList)
{

	if ((*tempList)->type == oddsym)
	{	
		gen (OPR, 0, ODD);
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

		//Check to see if the identifier is actually a var
		int i, found = 0;
		for(i = 0; i < numSymbols; i++){
			if(strcmp((*tempList)->lexeme, symbolTable[i].name) == 0 &&
						symbolTable[i].level == tempLevel){
				if(symbolTable[i].kind != 2){
					error(ASSIGNMENT_TO_CONST_OR_PROC);
				}
				found = 1;
			}
		}

		//If the identifier isn't declared yet, throw this
		if(found == 0)
			error(UNDECLARED_IDENTIFIER);

		(*tempList) = (*tempList)->next;

		if((*tempList)->type != becomessym)
		 	error(BECOMES_MUST_FOLLOW_IDENT_IN_STATEMENT);

		(*tempList) = (*tempList)->next;

		expression(tempList);
	}
	else if((*tempList)->type == callsym){
		(*tempList) = (*tempList)->next;

		if((*tempList)->type != identsym)
			error(IDENT_MUST_FOLLOW_CALL);

		//Check to see if the identifier is actually a proc
		int i, found = 0;
		for(i = 0; i < numSymbols; i++){
			if(strcmp((*tempList)->lexeme, symbolTable[i].name) == 0 &&
						symbolTable[i].level == tempLevel){
				if(symbolTable[i].kind != 3){
					error(UNDECLARED_IDENTIFIER);
				}
				found = 1;
			}
		}

		if(found == 0)
			error(UNDECLARED_IDENTIFIER);

		gen(CAL, symbolTable[i].level, symbolTable.addr);

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
		if ((*tempList)->type == plussym)
		{
			gen(OPR, 0, ADD);
		}
		else if ((*tempList)->type == minussym)
		{
			gen(OPR, 0, SUB);
		}
		(*tempList) = (*tempList)->next;
	}

	term (tempList);

	while ((*tempList)->type == plussym || (*tempList)->type == minussym)
	{
		if ((*tempList)->type == plussym)
		{
			gen(OPR, 0, ADD);
		}
		else if ((*tempList)->type == minussym)
		{
			gen(OPR, 0, SUB);
		}
		(*tempList) = (*tempList)->next;
		term (tempList);
	}

}

void term (Token **tempList)
{
	factor (tempList);

	while ((*tempList)->type == multsym || (*tempList)->type == slashsym)
	{	
		if ((*tempList)->type == multsym)
		{
			gen(OPR, 0, MUL);
		}
		else if ((*tempList)->type == slashsym)
		{
			gen(OPR, 0, DIV);
		}
		(*tempList) = (*tempList)->next;
		factor (tempList);
	}

}

void factor(Token **tempList){

	if((*tempList)->type == identsym){
		(*tempList) = (*tempList)->next;
	}
	else if((*tempList)->type == numbersym){
		gen(LIT, 0, atoi ((*tempList)->lexeme));
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
	{	
		gen (OPR, 0, EQL);
		return 1;
	}
	if ((*tempList)->type == neqsym)
	{
		gen (OPR, 0, NEQ);
		return 1;
	}
	if ((*tempList)->type == lessym)
	{
		gen (OPR, 0, LSS);
		return 1;
	}
	if ((*tempList)->type == leqsym)
	{	
		gen (OPR, 0, LEQ);
		return 1;
	}
	if ((*tempList)->type == gtrsym)
	{	
		gen (OPR, 0, GTR);
		return 1;
	}
	if ((*tempList)->type == geqsym)
	{
		gen (OPR, 0, GEQ);
		return 1;
	}

	return 0;
}

void gen(int opr, int l, int m){
	code[codeCounter]->op = opr;
	code[codeCounter]->l = l;
	code[codeCounter]->m = m;

	codeCounter++;
}
