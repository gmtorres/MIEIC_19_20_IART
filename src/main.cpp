#include <iostream>
#include <chrono> 

#include "game.h"

using namespace std;
using namespace std::chrono; 

int main(void){

    cout<<"       <------  Welcome to Eximo!  ------>\n\n"<<endl;

    Game game = Game(1,3);

    //

    auto start = high_resolution_clock::now(); 

    game.game_loop();

    auto stop = high_resolution_clock::now();

    auto duration = duration_cast<milliseconds>(stop - start); 
  
    cout << "Time taken by function: "
         << duration.count() << " milliseconds" << endl; 

    return 0;
}