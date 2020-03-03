#include <iostream>

#include "game.h"

using namespace std;

int main(void){

    cout<<"Ola"<<endl;

    Game game = Game();

    game.board.get_valid_moves(0);

    game.display();

    return 0;
}