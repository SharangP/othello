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

const char iterateModes[4] = {1, 2, 3, 4};
const char iterateDirections[2] = {-1, 1};

Board::Square::Square(){}

Board::Square::Square(char y, char x){
    this->y = y;
    this->x = x;
}

Board::Move::Move(char player, char y, char x){
    this->player = player;
    this->square.y = y;
    this->square.x = x;
    this->valid = false;
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
    cout << "    0  1  2  3  4  5  6  7" << endl;
    cout << "   ------------------------" << endl;
    for(int i = 0; i < BOARDSIZE; i++){
        cout << (int)i << " |";
        for(int j = 0; j < BOARDSIZE; j++){
            cout << " " << (int)board[i][j] << " ";//TODO: add color
        }
        cout << "|" << endl;
    }
    cout << "   ------------------------" << endl;
}

bool Board::onBoard(const char y, const char x){
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

    if((direction != 1) && (direction != -1))
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

vector<Board::Move> Board::LegalMoves(const char player){
    vector<Board::Move> moves;

    for(int i = 0; i < BOARDSIZE; i++){
        for(int j = 0; j < BOARDSIZE; j++){
            if(board[i][j] == 0){

                Board::Move move(player, i, j);

                for(int n = 0; n < sizeof(iterateModes); n++){
                    char mode = iterateModes[n];
                    for(int m = 0; m < sizeof(iterateDirections); m++){
                        char direction = iterateDirections[m];
                        char y = move.square.y, x = move.square.x;
                        vector<Board::Square> trace;

                        iterate(y, x, mode, direction);

                        //not a valid direction unless opponent's piece is next
                        if((board[y][x] == player) || (board[y][x] == 0)){
                            continue;
                        }

                        for(y, x; onBoard(y, x); iterate(y, x, mode, direction)){
                            trace.push_back(Board::Square(y, x)); //keep track of potential flips
                            if(board[y][x] == player){
                                //mark move as valid and append trace to flips vector
                                move.valid = true;
                                move.flips.insert(move.flips.end(), trace.begin(), trace.end());                            }
                        }
                    }
                }
                if(move.valid)
                    moves.push_back(move);
            }
        }
    }

    return moves;
}

#endif
