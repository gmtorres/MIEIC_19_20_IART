#include <iostream>
#include <chrono> 

#include "game.h"

using namespace std;
using namespace std::chrono; 

int main(void){

    cout<<"       <------  Welcome to Eximo!  ------>\n\n"<<endl;

    Game game = Game(1,3);

    //game.game_loop();
    /*auto start = high_resolution_clock::now(); 

    game.board.get_valid_boards(1);

    auto stop = high_resolution_clock::now();

    auto duration = duration_cast<microseconds>(stop - start); 
  
    cout << "Time taken by function: "
         << duration.count() << " microseconds" << endl; */

    return 0;
}