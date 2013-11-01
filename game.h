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
    void Start();

private:
    bool randomMove();
    bool humanMove();

    Board board;
    bool humanPlayer[2];
    int timeLimit;
};

#endif //_GAME_H_
