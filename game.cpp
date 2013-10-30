/*************************
 * GAME.CPP
 * game class declaration
 ************************/
#ifndef _GAME_CPP_
#define _GAME_CPP_

#include <iostream>
#include <fstream>
#include <vector>
#include <ctype.h>
#include <stdlib.h>
#include "game.h"
#include "board.h"
#include "const.h"

using namespace std;


Game::Player::Player(){}

Game::Player::Player(bool isHuman, int color){
    if((color != WHITE) && (color != BLACK))
        throw 1;
    this->isHuman = isHuman;
    this->color = color;
}

//gameTypes
//  1: go first (black)
//  2. go second (white)
//  3: computer vs. computer
Game::Game(int gameType){
    char import;
    int t;

    switch(gameType){
        case(1):
            p1 = Game::Player(true, BLACK);
            p2 = Game::Player(false, WHITE);
            break;
        case(2):
            p1 = Game::Player(false, BLACK);
            p2 = Game::Player(true, WHITE);
            break;
        case(3):
            p1 = Game::Player(false, BLACK);
            p2 = Game::Player(false, WHITE);
            break;
        default:
            throw;
    }

    cout << "Import a board state? (Y/N) ";
    cin >> import;

    //allow user to import a board state
    if(tolower(import) == 'y'){
        char c, currentPlayer, boardState[8][8];
        int i = 0, j = 0;
        string fileName;
        fstream boardFile;

        cout << "Enter a file name: ";
        cin >> fileName;

        boardFile.open(fileName.c_str(), fstream::in);
        while((c = boardFile.get()) != EOF){
            //if c is an int save it into the array
            if((c == '0') || (c == '1') || (c == '2')){
                boardState[i][j++] = c-'0';
                if(j == BOARDSIZE){
                    j = 0;
                    if(++i == BOARDSIZE)
                        break;
                }
            }
        }
        boardFile >> currentPlayer;
        boardFile >> t;
        timeLimit = t;
        board = Board(boardState, currentPlayer);
    }
    else{
        cout << "Enter a time limit: ";
        cin >> t;
        timeLimit = t;
        board = Board();
    }
}


// returns false if game over
bool Game::randomMove(){
    vector<Board::Move> moves = board.LegalMoves();
    bool numMoves = moves.size() > 0;
    if(!board.NextPlayer(numMoves))
        return false;
    board.ApplyMove(moves[rand() % moves.size()]);
    return true;
}

void Game::Start(){
    //based on case, make computer move.
    while(!board.EndState()){
        int movenum = 0;
        vector<Board::Move> m = b->LegalMoves(WHITE);
        for(int i = 0; i < m.size(); i++)
            cout << i << ": [" << (int)m[i].square.y << "," << (int)m[i].square.x << "]" << endl;
        cout << "choose a move: ";
        cin >> movenum;
        b->ApplyMove(m[movenum]);
        b->Print();
    }
    cout << "Game Over..." << endl;
}

#endif //_GAME_CPP_
