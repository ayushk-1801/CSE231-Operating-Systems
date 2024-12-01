CC = gcc
CFLAGS = -Wall -g

all: setup q3

setup: setup.c
	$(CC) $(CFLAGS) setup.c -o setup

q3: q3.c
	$(CC) $(CFLAGS) q3.c -o q3

run:
	./setup

clean:
	rm -f setup q3 students.rec
