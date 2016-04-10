For McAlpin's COP3402 class, Spring 2016
Group: Ryan Fonzi & Laura Driscoll


Requirements: gcc

These instructions assume you're running on Eustis.

Compilation instructions:
cd into the same directory as the source files and simply enter 'make'.
The makefile should do everything for you.

Running instructions:
Enter './pl0c.exe'

Error Codes:
1: Use = instead of :=
2: = must be followed by a number
3: Identifier must be followed by =
4: 'const', 'var', 'procedure' must be followed by identifier
5: Semicolon or comma missing
6: Incorrect symbol after procedure declaration
7: Statement expected
8: Incorrect symbol after statement part in block
9: Period expected
10: Semicolon between statements missing
11: Undeclared identifier
12: Assignment to constant or procedure is not allowed
13: Assignment operator expected
14: 'call' must be followed by an identifier
15: Call of a constant or variable is meaningless
16: 'then' expected
17: Semicolon or '}' expected
18: 'do' expected
19: Incorrect symbol following statement
20: Relational operator expected
21: Expression must not contain a procedure identifier
22: Right parenthesis missing
23: The preceding factor cannot begin with this symbol
24: An expression cannot begin with this symbol
25: This number is too large
26: Identifier expected after 'var'
27: Identifier expected after 'read'
28: Identifier expected after 'write'
29: ':=' must follow identifier in statement

===========================W A R N I N G===========================
This program expects PL/0 code as input! Not a lexeme list!

In order for our parser to work properly, we needed to null terminate our
token list with the nullsym '1'.
AS A RESULT, 'lexemetable.txt' AND 'lexemelist.txt'ARE NULL TERMINATED
WITH A '1' AS WELL.
===================================================================


NOTE:
Input file MUST be named 'input.txt', which will contain your PL/0 program
Output files will be named 'cleaninput.txt', 'lexemetable.txt', 'lexemelist.txt', and 'symlist.txt'

The sample inputs are displayed in the accompanying pdf.
The sample outputs were generated via this program.
