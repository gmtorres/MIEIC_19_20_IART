#include <iostream>

#include "game.h"

using namespace std;

int main(void){

    cout<<"Ola"<<endl;

    Game game = Game(0,1);

    //game.board.get_valid_boards(1);

    game.game_loop();

    /*Move move;

    cout<<game.minimax.minimax(game.board,1,-INF,INF,1,move)<<endl;

    cout<<move.x_orig<<"  "<<move.y_orig<<"  "<<move.x_dest<<"  "<<move.y_dest<<"  "<<endl;*/



    return 0;
}