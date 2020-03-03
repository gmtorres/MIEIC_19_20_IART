#include "game.h"

Board::Board(){
    for(size_t a = 0; a < BOARD_SIZE; ++a){
        for(size_t b = 0; b < BOARD_SIZE; ++b){
            board[a*BOARD_SIZE + b] = 0;
        }
    }
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

bool Board::get_piece_white(size_t pos){
    if(pos>=BOARD_SIZE*BOARD_SIZE)
        return 0;
    return (board_white >> pos) & 1L;
}
bool Board::get_piece_white(size_t x, size_t y){
    return get_piece_white(y*BOARD_SIZE + x);
}
bool Board::get_piece_black(size_t pos){
    if(pos>=BOARD_SIZE*BOARD_SIZE)
        return 0;
    return (board_black >> pos) & 1L;
}
bool Board::get_piece_black(size_t x, size_t y){
    return get_piece_black(y*BOARD_SIZE + x);
}
bool Board::is_piece(size_t pos){
    if(pos>=BOARD_SIZE*BOARD_SIZE)
        return 0;
    return get_piece_white(pos) || get_piece_black(pos);
}
bool Board::is_piece(size_t x, size_t y){
    return is_piece(y*BOARD_SIZE + x);
}
void Board::put_piece_white(size_t pos){
    if(pos>=BOARD_SIZE*BOARD_SIZE)
        return;
    board_white |= (1L << pos);
}
void Board::put_piece_white(size_t x, size_t y){
    put_piece_white(y*BOARD_SIZE + x);
}
bool Board::remove_piece_white(size_t pos){
    if(pos>=BOARD_SIZE*BOARD_SIZE || !get_piece_white(pos))
        return 0;
    board_white = board_white & ~(1L << pos);
    return 1;
}
bool Board::remove_piece_white(size_t x, size_t y){
    return remove_piece_white(y*BOARD_SIZE + x);
}
void Board::put_piece_black(size_t pos){
    if(pos>=BOARD_SIZE*BOARD_SIZE)
        return;
    board_black |= (1L << pos);
}
void Board::put_piece_black(size_t x, size_t y){
    put_piece_black(y*BOARD_SIZE + x);
}
bool Board::remove_piece_black(size_t pos){
    if(pos>=BOARD_SIZE*BOARD_SIZE || !get_piece_black(pos))
        return 0;
    board_black = board_black & ~(1L << pos);
    return 1;
}
bool Board::remove_piece_black(size_t x, size_t y){
    return remove_piece_black(y*BOARD_SIZE + x);
}
void Board::move_piece(size_t x_orig, size_t y_orig,size_t x_dest, size_t y_dest, bool white_player){
    if(!valid_move(x_orig,y_orig,x_dest,y_dest,white_player))
        return;
    if(white_player)
        move_piece_white(x_orig,y_orig,x_dest,y_dest);
    else
        move_piece_black(x_orig,y_orig,x_dest,y_dest);
}
void Board::move_piece_white(size_t pos_orig, size_t pos_dest){
    if(remove_piece_white(pos_orig))
        put_piece_white(pos_dest);
}
void Board::move_piece_black(size_t pos_orig, size_t pos_dest){
    if(remove_piece_black(pos_orig))
        put_piece_black(pos_dest);
}
void Board::move_piece_white(size_t x_orig, size_t y_orig,size_t x_dest, size_t y_dest){
    move_piece_white(y_orig*BOARD_SIZE+x_orig, y_dest*BOARD_SIZE+x_dest);
}
void Board::move_piece_black(size_t x_orig, size_t y_orig,size_t x_dest, size_t y_dest){
    move_piece_black(y_orig*BOARD_SIZE+x_orig, y_dest*BOARD_SIZE+x_dest);
}


bool Board::valid_move(size_t x_orig, size_t y_orig,size_t x_dest, size_t y_dest, bool white_player){
    
    //se nao houver peça daquele jogador na posição de origem
    if((white_player && !get_piece_white(x_orig,y_orig)) || (!white_player && !get_piece_black(x_dest,y_dest)))
        return false;
    //se houver peça na posição de destino,
    if(is_piece(x_dest,y_dest)){
        return false;
    }

    int x_delta = x_dest - x_orig;
    int y_delta = y_dest - y_orig;

    
    //se não houver peça na posição de destino

    if((x_delta == -1 || x_delta == 0 || x_delta == 1) && (y_delta == -1 || y_delta == 1)){
        //é possivel fazer um ORDINARY MOVE
        if((white_player && y_delta == -1) || (!white_player && y_delta == 1))
            return true;
    }
    else if((x_delta == -2 || x_delta == 0 || x_delta == 2) && (y_delta == -2 || y_delta == 2)){
        //é possivel fazer um JUMPING MOVE
        if((white_player && y_delta == -2 && get_piece_white(x_orig+x_delta/2,y_orig+y_delta/2)) || (!white_player && y_delta == 2 && get_piece_black(x_orig+x_delta/2,y_orig+y_delta/2)))
            return true;
    }

    
    return false;
}


void Board::display(){


    for(size_t a = 0; a < BOARD_SIZE; ++a){
        for(size_t b = 0; b < BOARD_SIZE; ++b){
            short pos = a * BOARD_SIZE + b;
            short v=0;
            if(get_piece_white(pos))
                v = 1;
            if(get_piece_black(pos))
                v = 2;
            cout<<" | "<< v << " |";
        }
        cout<<"\n";
    }
}


/*--------------------- GAME ---------------------*/

Game::Game(){
    board = Board();
    cout<<"Board created"<<endl;
}

void Game::get_move(){
    size_t x_orig,y_orig,x_dest,y_dest;
    cout<<"Origin x: "; cin>>x_orig;
    cout<<"Origin Y: "; cin>>y_orig;
    cout<<"Destination x: "; cin>>x_dest;
    cout<<"Destination Y: "; cin>>y_dest;
    board.move_piece(x_orig,y_orig,x_dest,y_dest,true);
}




void Game::display(){
    board.display();
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
