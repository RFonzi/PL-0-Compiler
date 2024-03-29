#include <string.h>
#include <stdlib.h>
#include "scanner.h"
#include "symboltable.h"
#include "tokenlist.h"
#include "grammarparser.h"
#include "error.h"
#include "cpu.h"

Symbol symbolTable[MAX_SYMBOL_TABLE_SIZE];
int tempKind;      // const = 1, var = 2, proc = 3
char tempName[12]; // name up to 11 chars
int tempVal;       // number (ASCII value)
int tempLevel = -1; // L level
int tempAddr = 0;  // M address
int codeCounter = 0; // Global code counter
int parenCounter = 0; // Count the number of parens down we are
int numParams = 0; // number of parameters
int stackSize = 0;
int numStoNeeded = 0;

char opstack[500] = {0}; // Operator stack for converting infix to postfix
int numOps = 0; // Number of operators in the operator stack




void program(Token *tokenlist)
{
	Token * tempList = tokenList;
	block (&tempList);

	if (tempList->type != periodsym)
	{
		error(PERIOD_EXPECTED); // period missing error
	}

	gen(SIO3, 0, 3); // Halt
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


		addVar(tempKind, tempName, offset+numParams, tempLevel, tempAddr);
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
			
			addVar(tempKind, tempName, offset+numParams, tempLevel, tempAddr);
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

		addProc(tempKind, tempName, tempLevel, codeCounter);
		
		addVar(2, "return", 0, tempLevel+1, tempAddr);

		paramBlock(tempList);
		
		
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
	code[jumpAddr].m = codeCounter;
	
	gen(INC, 0, offset+numParams);
	
	statement (tempList);

	if(tempLevel != 0)
		gen(OPR, 0, 0); //Return

	tempLevel--;
	numParams = 0;
	
	if(tempLevel > 0)
		deleteTopSymbolLevel();
}

void paramBlock (Token **tempList)
{
	numParams= 0;
	
	if ((*tempList)->type != lparentsym)
	{
		error(PROC_MUST_HAVE_PARAMS); // proc needs params error
	}
	
	(*tempList) = (*tempList)->next;
	
	if ((*tempList)->type == identsym)
	{
		tempKind = 2; 
		
		strcpy(tempName, (*tempList)->lexeme); //Store name of var

		addVar(tempKind, tempName, 4+numParams, tempLevel+1, tempAddr);
		numParams++;
		
		(*tempList) = (*tempList)->next;
		
		
		while ((*tempList)->type == commasym)
		{
			(*tempList) = (*tempList)->next;
			if ((*tempList)->type != identsym)
			{
				error(IDENT_EXPECTED_AS_PARAM); // needs error
			}
			
			strcpy(tempName, (*tempList)->lexeme); //Store name of var

			addVar(tempKind, tempName, 4+numParams, tempLevel+1, tempAddr);
			numParams++;
			
			(*tempList) = (*tempList)->next;
		}
	}
	
	if ((*tempList)->type != rparentsym)
	{
		error(BAD_PROC_DECLARATION); // bad procedure declaration
	}
	
	(*tempList) = (*tempList)->next;
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

		int relationalOP = relation(tempList);
		if (relationalOP == 0)
		{
			error (RELATIONAL_OP_EXPECTED); // missing relational op error
		}

		(*tempList) = (*tempList)->next;
		expression(tempList);

		// Determine what op was used and gen accordingly
		switch(relationalOP){
			case 1:
				gen (OPR, 0, EQL);
				break;
			case 2:
				gen (OPR, 0, NEQ);
				break;
			case 3:
				gen (OPR, 0, LSS);
				break;
			case 4:
				gen (OPR, 0, LEQ);
				break;
			case 5:
				gen (OPR, 0, GTR);
				break;
			case 6:
				gen (OPR, 0, GEQ);
				break;
		}
	}
}

