#include "game.h"

Board::Board(){

}

vector<Board> Board::get_valid_boards(bool white){
    return vector<Board> ();
}

float Board::eval(){
    return 0;
}

bool Board::gameover(bool white_player){
    return 0;
}

bool Board::end_game(){
    return gameover(0) || gameover(1);
}



/*--------------------- GAME ---------------------*/

Game::Game(){
    cout<<"Board created"<<endl;
}



/*-------------------- MINIMAX --------------------*/


Minimax::Minimax(){

}

float Minimax::minimax(Board board, unsigned short depth, float alpha, float beta, bool maximizingPlayer,bool white_player){
    if(depth == 0 || board.end_game())
        return board.eval();

    vector<Board> nextBoards = board.get_valid_boards(white_player);
    auto nextBoardsEnd = nextBoards.end();
    
    if(maximizingPlayer){
        float maxEval = -INF;
        for(auto it = nextBoards.begin(); it != nextBoardsEnd; ++it){
            float eval = minimax(*it, depth-1, alpha, beta, false, !white_player);
            maxEval = (maxEval > eval) ? maxEval : eval;
            alpha = (alpha > eval) ? alpha : eval;
            if(beta <= alpha)
                break;
        }
        return maxEval;
    }
    else{
        float minEval = INF;
        for(auto it = nextBoards.begin(); it != nextBoardsEnd; ++it){
            float eval = minimax(*it, depth-1, alpha, beta, true, !white_player);
            minEval = (minEval < eval) ? minEval : eval;
            beta = (beta < eval) ? beta : eval;
            if(beta <= alpha)
                break;
        }
        return minEval;
    }
}
