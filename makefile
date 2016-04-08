CC = gcc

PROG = pl0c.exe
SRCS = main.c scanner.c tokenlist.c cpu.c stack.c

OBJS = $(SRCS:.c=.o)

$(PROG) : $(OBJS)
	$(CC) -g $(OBJS) -o $(PROG)

scanner.o : scanner.c scanner.h tokenlist.h
	$(CC) -g -c scanner.c

tokenlist.o : tokenlist.c tokenlist.h
	$(CC) -g -c tokenlist.c

cpu.o : cpu.c cpu.h stack.h
	$(CC) -g -c cpu.c

stack.o : stack.c stack.h
	$(CC) -g -c stack.c

main.o : main.c scanner.h tokenlist.h cpu.h stack.h
	$(CC) -g -c main.c

clean :
	del $(PROG) $(OBJS)
