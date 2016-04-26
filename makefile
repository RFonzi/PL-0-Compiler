CC = gcc

PROG = pl0c.exe
SRCS = main.c scanner.c tokenlist.c cpu.c stack.c symboltable.c grammarparser.c error.c

OBJS = $(SRCS:.c=.o)

$(PROG) : $(OBJS)
	$(CC) -Wall -g $(OBJS) -o $(PROG)

scanner.o : scanner.c scanner.h tokenlist.h
	$(CC) -Wall -g -c scanner.c

tokenlist.o : tokenlist.c tokenlist.h
	$(CC) -Wall -g -c tokenlist.c

cpu.o : cpu.c cpu.h stack.h
	$(CC) -Wall -g -c cpu.c

stack.o : stack.c stack.h
	$(CC) -Wall -g -c stack.c

symboltable.o : symboltable.c symboltable.h
	$(CC) -Wall -g -c symboltable.c

grammarparser.o : grammarparser.c scanner.h symboltable.h grammarparser.h error.h cpu.h
	$(CC) -Wall -g -c grammarparser.c

error.o : error.c error.h
	$(CC) -Wall -g -c error.c

main.o : main.c scanner.h grammarparser.h symboltable.h
	$(CC) -Wall -g -c main.c

clean :
	rm $(PROG) $(OBJS)
