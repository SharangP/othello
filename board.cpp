/*************************
 * board class
 ************************/
#ifndef _BOARD_CPP_
#define _BOARD_CPP_

#include <stdio.h>
#include <iostream>
#include <vector>
#include "board.h"

#define BOARDSIZE 8
#define WHITE 1
#define BLACK 2

using namespace std;


Board::Board(){
    //initialize empty board
    for(int i = 0; i < BOARDSIZE; i++){
        for(int j = 0; j < BOARDSIZE; j++){
            board[i][j] = 0;
        }
    }

    //initialize starting pieces
    board[4][3] = WHITE;
    board[3][4] = WHITE;
    board[3][3] = BLACK;
    board[4][4] = BLACK;
}

void Board::Print(){
    cout << " ------------------------" << endl;
    for(int i = 0; i < BOARDSIZE; i++){
        cout << "|";
        for(int j = 0; j < BOARDSIZE; j++){
            printf(" %d ",board[i][j]); //TODO: add color
        }
        cout << "|" << endl;
    }
    cout << " ------------------------" << endl;
}

#endif
