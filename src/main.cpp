#include <iostream>

#include "game.h"

using namespace std;

int main(void){

    cout<<"Ola"<<endl;

    Game game = Game(0,1);

    //game.board.get_valid_boards(1);

    game.game_loop();

    return 0;
}