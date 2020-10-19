newshell : src/main.c
	gcc src/main.c -o newshell -Wall

.PHONY : clean

clean :
	rm newshell
