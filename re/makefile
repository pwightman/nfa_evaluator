CC=cc
EXECUTABLE=reparse

all: reparse

reparse: lex.yy.c y.tab.c
	$(CC) lex.yy.c y.tab.c -o $(EXECUTABLE)

lex.yy.c: reparse.y
	lex reparse.l

y.tab.c: reparse.y
	yacc -d reparse.y

clean:
	/bin/rm -rf lex.yy.c y.tab.* $(EXECUTABLE)