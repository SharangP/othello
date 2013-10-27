/*************************
 * board class
 ************************/
#ifndef _BOARD_H_
#define _BOARD_H_

#include <iostream>
#include <vector>

#define BOARDSIZE 8

class Board{
public:
    Board();
    void Print();
    char board[BOARDSIZE][BOARDSIZE];
};

#endif //_BOARD_H_
