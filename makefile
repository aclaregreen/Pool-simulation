CC=clang
CFLAGS=-Wall -std=c99 -pedantic

all: A1

clean:
	rm -f *.o *.so A1

libphylib.so: phylib.o
	$(CC) phylib.o -shared -o libphylib.so

phylib.o:  phylib.c phylib.h
	$(CC) $(CFLAGS) -c phylib.c -fPIC -o phylib.o

A1test1.o:  A1test1.c phylib.h
	$(CC) $(CFLAGS) -c A1test1.c -o A1test1.o

A1:  A1test1.o libphylib.so
	$(CC) A1test1.o -lm -L. -lphylib -o A1