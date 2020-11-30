newshell : main.o command.o mode.o parse.o statement.o string_util.o built_in_cmd.o cd.o path.o myhistory.o alias.o
	gcc -o newshell main.o command.o mode.o parse.o statement.o string_util.o built_in_cmd.o cd.o path.o myhistory.o alias.o

main.o : src/main.c
	gcc -c src/main.c -Wall

command.o : src/command.c
	gcc -c src/command.c -Wall

mode.o : src/mode.c
	gcc -c src/mode.c -Wall

parse.o : src/parse.c
	gcc -c src/parse.c -Wall

statement.o : src/statement.c
	gcc -c src/statement.c -Wall

string_util.o : src/string_util.c
	gcc -c src/string_util.c -Wall

built_in_cmd.o : src/built_in_cmd.c
	gcc -c src/built_in_cmd.c -Wall

cd.o : src/cd.c
	gcc -c src/cd.c -Wall

path.o : src/path.c
	gcc -c src/path.c -Wall

myhistory.o : src/myhistory.c
	gcc -c src/myhistory.c -Wall

alias.o : src/alias.c
	gcc -c src/alias.c -Wall

.PHONY : clean

clean :
	rm newshell main.o command.o mode.o parse.o statement.o string_util.o built_in_cmd.o cd.o path.o myhistory.o alias.o
