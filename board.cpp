/*************************
 * board class
 ************************/
#ifndef _BOARD_CPP_
#define _BOARD_CPP_

#include <iostream>
#include <vector>
#include "board.h"

#define BOARDSIZE 8

using namespace std;


Board::Board(){
    for(int i = 0; i < BOARDSIZE; i++){
        for(int j = 0; j < BOARDSIZE; j++){
            board[i][j] = 0;
        }
    }
}

void Board::Print(){
    cout << "hello" << endl;
}

#endif
