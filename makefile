all: phylib.so

phylib.o: phylib.c phylib.h
	gcc -std=c99 -Wall -pedantic -fpic -c phylib.c -o phylib.o

phylib.so: phylib.o
	gcc -std=c99 -Wall -pedantic -shared -o libphylib.so phylib.o

clean:
	rm -f phylib.o libphylib.so
