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
        Move(int player, char y, char x);
        Board::Square square;
        int player;
        bool valid;
        vector<Square> flips;
    };

    Board();
    Board(char boardState[8][8], int currentPlayer);
    void Print(vector<Board::Move> moves = vector<Board::Move>());
    bool NextPlayer(bool currentPlayerPass);
    void ApplyMove(Board::Move move);
    vector<Board::Move> LegalMoves();
    void GameOver();
    
    int currentPlayer;

private:
    bool onBoard(const char y, const char x);
    bool iterate(char &y, char &x, const int mode, const int direction);
    char board[BOARDSIZE][BOARDSIZE];
    int score[3];
    bool playerPassed;
};

#endif //_BOARD_H_
