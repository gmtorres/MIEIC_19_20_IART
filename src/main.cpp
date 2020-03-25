#include <iostream>
#include <chrono> 

#include "game.h"

using namespace std;
using namespace std::chrono; 

int main(void){


    Game game = Game(true,1,2,1,3,true);

    //

    auto start = high_resolution_clock::now(); 

    game.game_loop();
    //game.get_move_ai3();
    //game.get_move_ai3();

    auto stop = high_resolution_clock::now();

    auto duration = duration_cast<milliseconds>(stop - start); 
  
    cout << "Time taken by game: "
         << duration.count() << " milliseconds" << endl; 


    /*for(int a = 1; a <= 3; a++){
        for(int b = 1; b <= 3; b++){
            Game game = Game(false,1,a,1,b,false);
            game.game_loop();
            cout<<"\n\n\n";
        }
    }*/

    cout<<"\a"<<endl;

    return 0;
}