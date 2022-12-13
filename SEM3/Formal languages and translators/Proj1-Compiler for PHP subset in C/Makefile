#Makefile pre: Implemetácia prekladaču imperatívneho jazyka IFJ22
EXECUTABLE = IFJ22Compiler
CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 -g
.PHONY: clean

all: $(EXECUTABLE)

$(EXECUTABLE): Syntax_analysis.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm $(EXECUTABLE)