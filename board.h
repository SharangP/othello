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
    bool NextPlayer(bool currentPlayerPass);
    void ApplyMove(Board::Move move);
    vector<Board::Move> LegalMoves();
    
    char currentPlayer;

private:
    bool onBoard(const char y, const char x);
    bool iterate(char &y, char &x, const char mode, const char direction);
    char board[BOARDSIZE][BOARDSIZE];
    char blackScore;
    char whiteScore;
    bool playerPassed;
};

#endif //_BOARD_H_
