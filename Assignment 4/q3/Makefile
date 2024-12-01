CC = gcc
CFLAGS = -Wall -g

all: setup Q3

setup: setup.c
	$(CC) $(CFLAGS) setup.c -o setup

Q3: Q3.c
	$(CC) $(CFLAGS) Q3.c -o Q3

run:
	./setup

clean:
	rm -f setup Q3 students.rec
