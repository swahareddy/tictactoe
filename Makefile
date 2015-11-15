project: newmini.o
	cc newmini.o -o project
newmini.o: newmini.c new.h
	cc -Wall -c final.c
