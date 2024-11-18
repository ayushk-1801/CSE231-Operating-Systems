all: date cal uptime main

date: date.c
	gcc -o date date.c -Wall

cal: cal.c
	gcc -o cal cal.c -Wall

uptime: uptime.c
	gcc -o uptime uptime.c -Wall

main: main.c
	gcc -o main main.c -Wall

clean:
	rm -f date cal uptime main
