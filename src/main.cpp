#include <iostream>
#include <chrono> 

#include "game.h"

using namespace std;
using namespace std::chrono; 

int main(void){

    cout<<"       <------  Welcome to Eximo!  ------>\n\n"<<endl;

    Game game = Game(1,3);

    game.game_loop();

    auto start = high_resolution_clock::now(); 

    //game.board.get_valid_boards(1);
    //int r = game.board.move_piece(Move(1,5,1,4),true);

    auto stop = high_resolution_clock::now();

    auto duration = duration_cast<nanoseconds>(stop - start); 
  
    cout << "Time taken by function: "
         << duration.count() << " nanoseconds" << endl; 

    return 0;
}