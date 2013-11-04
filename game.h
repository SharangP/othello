/*************************
 * GAME.H
 * game class declaration
 ************************/
#ifndef _GAME_H_
#define _GAME_H_

#include <time.h>
#include "board.h"
#include "const.h"

using namespace std;


class Game{
public:
    Game();
    void Setup(int gameType);
    void Play();

private:
    int heuristic(Board board);
    int alphabeta(Board board, int depth, int alpha, int beta, bool maxPlayer);
    bool smartMove();
    bool humanMove();
    bool randomMove();

    Board board;
    bool humanPlayer[3];
    int timeLimit;
    clock_t startTime;
    bool timeout;
};

#endif //_GAME_H_
