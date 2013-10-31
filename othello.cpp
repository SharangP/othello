/*************************
 * Othello
 *
 * An Othello AI that implements a simple A* minimax search
 * using alpha/beta pruning
 *
 * Sharang Phadke
 * 10/26/2013
 ************************/

#include <iostream>
#include "game.h"
#include "const.h"

using namespace std;

int main(int argc, char *argv[]){
    int gameType;
    Game game;

    while(true){
        cout << "Welcome to Sharang's Othello AI" << endl
             << "Choose a game type:" << endl
             << "1: go first (black)" << endl
             << "2. go second (white)" << endl
             << "3: computer vs. computer" << endl;
        cin >> gameType;
        if((gameType == 1) || (gameType == 2) || (gameType == 3)){
            game.Setup(gameType);
            break;
        }
        cout << "Invalid option!" << endl;
    }
    game.Start();
}
