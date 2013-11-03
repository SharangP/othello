/*************************
 * GAME.CPP
 * game class declaration
 ************************/
#ifndef _GAME_CPP_
#define _GAME_CPP_

#include <iostream>
#include <fstream>
#include <vector>
#include <climits>
#include <time.h>
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
            humanPlayer[BLACK] = true;
            humanPlayer[WHITE] = false;
            break;
        case(2):
            humanPlayer[BLACK] = false;
            humanPlayer[WHITE] = true;
            break;
        case(3):
            humanPlayer[BLACK] = false;
            humanPlayer[WHITE] = false;
            break;
        default:
            throw;
    }

    cout << "Import a board state? (Y/N) ";
    cin >> import;

    //allow user to import a board state
    if(tolower(import) == 'y'){
        char c, boardState[8][8];
        int currentPlayer, i = 0, j = 0;
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


int Game::heuristic(Board board){
    int opponent = (board.currentPlayer == WHITE)
        ? BLACK
        : WHITE;
    return board.score[board.currentPlayer] - board.score[opponent];
}


int Game::alphabeta(Board board, int depth, int alpha, int beta, bool maxPlayer){
    int a = alpha, b = beta, msize;

    if(depth == 0) //do a quick check on depth
        return heuristic(board);
    else
        depth--;

    vector<Board::Move> m = board.LegalMoves(); //expand
    msize = m.size();

    if(msize == 0){
        if(board.TerminalState(true)) //if no legal moves, check terminal state
            return heuristic(board);
        else{ //if pass is only move, continue search with pass
            Board child = board;
            child.NextPlayer(true);
            return alphabeta(child, depth, alpha, beta, !maxPlayer);
        }
    }

    if(maxPlayer){ //maximize alpha
        for(int i = 0; i < msize; i++){

            Board child = board;
            child.ApplyMove(m[i]);
            child.NextPlayer(false);

            int eval = alphabeta(child, depth, alpha, beta, false);
            a = MAX(a, eval);

            cout << "===MAX move [" << (int)m[i].square.y << "," << (int)m[i].square.x << "] ";
            cout << "alpha: " << a << " beta: " << b << endl;

            //if opponent can make a move that will give max
            //a lower score than alpha, this branch is not
            //worth exploring
            if(b <= a)
                break;
        }
        return a;
    }
    else{ //minimize beta
        for(int i = 0; i < msize; i++){

            Board child = board;
            child.ApplyMove(m[i]);
            child.NextPlayer(false);

            int eval = alphabeta(child, depth, alpha, beta, true);
            b = MIN(b, eval);
            
            cout << "===MIN move [" << (int)m[i].square.y << "," << (int)m[i].square.x << "] ";
            cout << "alpha: " << a << " beta: " << b << endl;

            //if opponent can make a move that will give max
            //a lower score than alpha, this branch is not
            //worth exploring
            if(b <= a)
                break;
        }
        return b;
    }
}


bool Game::smartMove(){
    const int depthLimit = 3;
    int depth = 0, eval;
    Board::Move move;
    
    //expand layer 1
    vector<Board::Move> legal = board.LegalMoves();

    if(legal.size() == 0) //if no legal moves, pass
        return board.NextPlayer(true);

    //increment depth of search until time runs out
    //look for the move with the MAX evaluation
    for(depth = 0; depth < depthLimit; depth++){
        int alpha = INT_MIN, beta = INT_MAX;
        for(int i = 0; i < legal.size(); i++){
            cout << "Evaluating move [" << (int)legal[i].square.y << "," << (int)legal[i].square.x << "]" << endl;
            Board child = board;
            child.ApplyMove(legal[i]);
            child.NextPlayer(false);
            eval = alphabeta(child, depth, alpha, beta, false);
            cout << "best eval: " << eval << endl;
            if(eval > alpha){ //TODO:randomize if equal
                move = legal[i];
                alpha = eval;
            }
        }
    }

    board.Print(vector<Board::Move>(1,move), true);
    board.ApplyMove(move);
    return board.NextPlayer(false);
}


// returns false if game over
bool Game::randomMove(){
    vector<Board::Move> m = board.LegalMoves();
    if(m.size()){
        cout << "in randomMove with m.size() > 0" << endl;
        Board::Move randMove = m[rand() % m.size()];
        board.Print(vector<Board::Move>(1,randMove), true);
        board.ApplyMove(randMove);
        return board.NextPlayer(false);
    }
    else{
        cout << "Computer had to pass :(" << endl;
        return board.NextPlayer(true);
    }
}


bool Game::humanMove(){
    int moveNum;

    vector<Board::Move> m = board.LegalMoves();
    if(m.size()){
        board.Print(m);
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

void Game::Play(){
    bool gameOver = false;
    srand(time(NULL)); //seed rand

    cout << "Let the game begin!" << endl << endl;
    board.Print();

    if(!humanPlayer[board.currentPlayer])
        smartMove();//randomMove();

    while(true){
        gameOver = humanMove();
        if(gameOver)
            break;

        gameOver = smartMove();//randomMove();
        if(gameOver)
            break;
    }
    board.Print();
    board.GameOver();
}

#endif //_GAME_CPP_
