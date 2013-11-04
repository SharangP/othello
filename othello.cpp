/*************************
 * Othello
 *
 * An Othello AI that implements a minimax A* search
 * using alpha/beta pruning
 *
 * Sharang Phadke
 * 10/26/2013
 ************************/

#include <iostream>
#include "game.h"

using namespace std;


int main(int argc, char *argv[]){
    char gameType;
    Game game;

    cout << "Welcome to Sharang's Othello AI" << endl
         << "Choose a game type:" << endl
         << "1: go first (black)" << endl
         << "2. go second (white)" << endl
         << "3: computer vs. computer" << endl;

    while(true){
        cin >> gameType;
        if((gameType == '1') || (gameType == '2') || (gameType == '3')){
            game.Setup(gameType-'0');
            break;
        }
        cout << "Invalid option!" << endl << "Try again: ";
    }

    game.Play();
    return 0;
}
