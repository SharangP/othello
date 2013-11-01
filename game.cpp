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


Game::Game(){}

//gameTypes
//  1: go first (black)
//  2. go second (white)
//  3: computer vs. computer
void Game::Setup(int gameType){
    char import;
    int t;

    switch(gameType){
        case(1):
            cout << "in type 1" << endl;
            humanPlayer[0] = true;
            humanPlayer[1] = false;
            break;
        case(2):
            cout << "in type 2" << endl;
            humanPlayer[0] = false;
            humanPlayer[1] = true;
            break;
        case(3):
            cout << "in type 3" << endl;
            humanPlayer[0] = false;
            humanPlayer[1] = false;
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
            //if c is a number save it into the array
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
    vector<Board::Move> m = board.LegalMoves();
    if(m.size()){
        board.ApplyMove(m[rand() % m.size()]);
        return board.NextPlayer(false);
    }
    else
        return board.NextPlayer(true);
}


bool Game::humanMove(){
    int moveNum;

    vector<Board::Move> m = board.LegalMoves();
    if(m.size()){
        for(int i = 0; i < m.size(); i++)
            cout << i << ": [" << (int)m[i].square.y << "," << (int)m[i].square.x << "]" << endl;
        cout << "Choose your move wisely: ";
        cin >> moveNum;

        //make sure the user inputs a valid move
        while(moveNum > m.size() || moveNum < 0){
            cout << "Invalid move choice!" << endl;
            cout << "Choose your move wiselyer: ";
            cin >> moveNum;
        }
        board.ApplyMove(m[moveNum]);
        return board.NextPlayer(false);
    }
    else
        return board.NextPlayer(true);
}

void Game::Start(){
    bool gameOver = false;

    cout << "Let the game begin!" << endl << endl;
    board.Print();

    if(!humanPlayer[board.currentPlayer]){
        randomMove();
        board.Print();
    }

    while(true){
        if((gameOver = humanMove()))
            break;
        board.Print();
        if((gameOver = randomMove()))
            break;
        board.Print();
    }
    cout << "Game Over..." << endl;
}

#endif //_GAME_CPP_
