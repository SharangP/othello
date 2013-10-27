/*************************
 * BOARD.CPP
 * board class implementation
 ************************/
#ifndef _BOARD_CPP_
#define _BOARD_CPP_

#include <stdio.h>
#include <iostream>
#include <vector>
#include "macros.h"
#include "board.h"


using namespace std;

Board::Move::Move(char player, char y, char x){
    this->player = player;
    this->y = y;
    this->x = x;
}

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

bool Board::onBoard(char y, char x){
    return (x >= 0) && (x < BOARDSIZE) && (y >= 0) && (y < BOARDSIZE);
}

//modes
//  1 (horizontal)
//  2 (vertical)
//  3 (right diagonal)
//  4 (left diagonal)
//
//directions
//  +1
//  -1
bool Board::iterate(char &y, char &x, const char mode, const char direction){
    if((direction != 1) || (direction != -1))
        return false;

    switch(mode){
        case(1):
            x += direction;
            return true;
        case(2):
            y += direction;
            return true;
        case(3):
            y += direction;
            x += direction;
            return true;
        case(4):
            y += direction;
            x -= direction;
            return true;
        default:
            return false;
    }
}

vector<Board::Move> Board::LegalMoves(char player){
    vector<Board::Move> moves;
    for(int i = 0; i < BOARDSIZE; i++){
        for(int j = 0; j < BOARDSIZE; j++){
            if(board[i][j] == 0){
                bool valid = false;
                //find next player piece in row/col/diag
                //if opponent in between then legal (what about spaces)
                //
                //while opponents pieces, ++, if own piece found, valid 
                //
                //check horizontal
                for(int n = j; (n < BOARDSIZE) && (n != 0); n++){
                    
                }
                //check vertical
                //check right diag
                //check left diag
            };
        }
    }

    return moves;
}

#endif
