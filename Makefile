all: parser.o shell.o cd
	gcc -o shell parser.o shell.o

parser.o: src/parser.h src/parser.c
	gcc -c src/parser.c

shell.o: src/shell.c src/parser.h
	gcc -c src/shell.c

cd: src/cd.c
	gcc src/cd.c -o cd

clean: 
	rm shell *.o cd
