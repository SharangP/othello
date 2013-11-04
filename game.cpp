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
#include <unistd.h>
#include <stdlib.h>
#include "game.h"
#include "board.h"
#include "const.h"

using namespace std;


//Game::Game()
//  empty Game constructor
Game::Game(){}


//void Game::Setup(int gameType)
//  game setup method - initialies board and game state
//  based on gameType, allowing user to import board
//  state from a file in the specified format
//  gameType:
//    1: go first (black)
//    2. go second (white)
//    3: computer vs. computer
void Game::Setup(int gameType){
    char import;

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
        int player, i = 0, j = 0;
        string fileName;
        fstream boardFile;

        cout << "Enter a file name: ";
        cin >> fileName;
        boardFile.open(fileName.c_str(), fstream::in);

        //make sure user enters a valid file path
        while(boardFile.fail()){
            cout << "Invalid file!" << endl;
            cout << "Enter a file name: ";
            cin >> fileName;
            boardFile.open(fileName.c_str(), fstream::in);
        }

        //scan the file and generate a board state
        while((c = boardFile.get()) != EOF){
            if((c == '0') || (c == '1') || (c == '2')){
                boardState[i][j++] = c-'0';
                if(j == BOARDSIZE){
                    j = 0;
                    if(++i == BOARDSIZE)
                        break;
                }
            }
        }

        boardFile >> player;
        boardFile >> timeLimit;
        board = Board(boardState, player);
        boardFile.close();
    }
    else{
        string t;
        timeLimit = -1;
        while(timeLimit < 0){
            cout << "Enter a time limit: ";
            cin >> t;
            if(isdigit(t.c_str()[0]))
                timeLimit = atoi(t.c_str());
        }
        board = Board();
    }
}


//int Game::heuristic(Board b)
//  heuristic evaluation of board state
//  accounts for number of pieces of each color, corners,
//  potential corners (that the opponent can capture),
//  frontier pieces, mobility, and edges
//
//  reference:
//  Kartik Kukreja, New Delhi, India
//  {http://kartikkukreja.wordpress.com/2013/03/30/heuristic-function-for-reversiothello/}
int Game::heuristic(Board b){
    double piececount, corners, potentialCorners, edges, frontier, mobility;
    int mine, opp;

    int opponent = (maxPlayer == WHITE)
        ? BLACK
        : WHITE;

    //set weights of heuristic parameters
    int pW = 3*(b.score[maxPlayer]+b.score[opponent]);
    int cW = 5000;
    int pcW = 3000;
    int fW = 100;
    int eW = 200;
    int mW = 2*(100-(b.score[maxPlayer]+b.score[opponent]));

    //piece count
    piececount = (100.0*b.score[maxPlayer])/(b.score[maxPlayer] + b.score[opponent]);
    
    //corners
    mine = 0, opp = 0;
    if(b.board[0][0] == maxPlayer) mine++;
    else if(b.board[0][0] == opponent) opp++;
    if(b.board[0][7] == maxPlayer) mine++;
    else if(b.board[0][7] == opponent) opp++;
    if(b.board[7][0] == maxPlayer) mine++;
    else if(b.board[7][0] == opponent) opp++;
    if(b.board[7][7] == maxPlayer) mine++;
    else if(b.board[7][7] == opponent) opp++;
    corners = 25.0*(mine - opp);

    //edges and frontier
    int myEdges = 0, oppEdges = 0;
    int myFrontier = 0, oppFrontier = 0;
    for(int i = 0; i < BOARDSIZE; i++){
        for(int j = 0; j < BOARDSIZE; j++){
            if(i == 0 || i == BOARDSIZE-1 || j == 0 || j == BOARDSIZE-1){
                if(b.board[i][j] == maxPlayer) myEdges++;
                else if(b.board[i][j] == opponent) oppEdges++;
            }
            else if(b.board[i][j] != '0'){
                if(b.OnFrontier(i, j)){
                    if(b.board[i][j] == maxPlayer) myFrontier++;
                    else if(b.board[i][j] == opponent) oppFrontier++;
                }
            }
        }
    }
    edges = 100.0*myEdges/(myEdges + oppEdges);
    frontier = -100*(myFrontier - oppFrontier); //frontier pieces are bad!

    //mobility
    vector<Board::Move> myLegal = board.LegalMoves(maxPlayer);
    vector<Board::Move> oppLegal = board.LegalMoves(opponent);
    mobility = 100.0*myLegal.size()/(myLegal.size() + oppLegal.size());
    
    //potential corners (pseudo-expand node)
    opp = 0;
    for(int i = 0; i < oppLegal.size(); i++){
        if(oppLegal[i].square.y == 0 && oppLegal[i].square.x == 0) opp++;
        else if(oppLegal[i].square.y == 0 && oppLegal[i].square.x == (BOARDSIZE-1)) opp++;
        else if(oppLegal[i].square.y == (BOARDSIZE-1) && oppLegal[i].square.x == 0) opp++;
        else if(oppLegal[i].square.y == (BOARDSIZE-1) && oppLegal[i].square.x == (BOARDSIZE-1)) opp++;
    }
    potentialCorners = -25.0*opp;

    return pW*piececount + cW*corners + pcW*potentialCorners+ eW*edges + fW*frontier + mW*mobility;
}