void statement(Token **tempList){
	
	//printf("The statement is: %s\n", (*tempList)->lexeme);
	if((*tempList)->type == identsym){
			
		//Check to see if the identifier is actually a var
		int i = 0, found = 0, location;
		for(i = numSymbols; i >= 0; i--){
			if(strcmp((*tempList)->lexeme, symbolTable[i].name) == 0 &&
				symbolTable[i].level <= tempLevel){
				if(symbolTable[i].kind != 2){
					error(ASSIGNMENT_TO_CONST_OR_PROC);
				}
				found = 1;
				location = i;
				break;
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

		gen(STO, tempLevel - symbolTable[location].level, symbolTable[location].val);

	}
	else if((*tempList)->type == callsym){
		(*tempList) = (*tempList)->next;

		if((*tempList)->type != identsym)
			error(IDENT_MUST_FOLLOW_CALL);

		//Check to see if the identifier is actually a proc
		int i, found = 0, location;
		for(i = 0; i < numSymbols; i++){
			if(strcmp((*tempList)->lexeme, symbolTable[i].name) == 0){
				if(symbolTable[i].kind != 3){
					error(UNDECLARED_IDENTIFIER);
				}
				found = 1;
				location = i;
			}
		}

		if(found == 0)
			error(UNDECLARED_IDENTIFIER);

			
		paramList(tempList);
		/*
		int stackCounter = 0;
		for(i = 0; i < numSymbols; i++){
			if(symbolTable[location].level +1 ==  symbolTable[i].level){
				if(symbolTable[i].kind == 2){
					if(strcmp(symbolTable[i].name, "return") != 0)
					{
						stackCounter++;
					}
				}
			}
		}
		
		int tempSize = stackSize;
		*/
		while (numStoNeeded>0)
		{
			gen(STO, 0, stackSize+3);
			numStoNeeded--;
		}
		
		
		gen(CAL, tempLevel - symbolTable[location].level, symbolTable[location].val);

		//(*tempList) = (*tempList)->next; //this was at the end but i don't think it's needed anymore
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
		
		int jumpFromThen = codeCounter;
		int jumpFromElse;
		
		gen(JPC, 0, 0); // Jump from if, M is placeholder

		if((*tempList)->type != thensym)
			error(THEN_EXPECTED);

		(*tempList) = (*tempList)->next;

		statement(tempList);

		jumpFromElse = codeCounter;
		gen(JMP, 0, 0); //Jump from else, M is placeholder

		code[jumpFromThen].m = codeCounter; //Set jpc to jump at this point if the conditional fails

		if ((*tempList)->type == elsesym)
		{
			(*tempList) = (*tempList)->next;

			statement (tempList);
		}

		code[jumpFromElse].m = codeCounter;
	}
	else if((*tempList)->type == whilesym){
		(*tempList) = (*tempList)->next;

		int conditionLine = codeCounter;
		int jumpLine;

		condition(tempList);

		jumpLine = codeCounter;

		gen(JPC, 0, 0); // Jump after conditional if it's false, M is placeholder

		if((*tempList)->type != dosym)
			error(DO_EXPECTED);

		(*tempList) = (*tempList)->next;

		statement(tempList);

		gen(JMP, 0, conditionLine); // Jump back up to recheck the conditional
		code[jumpLine].m = codeCounter; // If the conditional is false, jump after this
	}
	else if ((*tempList)->type == readsym)
	{
		(*tempList) = (*tempList)->next;

		if ((*tempList)->type != identsym)
			error(IDENT_MUST_FOLLOW_READ);

		// Check if the ident is actually a var
		int i, found = 0, location;
		for(i = numSymbols; i >= 0; i--){
			if(strcmp((*tempList)->lexeme, symbolTable[i].name) == 0 &&
						symbolTable[i].level <= tempLevel){
				if(symbolTable[i].kind != 2){
					error(ASSIGNMENT_TO_CONST_OR_PROC);
				}
				found = 1;
				location = i;
			}
		}

		if(found == 0){
			error(UNDECLARED_IDENTIFIER);
		}
		
		
		gen(SIO2, 0, 2);
		gen(STO, tempLevel - symbolTable[location].level, symbolTable[location].val);

		(*tempList) = (*tempList)->next;
	}
	else if ((*tempList)->type == writesym)
	{
		(*tempList) = (*tempList)->next;

		if ((*tempList)->type != identsym)
			error(IDENT_MUST_FOLLOW_WRITE);

		// Check if the ident is actually a var
		int i, found = 0, location;
		for(i = numSymbols; i >= 0; i--){
			if(strcmp((*tempList)->lexeme, symbolTable[i].name) == 0 &&
						symbolTable[i].level <= tempLevel){
				if(symbolTable[i].kind != 2){
					error(ASSIGNMENT_TO_CONST_OR_PROC);
				}
				found = 1;
				location = i;
			}
		}

		if(found == 0){
			error(UNDECLARED_IDENTIFIER);
		}

		gen(LOD, tempLevel - symbolTable[location].level, symbolTable[location].val);

		gen(SIO1, 0, 1);
	

		(*tempList) = (*tempList)->next;
	}
}

void paramList (Token ** tempList){
	
	numStoNeeded = 0;

	(*tempList) = (*tempList)->next;
	
	if ((*tempList)->type != lparentsym)
	{
		error(MISSING_PARAM_LIST);
	}
	
	(*tempList) = (*tempList)->next;
	
	if ((*tempList)->type != rparentsym)
	{
		expression(tempList);
		numStoNeeded++;
		// generate code to store expression's result into first parameter slot
	}
	while ((*tempList)->type == commasym)
	{
		(*tempList) = (*tempList)->next;
		
		expression(tempList);
		numStoNeeded++;
		// generate code to store expression's result into next parameter slot
	}
	
	if ((*tempList)->type != rparentsym)
	{
		error(BAD_CALL_FORMATTING);// need error "Bad call formatting"
	}
	
	(*tempList) = (*tempList)->next;
	
}

void expression (Token **tempList)
{
	// Convert expression to infix notation
	term (tempList);

	while ((*tempList)->type == plussym || (*tempList)->type == minussym)
	{

		// If a lower precidence op is about to enter the opstack, check for higher precident ops and push them
		if(numOps > 0){
			while(opstack[numOps - 1] == '*' || opstack[numOps - 1] == '/'){
				numOps--;

				if(opstack[numOps] == '*')
					gen(OPR, 0, MUL);
				if(opstack[numOps] == '/')
					gen(OPR, 0, DIV);

				opstack[numOps] = 0;

				// Quick and dirty check for array bounds
				if(numOps == 0)
					break;
			}
		}

		// If the previous op was the same precidence, then pop it first.
		if(numOps > 0){
			if(opstack[numOps - 1] == '+'){
				gen(OPR, 0, ADD);
				opstack[numOps - 1] = 0;
				numOps--;
			}
			else if(opstack[numOps - 1] == '-'){
				gen(OPR, 0, SUB);
				opstack[numOps - 1] = 0;
				numOps--;
			}
		}
		
		if ((*tempList)->type == plussym)
		{
			// Push plus to the opstack
			opstack[numOps] = '+';
			numOps++;
		}
		else if ((*tempList)->type == minussym)
		{
			// Push minus to the opstack
			opstack[numOps] = '-';
			numOps++;
		}
		(*tempList) = (*tempList)->next;
		term (tempList);
	}

	//Pop the rest of the ops
	//printf("# of ops this round is %d\n", numOps);
	if(parenCounter == 0){ // Only do this when we aren't in a paren expression
		while(numOps > 0){
			numOps--;
		
			if(opstack[numOps] == '*')
				gen(OPR, 0, MUL);
			if(opstack[numOps] == '/')
				gen(OPR, 0, DIV);
			if(opstack[numOps] == '+')
				gen(OPR, 0, ADD);
			if(opstack[numOps] == '-')
				gen(OPR, 0, SUB);


			opstack[numOps] = 0;
		}
	}

}

void term (Token **tempList)
{
	factor (tempList);

	while ((*tempList)->type == multsym || (*tempList)->type == slashsym)
	{	

		// If the previous op was the same precidence, then pop it first.
		if(numOps > 0){
			if(opstack[numOps - 1] == '*'){
				gen(OPR, 0, MUL);
				opstack[numOps - 1] = 0;
				numOps--;
			}
			else if(opstack[numOps - 1] == '/'){
				gen(OPR, 0, DIV);
				opstack[numOps - 1] = 0;
				numOps--;
			}
		}

		if ((*tempList)->type == multsym)
		{
			// Push mult to the opstack
			opstack[numOps] = '*';
			numOps++;
		}
		else if ((*tempList)->type == slashsym)
		{
			// Push div to the opstack
			opstack[numOps] = '/';
			numOps++;
		}
		(*tempList) = (*tempList)->next;
		factor (tempList);
	}

}

void factor(Token **tempList){

	int isSignNeg = 0;

	// Check for sign
	if ((*tempList)->type == plussym || (*tempList)->type == minussym)
	{

		if ((*tempList)->type == plussym)
		{
			// Sign is positive
			isSignNeg = 0;
		}
		else if ((*tempList)->type == minussym)
		{
			// Sign is negative
			isSignNeg = 1;
		}
		(*tempList) = (*tempList)->next;
	}

	if((*tempList)->type == identsym){

		// Check if the ident is actually a var
		int i, found = 0, location;
		for(i = numSymbols; i >= 0; i--){
			if(strcmp((*tempList)->lexeme, symbolTable[i].name) == 0 &&
						symbolTable[i].level <= tempLevel){
				if(symbolTable[i].kind == 3){
					error(ASSIGNMENT_TO_CONST_OR_PROC); // not the right error potentially
				}
				found = 1;
				location = i;
				break;
			}
		}

		if(found == 0)
			error(UNDECLARED_IDENTIFIER);
		
		if (symbolTable[location].kind == 2)
			gen(LOD, tempLevel - symbolTable[location].level, symbolTable[location].val);
		
		if (symbolTable[location].kind == 1)
		{
			gen(LIT, 0, symbolTable[location].val);
		}
		if(isSignNeg)
			gen(OPR, 0, NEG);

		(*tempList) = (*tempList)->next;
	}
	else if((*tempList)->type == numbersym){
		gen(LIT, 0, atoi ((*tempList)->lexeme));

		if(isSignNeg)
			gen(OPR, 0, NEG);

		(*tempList) = (*tempList)->next;
		//(*tempList) = (*tempList)->next; 	//SKIP NUMBER AFTER NUMBERSYM
	}
	else if((*tempList)->type == lparentsym){

		// Push lparen to the opstack
		opstack[numOps] = '(';
		numOps++;
		parenCounter++; // Make sure we don't pop anything too early by using counters

		(*tempList) = (*tempList)->next;

		expression(tempList);

		if((*tempList)->type != rparentsym)
			error(RIGHTPAREN_MISSING);

		parenCounter--;

		// Pop every op from the opstack until a lparen is found
		while(opstack[numOps] != '('){

			if(opstack[numOps] == '*')
			{
				gen(OPR, 0, MUL);
				
			}
			if(opstack[numOps] == '/')
			{
				gen(OPR, 0, DIV);
				
			}
			if(opstack[numOps] == '+')
			{
				gen(OPR, 0, ADD);
				
			}
			if(opstack[numOps] == '-')
			{
				gen(OPR, 0, SUB);
				
			}

			opstack[numOps] = 0;

			numOps--;

		}

		// Clear the lparen
		opstack[numOps] = 0;

		(*tempList) = (*tempList)->next;
	}
	
	else if((*tempList)->type == callsym){
		(*tempList) = (*tempList)->next;

		if((*tempList)->type != identsym)
			error(IDENT_MUST_FOLLOW_CALL);

		//Check to see if the identifier is actually a proc
		int i, found = 0, location;
		for(i = 0; i < numSymbols; i++){
			if(strcmp((*tempList)->lexeme, symbolTable[i].name) == 0){
				if(symbolTable[i].kind != 3){
					error(UNDECLARED_IDENTIFIER);
				}
				found = 1;
				location = i;
			}
		}

		if(found == 0)
			error(UNDECLARED_IDENTIFIER);
	
		paramList(tempList);
		
		
		/*
		int stackCounter = 0;
		for(i = 0; i < numSymbols; i++){
			if(symbolTable[location].level +1 ==  symbolTable[i].level){
				if(symbolTable[i].kind == 2){
					if(strcmp(symbolTable[i].name, "return") != 0)
					{
						stackCounter++;
					}
				}
			}
		}
		
		int tempSize = stackSize;
		*/
		while (numStoNeeded>0)
		{
			gen(STO, 0, stackSize+3);
			numStoNeeded--;
		}
		
		gen(CAL, tempLevel - symbolTable[location].level, symbolTable[location].val);
		gen(INC, 0, 1);

		// (*tempList) = (*tempList)->next; //this was at the end but i don't think it's needed anymore
	}
	else{
		error(PRECEDING_FACTOR_CANNOT_BEGIN_WITH_SYM);
	}
}

int relation (Token **tempList)
{
	if ((*tempList)->type == eqsym)
	{	
		return 1;
	}
	if ((*tempList)->type == neqsym)
	{
		return 2;
	}
	if ((*tempList)->type == lessym)
	{
		return 3;
	}
	if ((*tempList)->type == leqsym)
	{	
		return 4;
	}
	if ((*tempList)->type == gtrsym)
	{	
		return 5;
	}
	if ((*tempList)->type == geqsym)
	{
		return 6;
	}

	return 0;
}

void gen(int opr, int l, int m){
	code[codeCounter].op = opr;
	code[codeCounter].l = l;
	code[codeCounter].m = m;
	
	updateStackSize(opr,l,m);
	
	codeCounter++;
}

void updateStackSize (int opr, int l, int m)
{
	if (opr == LIT || opr == LOD || opr == SIO2)
	{
		stackSize++;
	}
	else if (opr == STO || opr == JPC || opr == SIO1)
	{
		stackSize--;
	}
	else if (opr == INC)
	{
		stackSize+=m;
	}
	else if (opr == OPR)
	{
		if (m == RET)
		{
			stackSize = 0;
		}
		else if (m!= NEG && m!= ODD)
		{
			stackSize--;
		}
	}
}
