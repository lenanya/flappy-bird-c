all: main

main: main.c
	clang main.c -o main -lraylib -lm
