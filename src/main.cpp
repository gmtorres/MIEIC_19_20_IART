#include <iostream>

#include "game.h"

using namespace std;

int main(void){

    cout<<"Ola"<<endl;

    Game game = Game();

    game.display();

    game.get_move();

    game.display();

    return 0;
}