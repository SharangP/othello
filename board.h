/*************************
 * BOARD.H
 * board class declaration
 ************************/
#ifndef _BOARD_H_
#define _BOARD_H_

#include <iostream>
#include <vector>
#include "const.h"

using namespace std;

class Board{

public:
    class Square{
    public:
        Square();
        Square(char y, char x);
        char x, y;
    };

    class Move{
    public:
        Move(char player, char y, char x);
        Board::Square square;
        char player;
        bool valid;
        vector<Square> flips;
    };

    Board();
    Board(char boardState[8][8], char currentPlayer);
    void Print();
    void NextPlayer();
    bool EndState();
    void ApplyMove(Board::Move move);
    vector<Board::Move> LegalMoves();

    char board[BOARDSIZE][BOARDSIZE];
    char currentPlayer;
    char numMovesMade;

private:
    bool onBoard(const char y, const char x);
    bool iterate(char &y, char &x, const char mode, const char direction);
};

#endif //_BOARD_H_
