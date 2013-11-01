CC = g++ #-O3

debug: CC += -g
debug: othello

othello: othello.o board.o game.o
	$(CC) -o othello othello.o board.o game.o

othello.o: othello.cpp game.h board.h
	$(CC) -c othello.cpp

game.o: game.cpp game.h board.h const.h
	$(CC) -c game.cpp

board.o: board.cpp board.h const.h
	$(CC) -c board.cpp

clean:
	rm *.o *.stackdump *~
