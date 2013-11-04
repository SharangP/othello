/*************************
 * BOARD.H
 * board class declaration
 ************************/
#ifndef _BOARD_H_
#define _BOARD_H_

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
        Move();
        Move(char y, char x);
        Board::Square square;
        bool valid;
        vector<Square> flips;
    };

    Board();
    Board(Board &b);
    Board(char boardState[8][8], int currentPlayer);
    void Print(vector<Board::Move> moves = vector<Board::Move>(), bool computer = false);
    bool OnFrontier(int y, int x);
    bool TerminalState(bool currentPlayerPass);
    bool NextPlayer(bool currentPlayerPass);
    void ApplyMove(Board::Move move);
    vector<Board::Move> LegalMoves(int player);
    void GameOver();
    
    int currentPlayer;
    int score[3];
    char board[BOARDSIZE][BOARDSIZE];
    bool playerPassed;

private:
    bool onBoard(const char y, const char x);
    bool iterate(char &y, char &x, const int mode, const int direction);
};

#endif //_BOARD_H_
