build:
	cc -Wall -std=c99 program.c -lncurses -o program

run:
	./program

clean:
	rm program
