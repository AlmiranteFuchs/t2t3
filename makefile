# Makefile de exemplo (Manual do GNU Make)
     
CFLAGS = -Wall -std=c90 -g  # flags de compilacao
LDFLAGS = -lm

CC = gcc

# arquivos-objeto
	objects = mundo.o libconjunto.o
     
all: mundo.o libconjunto.o
	$(CC) -o mundo mundo.o libconjunto.o $(LDFLAGS)

libconjunto.o: libconjunto.c
	$(CC) -c $(CFLAGS) libconjunto.c

mundo.o: mundo.c
	$(CC) -c $(CFLAGS) mundo.c

clean:
	rm -f $(objects) mundo


