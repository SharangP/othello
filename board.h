/*************************
 * BOARD.H
 * board class declaration
 ************************/
#ifndef _BOARD_H_
#define _BOARD_H_

#include <iostream>
#include <vector>
#include "macros.h"

using namespace std;

class Board{
public:
    class Move{
        Move(char player, char y, char x);
        char player, x, y;
    };
    Board();
    void Print();
    vector<Board::Move> LegalMoves(char player);
    char board[BOARDSIZE][BOARDSIZE];
private:
    bool onBoard(char y, char x);
    bool iterate(char &y, char &x, const char mode, const char direction);
};

#endif //_BOARD_H_
