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
    int is_valid = valid_move(x_orig,y_orig,x_dest,y_dest,white_player);
    if(!is_valid)
        return;
    if(white_player)
        move_piece_white(x_orig,y_orig,x_dest,y_dest);
    else if(!white_player)
        move_piece_black(x_orig,y_orig,x_dest,y_dest);
    if(is_valid == 1){ //NORMAL MOVE
        current_player = !current_player;
        jumpingMove = -1;
        capturingMove = -1;
    }else if(is_valid == 2){ //JUMPING MOVE
        jumpingMove = y_dest * BOARD_SIZE + x_dest;
    }else if(is_valid == 3){ //CAPTURING
        if(white_player) remove_piece_black((x_orig + x_dest)/2 , (y_orig + y_dest)/2);
        else if(!white_player) remove_piece_white((x_orig + x_dest)/2 , (y_orig + y_dest)/2);
        capturingMove = y_dest * BOARD_SIZE + x_dest; 
    }
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


unsigned int Board::valid_move(size_t x_orig, size_t y_orig,size_t x_dest, size_t y_dest, bool white_player){
    
    //se nao houver peça daquele jogador na posição de origem
    if((white_player && !get_piece_white(x_orig,y_orig)) || (!white_player && !get_piece_black(x_orig,y_orig)))
        return false;
    //se houver peça na posição de destino,
    if(is_piece(x_dest,y_dest)){
        return false;
    }

    if(jumpingMove != -1){
        if(int(y_orig*BOARD_SIZE+x_dest) != jumpingMove)
            return false;
    }
    else if(capturingMove != -1){
        if(int(y_orig*BOARD_SIZE+x_dest) != capturingMove)
            return false;
    }

    int x_delta = x_dest - x_orig;
    int y_delta = y_dest - y_orig;

    //se não houver peça na posição de destino
    if((x_delta == -1 || x_delta == 0 || x_delta == 1) && (y_delta == -1 || y_delta == 1) && jumpingMove == -1 && capturingMove == -1){
        //é possivel fazer um ORDINARY MOVE
        if((white_player && y_delta == -1) || (!white_player && y_delta == 1))
            return 1; //ORDINARY MOVE
    }
    else if((x_delta == -2 || x_delta == 0 || x_delta == 2)){
        int mid_x=x_orig+x_delta/2;
        int mid_y=y_orig+y_delta/2;
        // JUMPING MOVE ou alguns casos do CAPTURE
        if(y_delta == -2 || y_delta == 2){
            if(white_player && y_delta == -2){
                if(get_piece_white(mid_x,mid_y) && capturingMove == -1)
                    return 2; //JUMP
                else if(get_piece_black(mid_x,mid_y) && jumpingMove == -1)
                    return 3; //CAPTURE
            }
            else if(!white_player && y_delta == 2){
                if(get_piece_black(mid_x,mid_y) && capturingMove == -1)
                    return 2; //JUMP
                else if(get_piece_white(mid_x,mid_y) && jumpingMove == -1)
                    return 3; //CAPTURE
            }
        }else if(y_delta == 0 && x_delta != 0 && jumpingMove == -1){
            if((white_player && get_piece_black(mid_x,mid_y)) || (!white_player && get_piece_white(mid_x,mid_y)) )
                return 3; //CAPTURE
        }
    }

    return false;
}

//TODO: better display
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
    cout<<endl;
}


/*--------------------- GAME ---------------------*/

Game::Game(){
    board = Board();
    cout<<"Board created"<<endl;
    //board.move_piece(5,2,4,3,false);
}

void Game::make_move(size_t x_orig, size_t y_orig,size_t x_dest, size_t y_dest, bool white_player){
    board.move_piece(x_orig,y_orig,x_dest,y_dest,board.current_player);
}

//TODO: parse information based on letter and number, draw the board accordingly
void Game::get_move(){
    size_t x_orig,y_orig,x_dest,y_dest;
    cout<<"Player "<< (board.current_player ? 1 : 2) <<" turn.\n";
    cout<<"Origin x: "; cin>>x_orig;
    cout<<"Origin Y: "; cin>>y_orig;
    cout<<"Destination x: "; cin>>x_dest;
    cout<<"Destination Y: "; cin>>y_dest;
    make_move(x_orig,y_orig,x_dest,y_dest,board.current_player);
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
