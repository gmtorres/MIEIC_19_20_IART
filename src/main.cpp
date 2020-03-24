#include <iostream>
#include <chrono> 

#include "game.h"

using namespace std;
using namespace std::chrono; 

int main(void){


    Game game = Game(true,1,2);

    //

    auto start = high_resolution_clock::now(); 

    game.game_loop();
    //game.get_move_ai3();
    //game.get_move_ai3();

    auto stop = high_resolution_clock::now();

    auto duration = duration_cast<milliseconds>(stop - start); 
  
    cout << "Time taken by game: "
         << duration.count() << " milliseconds" << endl; 

    return 0;
}