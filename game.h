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

    class Player{
    public:
        Player();
        Player(bool isHuman, int color);
        bool isHuman;
        int color;
    };

    Game(int gameType);
    void Start();

private:
    void randomMove();
    Board board;
    Game::Player p1;
    Game::Player p2;
    int timeLimit;
};

#endif //_GAME_H_
