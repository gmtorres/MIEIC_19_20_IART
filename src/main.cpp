#include <iostream>

#include "game.h"

using namespace std;

int main(void){

    cout<<"Ola"<<endl;

    Game game = Game(3,2);

    cout<<game.board.eval()<<endl;

    //game.board.get_valid_boards(1);

    //game.game_loop();

    vector<Board> nextBoards = game.board.get_valid_boards(game.board.current_player);

    for(int i = 0; i < nextBoards.size(); ++i){
        nextBoards[i].last_move.display(true);
    }


    Move move;

    float alpha = -INF, beta = INF;
    cout<<game.minimax.minimax(game.board,3,alpha,beta,1,move)<<endl;

    cout<<move.x_orig<<"  "<<move.y_orig<<"  "<<move.x_dest<<"  "<<move.y_dest<<"  "<<endl;



    return 0;
}