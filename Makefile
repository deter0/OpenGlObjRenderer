all: compile run

CC = gcc
CFLAGS = -Wall -g -O2 -std=c99
GLIBS = -lGL -lm -lGLU -lglut
compile: # -02 for optimized: https://stackoverflow.com/questions/14919366/how-to-compile-library-on-c-using-gcc#14920973
	${CC} ${CFLAGS} ${GLIBS} src/main.c -o ./main.o

run:
	./main.o