//int Game::alphabeta(Board board, int depth, int alpha, int beta, bool maxPlayer)
//  alpha beta search method implementing minimax A* search
//  with alpha beta pruning
//  reference
//  {http://aima.cs.berkeley.edu/python/games.html}
int Game::alphabeta(Board board, int depth, int alpha, int beta, bool maxPlayer){
    int a = alpha, b = beta, msize;

    //do a quick check on time limit and depth
    if((((float)(clock()-startTime))/CLOCKS_PER_SEC) > TIMECUTOFF*timeLimit){
        timeout = true;
        return heuristic(board);
    }
    else if(depth == 0)
        return heuristic(board);
    else
        depth--;

    vector<Board::Move> m = board.LegalMoves(board.currentPlayer); //expand
    msize = m.size();

    if(msize == 0){ //no legal moves
        if(board.TerminalState(true)){ //check terminal state
            Board child = board;
            child.NextPlayer(false);
            return heuristic(child);
        }
        else{ //if pass is only move, continue search with pass
            Board child = board;
            child.NextPlayer(true);
            return alphabeta(child, depth, alpha, beta, !maxPlayer);
        }
    }

    if(maxPlayer){ //maximize alpha
        int v = INT_MIN;
        for(int i = 0; i < msize; i++){
            Board child = board;
            child.ApplyMove(m[i]);
            child.NextPlayer(false);

            int eval = alphabeta(child, depth, a, b, false);
            v = MAX(v, eval);
            
            //if opponent can make a move that will give max
            //a lower score than alpha, this branch is not
            //worth exploring
            if(v >= beta)
                return v;
            a = MAX(a, v);
        }
        return v;
    }
    else{ //minimize beta
        int v = INT_MAX;
        for(int i = 0; i < msize; i++){
            Board child = board;
            child.ApplyMove(m[i]);
            child.NextPlayer(false);

            int eval = alphabeta(child, depth, a, b, true);
            v = MIN(v, eval);
            
            //if opponent can make a move that will give max
            //a lower score than alpha, this branch is not
            //worth exploring
            if(v <= a)
                return v;
            b = MIN(b, v);
        }
        return v;
    }
}


//bool Game::smartMove()
//  intelligent move selection using alpha beta tree search
//  returns false if game in terminal state
bool Game::smartMove(){
    int depth, eval;
    Board::Move move, bestMove;
    
    startTime = clock();
    maxPlayer = board.currentPlayer;
    
    //expand layer 1
    vector<Board::Move> legal = board.LegalMoves(board.currentPlayer);

    if(legal.size() == 0){ //if no legal moves, pass
        cout << "Computer had to pass :(" << endl;
        return board.NextPlayer(true);
    }

    //increment depth of search until time runs out
    //look for the move with the MAX evaluation
    for(depth = 0; (((float)(clock()-startTime))/CLOCKS_PER_SEC < timeLimit/2.0) && depth < NUMSQUARES; depth++){
        int alpha = INT_MIN, beta = INT_MAX, randMove = 1;
        timeout = false; //reset timeout

        for(int i = 0; i < legal.size(); i++){ //maximize alpha
            Board child = board;
            child.ApplyMove(legal[i]);
            child.NextPlayer(false);
            eval = alphabeta(child, depth, alpha, beta, false);

            //if this depth timed out, use the best move from the previous depth
            if(timeout)
                break;

            if(eval > alpha){
                move = legal[i];
                alpha = eval;
            }
            else if(eval == alpha){
                //use the new move instead of the existing one with uniform probabilty
                if(((rand() % randMove++)-1) == 0)
                    move = legal[i];
            }
        }
        bestMove = move;
    }

    board.Print(vector<Board::Move>(1,move), true);
    board.ApplyMove(move);
    return board.NextPlayer(false);
}


//bool Game::randomMove()
//  random move method for testing
//  returns false if game in terminal state
bool Game::randomMove(){
    vector<Board::Move> m = board.LegalMoves(board.currentPlayer);
    if(m.size()){
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


//bool Game::humanMove()
//  method to apply move to game based on human input
//  returns false if game in terminal state
bool Game::humanMove(){
    int moveNum = -1;
    string in;

    vector<Board::Move> m = board.LegalMoves(board.currentPlayer);
    if(m.size()){
        board.Print(m);
        for(int i = 0; i < m.size(); i++)
            cout << i << ": [" << (int)m[i].square.y << "," << (int)m[i].square.x << "]" << endl;

        //make sure the user inputs a valid move
        while(!(moveNum < m.size() && moveNum >= 0)){
            cout << "Choose your move wisely: " << endl;
            cin >> in;
            if(isdigit(in.c_str()[0]))
                moveNum = atoi(in.c_str());
        }
        board.ApplyMove(m[moveNum]);
        return board.NextPlayer(false);
    }
    else{
        cout << "Sorry, you have to pass :(" << endl;
        return board.NextPlayer(true);
    }
}


//void Game::Play()
//  main game loop
void Game::Play(){
    bool gameOver = false;
    srand(time(NULL)); //seed rand

    cout << "Let the game begin!" << endl << endl;
    board.Print();

    while(!gameOver){
        if(humanPlayer[board.currentPlayer])
            gameOver = humanMove();
        else
            gameOver = smartMove();
    }
    board.Print();
    board.GameOver();
}

#endif //_GAME_CPP_
