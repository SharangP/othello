/*************************
 * BOARD.CPP
 * board class implementation
 ************************/
#ifndef _BOARD_CPP_
#define _BOARD_CPP_

#include <stdio.h>
#include <iostream>
#include <vector>
#include "const.h"
#include "board.h"

using namespace std;


const int iterateModes[4] = {1, 2, 3, 4};
const int iterateDirections[2] = {-1, 1};

//Board::Square::Square()
//  empty Square constructor
Board::Square::Square(){}


//Board::Square::Square(char y, char x)
//  Square constructor to initialize coordinates
Board::Square::Square(char y, char x){
    this->y = y;
    this->x = x;
}


//Board::Move::Move(int player, char y, char x)
//  Move constructor
Board::Move::Move(int player, char y, char x){
    this->player = player;
    this->square.y = y;
    this->square.x = x;
    this->valid = false;
}


//Board::Board()
//  constructor to initialize new game board
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

    currentPlayer = BLACK;
    playerPassed = false;
    score[BLACK] = 2;
    score[WHITE] = 2;
}


//Board::Board(char boardState[8][8], int currentPlayer)
//  constructor to initialize board with boardState and currentPlayer
Board::Board(char boardState[8][8], int currentPlayer){
    for(int i = 0; i < BOARDSIZE; i++){
        for(int j = 0; j < BOARDSIZE; j++){
            board[i][j] = boardState[i][j];
            if(boardState[i][j] == WHITE)
                score[WHITE]++;
            else if(boardState[i][j] == BLACK)
                score[BLACK]++;
        }
    }
    this->currentPlayer = currentPlayer;
    playerPassed = false;
}


//bool Board::onBoard(const char y, const char x)
//  private method to check whether coordinates are on the board
bool Board::onBoard(const char y, const char x){
    return (x >= 0) && (x < BOARDSIZE) && (y >= 0) && (y < BOARDSIZE);
}


//bool Board::iterate(char &y, char &x, const int mode, const int direction)
//  private method to iterate through the board in one direction
//  modes
//    1 (horizontal)
//    2 (vertical)
//    3 (right diagonal)
//    4 (left diagonal)
//  
//  directions
//    +1
//    -1
bool Board::iterate(char &y, char &x, const int mode, const int direction){

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


//void Board::Print()
//  board print method
void Board::Print(vector<Board::Move> moves){
    cout << "    0  1  2  3  4  5  6  7" << endl;
    cout << "   ------------------------" << endl;
    for(int i = 0; i < BOARDSIZE; i++){
        cout << (int)i << " |";
        for(int j = 0; j < BOARDSIZE; j++){
            bool potentialMove = false;
            cout << " ";
            for(int k = 0; k < moves.size(); k++){
                if(moves[k].square.y == i && moves[k].square.x == j){
                    cout << YELLOW << k << RESET << " ";
                    potentialMove = true;
                }
            }
            if(!potentialMove){
                if(board[i][j] == WHITE)
                    cout << RED;
                else if(board[i][j] == BLACK)
                    cout << BLUE;
                cout << (int)board[i][j] << RESET << " ";
            }
        }
        cout << "|" << endl;
    }
    cout << "   ------------------------" << endl;
}


//bool Board::NextPlayer()
//  method to move to the next player,
//  checking if the game is in an end state
bool Board::NextPlayer(bool currentPlayerPass){
    //if both players pass or the board is full, game over
    if((playerPassed && currentPlayerPass) || (score[BLACK]+score[WHITE] == NUMSQUARES))
        return true;

    currentPlayer = (currentPlayer == WHITE)
        ? BLACK
        : WHITE;
    playerPassed = false;
    return false;
}


//void Board::ApplyMove(Board::Move move)
//  method to apply a move to the board,
//  flipping the appropriate tiles
void Board::ApplyMove(Board::Move move){

    cout << "in applyMove" << endl;

    cout << "move being applied: p: " << move.player << " y: " << move.square.y << " x: " << move.square.x << endl;

    board[move.square.y][move.square.x] = move.player;
    for(int i = 0; i < move.flips.size(); i++){
        board[move.flips[i].y][move.flips[i].x] = move.player;
        if(move.player == WHITE){
            score[WHITE]++;
            score[BLACK]--;
        }
        else{
            score[BLACK]++;
            score[WHITE]--;
        }
    }
}

//vector<Board::Move> Board::LegalMoves()
//  method to find the legal moves for the current player
vector<Board::Move> Board::LegalMoves(){
    vector<Board::Move> moves;

    for(int i = 0; i < BOARDSIZE; i++){
        for(int j = 0; j < BOARDSIZE; j++){
            if(board[i][j] == 0){

                Board::Move move(currentPlayer, i, j);

                for(int n = 0; n < sizeof(iterateModes); n++){
                    int mode = iterateModes[n];
                    for(int m = 0; m < sizeof(iterateDirections); m++){
                        int direction = iterateDirections[m];
                        char y = move.square.y, x = move.square.x;
                        vector<Board::Square> trace;

                        iterate(y, x, mode, direction);

                        //not a valid direction unless opponent's piece is next
                        if((board[y][x] == currentPlayer) || (board[y][x] == 0)){
                            continue;
                        }

                        for(y, x; onBoard(y, x); iterate(y, x, mode, direction)){
                            trace.push_back(Board::Square(y, x)); //keep track of potential flips
                            if(board[y][x] == currentPlayer){
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

#endif //_BOARD_CPP_
