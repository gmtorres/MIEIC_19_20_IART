#include <iostream>
#include <chrono> 

#include "game.h"

using namespace std;
using namespace std::chrono; 

int main(void){


    Game game = Game(false,3,2,3,3,true);

    //

    auto start = high_resolution_clock::now(); 

    //game.game_loop();
    //game.get_move_ai3();
    //game.get_move_ai3();
    cout<<"START\n\n";
    float alpha = -INF, beta = INF,eval;
    Move m;
    game.minimax.minimax(game.board,6,alpha,beta,m,eval,start,15000000000, 3 );
    //game.minimax.minimax(game.board,7,alpha,beta,m,eval,start,15000000000, 3 );
    //game.minimax.minimax(game.board,8,alpha,beta,m,eval,start,15000000000, 3 );
    //game.minimax.minimax(game.board,9,alpha,beta,m,eval,start,15000000000, 3 );
    //game.minimax.minimax(game.board,10,alpha,beta,m,eval,start,15000000000, 3 );

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