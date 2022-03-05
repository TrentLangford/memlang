

all: cls slap main
	gcc -g bin/main.o bin/gram.o bin/token.o -o bin/main.out
	bin/main.out

cls:
	clear

slap:
	gcc -g -c slap/src/gram.c -o bin/gram.o
	gcc -g -c slap/src/token.c -o bin/token.o

main:
	gcc -g -c src/main.c -o bin/main.o
	