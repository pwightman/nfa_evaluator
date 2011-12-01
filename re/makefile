CC=g++-4.2
SOURCES=lex.yy.o reparse.cpp
EXECUTABLE=reparse
BISONFLAGS=-d
BISONDEBUG=--verbose --debug

all: reparse

$(EXECUTABLE): $(SOURCES)
	$(CC) $(SOURCES) -o $(EXECUTABLE)

lex.yy.c: reparse.l
	flex reparse.l

reparse.tab.c: reparse.y
	bison $(BISONDEBUG) $(BISONFLAGS) reparse.y -o reparse.cpp

lex.yy.o: lex.yy.c reparse.tab.c
	$(CC) -c lex.yy.c -o lex.yy.o

clean:
	/bin/rm -rf $(SOURCES) reparse.hpp lex.yy.c reparse.output $(EXECUTABLE)