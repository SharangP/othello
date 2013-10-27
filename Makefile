CC = g++

debug: CC += -g
debug: othello

othello: othello.o board.o
	$(CC) -o othello othello.o board.o

othello.o: othello.cpp board.h
	$(CC) -c othello.cpp

board.o: board.cpp board.h macros.h
	$(CC) -c board.cpp

clean:
	rm *.o *.stackdump *~
