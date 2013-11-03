/*************************
 * GAME.H
 * game class declaration
 ************************/
#ifndef _GAME_H_
#define _GAME_H_

#include <iostream>
#include <vector>
#include "board.h"
#include "const.h"

using namespace std;


class Game{
public:
    Game();
    void Setup(int gameType);
    void Play();

private:
    int heuristic(Board board, bool maxPlayer);
    int alphabeta(Board board, int depth, int alpha, int beta, bool maxPlayer);
    bool smartMove();
    bool humanMove();
    bool randomMove();

    Board board;
    bool humanPlayer[3];
    int timeLimit;
};

#endif //_GAME_H_
