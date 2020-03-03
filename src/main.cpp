#include <iostream>

#include "game.h"

using namespace std;

int main(void){

    cout<<"Ola"<<endl;

    Game game = Game();

    game.display();

    game.make_move(5,5,4,4,true);

    game.display();

    game.make_move(5,2,4,3,false);

    game.display();

    game.make_move(4,4,4,2,true);

    game.display();

    return 0;
}