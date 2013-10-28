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

    cout << "Welcome to Sharang's Othello AI" << endl
         << "Choose a game type:" << endl
         << "1: go first (black)" << endl
         << "2. go second (white)" << endl
         << "3: computer vs. computer" << endl;
    cin >> gameType;
    Game game = Game(gameType);
//    Board *b = new Board();
//    b->Print();
//    while(true){
//        int movenum = 0;
//        vector<Board::Move> m = b->LegalMoves(WHITE);
//        for(int i = 0; i < m.size(); i++)
//            cout << i << ": [" << (int)m[i].square.y << "," << (int)m[i].square.x << "]" << endl;
//        cout << "choose a move: ";
//        cin >> movenum;
//        b->ApplyMove(m[movenum]);
//        b->Print();
//    }
}
