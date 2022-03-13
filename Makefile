
testcmp: cmp
	bin/cmp.out example/ex.txt example/ex.bin

cmp: cls common slap
	gcc -g -c src/cmp.c -o bin/cmp.o
	gcc -g bin/cmp.o bin/byte.o bin/kernel.o bin/lang.o slap/bin/token.o slap/bin/gram.o -o bin/cmp.out

cls:
	clear

common:
	gcc -g -c src/byte.c -o bin/byte.o
	gcc -g -c src/kernel.c -o bin/kernel.o
	gcc -g -c src/lang.c -o bin/lang.o

slap:
	gcc -g -c slap/src/token.c -o slap/bin/token.o
	gcc -g -c slap/src/gram.c -o slap/bin/gram.o