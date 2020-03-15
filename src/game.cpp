#include "game.h"

size_t count_set_bit(ulong n){
    int t = 0;

    while(n != 0){
        t += (n & 1L);
        n = n >> 1;
    }

    return t;
}

void Move::display(bool t=false){
    cout<<(int)x_orig<<","<<(int)y_orig<<" ->"<<(int)x_dest<<","<<(int)y_dest;
    if(t) cout<<"  "<< type;
}

void Move::set_vars(u_char x_o,u_char y_o,u_char x_d,u_char y_d){
    x_orig = x_o;
    y_orig = y_o;
    x_dest = x_d;
    y_dest = y_d;
}

bool Move::operator==(Move const &m2){
    if(this->x_orig == m2.x_orig && this->y_orig == m2.y_orig && this->x_dest == m2.x_dest && this->y_dest== m2.y_dest)
        return true;
    return false;
}


Board::Board(){}
Board::Board(const Board &old){
    board_white = old.board_white;
    board_black = old.board_black;
    jumpingMove = old.jumpingMove;
    capturingMove = old.capturingMove;
    current_player = old.current_player;
    dropPiece = old.dropPiece;
    last_move = old.last_move;
}

bool Board::operator==(const Board& b2)const{
    return (board_black == b2.board_black && board_white == b2.board_white && capturingMove == b2.capturingMove && current_player == b2.current_player && dropPiece == b2.dropPiece && jumpingMove == b2.jumpingMove);
}



vector<Board> Board::get_valid_boards(bool white_player){
    vector<Move> moves = get_valid_moves(white_player);
    vector<Board> boards = vector<Board> ();
    for(auto it = moves.begin(); it != moves.end(); ++it){
        Board temp = *this;
        Move m = *it;
        temp.move_piece(m,white_player,m.type);
        temp.last_move.set_vars(m.x_orig,m.y_orig,m.x_dest,m.y_dest);
        boards.push_back(temp);

    }

    return boards;
}

float Board::eval() const{ // positive favours white, negative favours black

    /*if(gameover(0))
        return 1000;
    else if(gameover(1))
        return -1000;*/

    float score = 0;

    ulong b1 = board_white, b2 = board_black;
    while (b1 != 0 || b2 != 0){
        score = score + (b1 & 1L) - (b2 & 1L);
        b1 = b1 >> 1;
        b2 = b2 >> 1; 
    }

    score += 1.5*count_set_bit(board_white & 0xFF00); // bits na penultima fila para ser convertidos
    score -= 1.5*count_set_bit(board_black & 0xFF000000000000L); // bits na penultima fila para seer convertidos
    score += 1.2*count_set_bit(board_white & 0xFF00000000000000L); // bits na primeira fila a proteger
    score -= 1.2*count_set_bit(board_black & 0xFFL); // bits na primeira fila a proteger
    score += 0.8*count_set_bit(board_white & 0x3C3C000000L); //board center
    score -= 0.8*count_set_bit(board_black & 0x3C3C000000L); //board center
    score += 1.125*count_set_bit(board_white & 0x8181818181818181); // board sides
    score -= 1.125*count_set_bit(board_black & 0x8181818181818181); //board sides


    return score;
}

bool Board::gameover(bool white_player){ // check if a player has lost the game
    ulong board;
    if(white_player) board = board_white;
    else board = board_black;
    if(board == 0)
        return true;

    if(!any_move_available(white_player)){
        //cout<<"No more possible moves for player"<<(current_player ? 1 : 2)<<endl;
        return true;
    }
    return false;
}

bool Board::end_game(bool white_player){
    if(board_white == 0 || board_black == 0){
        return true;
    }
    
    if(!any_move_available(white_player)){
        //cout<<"No more possible moves for player"<<(current_player ? 1 : 2)<<endl;
        return true;
    }
    return false;
}

unsigned int Board::valid_move(Move move, bool white_player){
    vector<Move> moves = get_valid_moves(white_player);
    for(auto it = moves.begin(); it != moves.end();++it){
        Move m = *it;
        if(move == m)
            return m.type;
    }
    return 0;
}


void Board::get_valid_moves_aux(vector<Move> &moves, Move move, bool &capture, bool white_player){
    int r = valid_move_aux(move, white_player);
    move.type = r;
    if(capture == true){
        if(r == CAPTURE)
            moves.push_back(move);
    }else if(capture == false){
        if(r == CAPTURE){
            capture = true;
            moves = vector<Move>();
            moves.push_back(move);
        }else if(r){
            moves.push_back(move);
        }
    }
}

//true if there is a move available
bool Board::any_move_available(bool white_player){
    ulong board;
    if(white_player) board = board_white;
    else if(!white_player) board = board_black;

    if(dropPiece == 0){
        u_char x = 0;
        u_char y = 0;
        while(board != 0){
            if(board & 1L){
                
                if(white_player){
                    if(valid_move_aux(Move(x,y,x-2,y),true)) return true;
                    if(valid_move_aux(Move(x,y,x-2,y-2),true)) return true;
                    if(valid_move_aux(Move(x,y,x-1,y-1),true)) return true;
                    if(valid_move_aux(Move(x,y,x,y-1),true)) return true;
                    if(valid_move_aux(Move(x,y,x,y-2),true)) return true;
                    if(valid_move_aux(Move(x,y,x+1,y-1),true)) return true;
                    if(valid_move_aux(Move(x,y,x+2,y-2),true)) return true;
                    if(valid_move_aux(Move(x,y,x+2,y),true)) return true;
                }else if(!white_player){
                    if(valid_move_aux(Move(x,y,x-2,y),false)) return true;
                    if(valid_move_aux(Move(x,y,x-2,y+2),false)) return true;
                    if(valid_move_aux(Move(x,y,x-1,y+1),false)) return true;
                    if(valid_move_aux(Move(x,y,x,y+1),false)) return true;
                    if(valid_move_aux(Move(x,y,x,y+2),false)) return true;
                    if(valid_move_aux(Move(x,y,x+1,y+1),false)) return true;
                    if(valid_move_aux(Move(x,y,x+2,y+2),false)) return true;
                    if(valid_move_aux(Move(x,y,x+2,y),false)) return true;
                }
            }
            ++x;
            if(x == BOARD_SIZE){
                x = 0;
                ++y;
            }
            board = board >> 1;
        }
    }else{
        for(u_char y = 0; y < 2; ++y){
            for(u_char x = 1; x < BOARD_SIZE; ++x){
                if(white_player) if(valid_move_aux(Move(0,0,x, y + 6),true)) return true;
                if(!white_player) if(valid_move_aux(Move(0,0,x, y),false)) return true;
                
            }
        }
    }
    return false;
}

vector<Move> Board::get_valid_moves(bool white_player){
    vector<Move> moves = vector<Move>();

    ulong board;
    if(white_player) board = board_white;
    else if(!white_player) board = board_black;

    bool capture_possible = false;

    if(dropPiece == 0){
        u_char x = 0;
        u_char y = 0;
        while(board != 0){
            if(board & 1L){
                
                if(white_player){
                    get_valid_moves_aux(moves,Move(x,y,x-2,y),capture_possible,true);
                    get_valid_moves_aux(moves,Move(x,y,x-2,y-2),capture_possible,true);
                    get_valid_moves_aux(moves,Move(x,y,x-1,y-1),capture_possible,true);
                    get_valid_moves_aux(moves,Move(x,y,x,y-1),capture_possible,true);
                    get_valid_moves_aux(moves,Move(x,y,x,y-2),capture_possible,true);
                    get_valid_moves_aux(moves,Move(x,y,x+1,y-1),capture_possible,true);
                    get_valid_moves_aux(moves,Move(x,y,x+2,y-2),capture_possible,true);
                    get_valid_moves_aux(moves,Move(x,y,x+2,y),capture_possible,true);
                }else if(!white_player){
                    get_valid_moves_aux(moves,Move(x,y,x-2,y),capture_possible,false);
                    get_valid_moves_aux(moves,Move(x,y,x-2,y+2),capture_possible,false);
                    get_valid_moves_aux(moves,Move(x,y,x-1,y+1),capture_possible,false);
                    get_valid_moves_aux(moves,Move(x,y,x,y+1),capture_possible,false);
                    get_valid_moves_aux(moves,Move(x,y,x,y+2),capture_possible,false);
                    get_valid_moves_aux(moves,Move(x,y,x+1,y+1),capture_possible,false);
                    get_valid_moves_aux(moves,Move(x,y,x+2,y+2),capture_possible,false);
                    get_valid_moves_aux(moves,Move(x,y,x+2,y),capture_possible,false);
                }
            }
            ++x;
            if(x == BOARD_SIZE){
                x = 0;
                ++y;
            }
            board = board >> 1;
        }
    }else{
        for(u_char y = 0; y < 2; ++y){
            for(u_char x = 1; x < BOARD_SIZE; ++x){
                if(white_player) get_valid_moves_aux(moves,Move(0,0,x, y + 6),capture_possible,true);
                if(!white_player) get_valid_moves_aux(moves,Move(0,0,x, y),capture_possible,false);
                
            }
        }
    }
    return moves;
}

//check if a move is valid, not caring if there is capturing moves possible
unsigned int Board::valid_move_aux(const Move &move, bool white_player){
    if(move.x_dest >= BOARD_SIZE || move.y_dest >=BOARD_SIZE)
        return false;
    u_char pos_orig = move.y_orig * BOARD_SIZE + move.x_orig;
    u_char pos_dest = move.y_dest * BOARD_SIZE + move.x_dest;

    ulong occupied = board_white | board_black;
    

    if(occupied & (1L << pos_dest))
        return false;
    
    if(dropPiece == 0){

        if( (white_player && !(board_white & (1L << pos_orig))) || (!white_player && !(board_black & (1L << pos_orig))) )
            return false;

        if(jumpingMove != -1 && jumpingMove != int(pos_orig))
            return false;
        if(capturingMove != -1 && capturingMove != int(pos_orig))
            return false;

        char delta_x = move.x_dest - move.x_orig;
        char delta_y = move.y_dest - move.y_orig;

        if((delta_x == -1 || delta_x == 0 || delta_x == 1) && jumpingMove == -1 && capturingMove == -1){
            if(white_player && delta_y == -1)
                return ORDINARY_MOVE;
            if(!white_player && delta_y == 1)
                return ORDINARY_MOVE;
        }
        if(delta_x == -2 || delta_x == 0 || delta_x == 2){
            if(white_player && (delta_y == -2 || delta_y == 0)){
                if(board_black & (1L << ((move.y_orig + (delta_y >> 1)) * BOARD_SIZE + (move.x_orig + (delta_x >> 1)))) && jumpingMove == -1)
                    return CAPTURE;
                if(delta_y == -2 && board_white & (1L << ((move.y_orig - 1) * BOARD_SIZE + (move.x_orig + (delta_x >> 1)))) && capturingMove == -1)
                    return JUMPING_MOVE;
            }
            if(!white_player && (delta_y == 2 || delta_y == 0)){
                if(board_white & (1L << ((move.y_orig + (delta_y >> 1)) * BOARD_SIZE + (move.x_orig + (delta_x >> 1))))  && jumpingMove == -1)
                    return CAPTURE;
                if(delta_y == 2 && board_black & (1L << ((move.y_orig + 1) * BOARD_SIZE + (move.x_orig + (delta_x >> 1))))  && capturingMove == -1)
                    return JUMPING_MOVE;
            }
        }
    }else{
        if(white_player && is_drop_zone_white(move.x_dest,move.y_dest))
            return DROP;
        if(!white_player && is_drop_zone_black(move.x_dest,move.y_dest))
            return DROP;
    }

    return false;
}


bool Board::move_piece(const Move &move, bool white_player, int valid = false){
    
    u_char type;
    if(valid == 0){
        type = valid_move(move,white_player);
        if(type == false)
            return false;
    }else
        type = move.type;

    if(type != DROP){
        if(white_player)
            move_piece_white(move.x_orig,move.y_orig,move.x_dest,move.y_dest);
        else if(!white_player)
            move_piece_black(move.x_orig,move.y_orig,move.x_dest,move.y_dest);
    }
    
    switch (type){
        case ORDINARY_MOVE:

            if((white_player && is_last_white(move.x_dest,move.y_dest)) || (!white_player && is_last_black(move.x_dest,move.y_dest))){
                dropPiece = 2;
                if(white_player) remove_piece_white(move.x_dest,move.y_dest);
                else if(!white_player) remove_piece_black(move.x_dest,move.y_dest);
                if(!any_move_available(white_player)){
                    dropPiece = 0;
                    current_player = !current_player;
                }
            }else
                current_player = !current_player;
            
            break;

        case JUMPING_MOVE:
            if((white_player && is_last_white(move.x_dest,move.y_dest)) || (!white_player && is_last_black(move.x_dest,move.y_dest))){
                dropPiece = 2;
                if(white_player) remove_piece_white(move.x_dest,move.y_dest);
                else if(!white_player) remove_piece_black(move.x_dest,move.y_dest);
                if(!any_move_available(white_player)){
                    dropPiece = 0;
                    current_player = !current_player;
                }
            }else{
                jumpingMove = move.y_dest * BOARD_SIZE + move.x_dest;
                if(!any_move_available(white_player)){
                    current_player = !current_player;
                    jumpingMove = -1;
                }
            }
            break;

        case CAPTURE:
            if(white_player) remove_piece_black((move.x_orig + move.x_dest)>>1 , (move.y_orig + move.y_dest)>>1);
            else if(!white_player) remove_piece_white((move.x_orig + move.x_dest)>>1 , (move.y_orig + move.y_dest)>>1);
            capturingMove = move.y_dest * BOARD_SIZE + move.x_dest;
            if(!any_move_available(white_player)){
                capturingMove = -1;
                if((white_player && is_last_white(move.x_dest,move.y_dest)) || (!white_player && is_last_black(move.x_dest,move.y_dest))){
                    dropPiece = 2;
                    if(white_player) remove_piece_white(move.x_dest,move.y_dest);
                    else if(!white_player) remove_piece_black(move.x_dest,move.y_dest);
                    if(!any_move_available(white_player)){
                        dropPiece = 0;
                        current_player = !current_player;
                    }
                }else{
                    current_player = !current_player;
                }

            }
            break;

        case DROP:
            if(white_player) put_piece_white(move.x_dest, move.y_dest);
            else if(!white_player) put_piece_black(move.x_dest, move.y_dest);
            --dropPiece;
            if( (dropPiece == 0) || (dropPiece > 0 && !any_move_available(white_player))){
                current_player = !current_player;
                dropPiece = 0;
            }

            break;

        default:
            break;
    }

    
    return true;
}











void Board::move_piece_white(u_char pos_orig, u_char pos_dest){
    if(remove_piece_white(pos_orig))
        put_piece_white(pos_dest);
}
void Board::move_piece_black(u_char pos_orig, u_char pos_dest){
    if(remove_piece_black(pos_orig))
        put_piece_black(pos_dest);
}
void Board::move_piece_white(u_char x_orig, u_char y_orig,u_char x_dest, u_char y_dest){
    move_piece_white(y_orig*BOARD_SIZE+x_orig, y_dest*BOARD_SIZE+x_dest);
}
void Board::move_piece_black(u_char x_orig, u_char y_orig,u_char x_dest, u_char y_dest){
    move_piece_black(y_orig*BOARD_SIZE+x_orig, y_dest*BOARD_SIZE+x_dest);
}



bool Board::get_piece_white(u_char pos){
    if(pos>=BOARD_SIZE*BOARD_SIZE)
        return false;
    return (board_white >> pos) & 1L;
}
bool Board::get_piece_white(u_char x, u_char y){
    if(x>=BOARD_SIZE || y>=BOARD_SIZE)
        return false;
    return get_piece_white(y*BOARD_SIZE + x);
}
bool Board::get_piece_black(u_char pos){
    if(pos>=BOARD_SIZE*BOARD_SIZE)
        return false;
    return (board_black >> pos) & 1L;
}
bool Board::get_piece_black(u_char x, u_char y){
    if(x>=BOARD_SIZE || y>=BOARD_SIZE)
        return false;
    return get_piece_black(y*BOARD_SIZE + x);
}
bool Board::is_piece(u_char pos){
    if(pos>=BOARD_SIZE*BOARD_SIZE)
        return false;
    return get_piece_white(pos) || get_piece_black(pos);
}
bool Board::is_piece(u_char x, u_char y){
    if(x>=BOARD_SIZE || y>=BOARD_SIZE){
        return false;
    }
    return is_piece(y*BOARD_SIZE + x);
}
void Board::put_piece_white(u_char pos){
    if(pos>=BOARD_SIZE*BOARD_SIZE)
        return;
    board_white |= (1L << pos);
}
void Board::put_piece_white(u_char x, u_char y){
    if(x>=BOARD_SIZE || y>=BOARD_SIZE)
        return;
    put_piece_white(y*BOARD_SIZE + x);
}
bool Board::remove_piece_white(u_char pos){
    if(pos>=BOARD_SIZE*BOARD_SIZE || !get_piece_white(pos))
        return false;
    board_white = board_white & ~(1L << pos);
    return true;
}
bool Board::remove_piece_white(u_char x, u_char y){
    if(x>=BOARD_SIZE || y>=BOARD_SIZE)
        return false;
    return remove_piece_white(y*BOARD_SIZE + x);
}
void Board::put_piece_black(u_char pos){
    if(pos>=BOARD_SIZE*BOARD_SIZE)
        return;
    board_black |= (1L << pos);
}
void Board::put_piece_black(u_char x, u_char y){
    if(x>=BOARD_SIZE || y>=BOARD_SIZE)
        return;
    put_piece_black(y*BOARD_SIZE + x);
}
bool Board::remove_piece_black(u_char pos){
    if(pos>=BOARD_SIZE*BOARD_SIZE || !get_piece_black(pos))
        return false;
    board_black = board_black & ~(1L << pos);
    return true;
}
bool Board::remove_piece_black(u_char x, u_char y){
    if(x>=BOARD_SIZE || y>=BOARD_SIZE)
        return false;
    return remove_piece_black(y*BOARD_SIZE + x);
}


//CHECK if OKAY
bool Board::is_last_white(u_char x, u_char y){
    return y == 0;
}
bool Board::is_last_black(u_char x, u_char y){
    return y == BOARD_SIZE-1;
}
bool Board::is_drop_zone_white(u_char x, u_char y){
    return (y == BOARD_SIZE -1 || y == BOARD_SIZE - 2) && (x != 0 && x != BOARD_SIZE - 1) ;
}
bool Board::is_drop_zone_black(u_char x, u_char y){
    return (y == 0 || y == 1) && (x != 0 && x != BOARD_SIZE - 1) ;
}





//TODO: better display
void Board::display(){

    for(u_char a = 0; a < BOARD_SIZE; ++a){
        for(u_char b = 0; b < BOARD_SIZE; ++b){
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

Game::Game(int player1Mode, int player2Mode) : player1(player1Mode) , player2(player2Mode){
    board = Board();
    minimax = Minimax();
}

bool Game::make_move(Move move, bool white_player){
    return board.move_piece(move,board.current_player,false);
}

void Game::get_move(){
    int player;
    if(board.current_player) player = player1;
    else if(!board.current_player) player = player2;
    cout<<"Player "<< (board.current_player ? 1 : 2) <<" turn. ";
    if(board.jumpingMove != -1)
        cout<<"JUMPING \n";
    else if(board.capturingMove != -1)
        cout<<"CAPTURING \n";
    else if(board.dropPiece != 0)
        cout<<"DROP \n";
    else cout<<"MOVE\n";

    if(player == 0){
        get_move_human();
    }else if(player == 1){
        get_move_ai1();
    }else if(player == 2){
        get_move_ai2();
    }else if(player == 3){
        get_move_ai3();
    }
}

//TODO: parse information based on letter and number, draw the board accordingly
void Game::get_move_human(){
    vector<Move> moves = board.get_valid_moves(board.current_player);
    cout<<"Available moves (#="<<moves.size()<<"): ";
    for(size_t i = 0; i < moves.size();++i){
        Move m = moves[i];
        cout<<(int)m.x_orig<<","<<(int)m.y_orig<<"->"<<(int)m.x_dest<<","<<(int)m.y_dest<<"   ";
    }
    cout<<endl;

    size_t x_orig=0,y_orig=0,x_dest,y_dest;
    if (board.dropPiece == 0) {
        cout<<"Origin x: "; cin>>x_orig;
        cout<<"Origin Y: "; cin>>y_orig;
    }

    cout<<"Destination x: "; cin>>x_dest;
    cout<<"Destination Y: "; cin>>y_dest;
    Move m = Move(x_orig,y_orig,x_dest,y_dest);
    bool move = make_move(m,board.current_player);
    if(!move)
        cout<<"Invalid move, try another one"<<endl;
}

void Game::get_move_ai1(){ //makes first move available
    vector<Move> moves = board.get_valid_moves(board.current_player);
    Move m = moves[0];
    m.display();cout<<endl;
    make_move(m,board.current_player);
}

void Game::get_move_ai2(){ //minimax from depth 3 but min time is 1000 ms
    unsigned int depth = 3;
    int64_t min_time_milli = 1000;

    Move m;
    cout<<"Start minimax"<<endl;

    float eval;
    float alpha = -INF, beta = INF;
    auto start = high_resolution_clock::now(); 
    auto stop = high_resolution_clock::now();
    while(duration_cast<milliseconds>(stop - start).count() < min_time_milli){
        eval = minimax.minimax(board,depth++,alpha,beta,m);
        stop = high_resolution_clock::now();
    }

    cout<<"EVAL: "<<eval<<"\t DEPTH: "<<depth-1<<endl;


    cout<<"End minimax"<<endl;
    m.display();cout<<endl;
    make_move(m,board.current_player);
}

void Game::get_move_ai3(){ //minimax from depth 6 but min time is 10000 ms

    unsigned int depth = 6;
    int64_t min_time_milli = 10000;

    Move m;
    cout<<"Start minimax"<<endl;

    float eval;
    float alpha = -INF, beta = INF;
    auto start = high_resolution_clock::now(); 
    auto stop = high_resolution_clock::now();
    while(duration_cast<milliseconds>(stop - start).count() < min_time_milli){
        eval = minimax.minimax(board,depth++,alpha,beta,m);
        stop = high_resolution_clock::now();
    }

    cout<<"EVAL: "<<eval<<"\t DEPTH: "<<depth-1<<endl;


    cout<<"End minimax"<<endl;
    m.display();cout<<endl;
    make_move(m,board.current_player);
}

void Game::game_loop(){

    display(); 

    while (true)
    {   
        //cout<<board.board_white<<endl;cout<<board.board_black<<endl;cout<<board.current_player<<endl;cout<<board.jumpingMove<<endl;cout<<board.capturingMove<<endl;
        if(board.end_game(board.current_player)){
            cout<<"END OF GAME  ----- PLAYER "<<(board.gameover(0) ? 1 : 2)<<" WON"<<endl;
            cout<<board.board_white<<endl;
            cout<<board.board_black<<endl;
            cout<<board.current_player<<endl;
            cout<<board.jumpingMove<<endl;
            cout<<board.capturingMove<<endl;
            break;
        }
        get_move();
        display(); 
    }
    

}


void Game::display(){
    board.display();
}



/*-------------------- MINIMAX --------------------*/


bool Minimax::compareBoards(const Board b1, const Board b2 ){
    float v1 = b1.eval();
    float v2 = b2.eval();
    return (v1 < v2);
}


Minimax::Minimax(){
}

float Minimax::minimax(Board &board, unsigned short depth, float alpha, float beta, Move &move){

    auto look = table.find(board);
    if(look != table.end() && look->second.depth >= depth){
        //return look->second.eval;
    }

    if(depth == 0 || board.end_game(board.current_player))
        return board.eval();

    vector<Board> nextBoards = board.get_valid_boards(board.current_player);
    sort(nextBoards.begin(),nextBoards.end(),compareBoards);
    
    Move temp_move;

    auto nextBoardsEnd = nextBoards.end();
    if(board.current_player){
        float maxEval = -INF;
        for(auto it = nextBoards.begin(); it != nextBoardsEnd; ++it){
            float eval = minimax_aux(*it, depth - (board.current_player != it->current_player ? 1 : 0) , alpha, beta);
            if(maxEval < eval){
                maxEval = eval;
                move = (*it).last_move;
            }
            alpha = (alpha > eval) ? alpha : eval;
            if(beta <= alpha)
                break;
        }
        auto entry = table.find(board);
        if(entry == table.end() || entry->second.depth < depth){
            Entry table_entry; 
            table_entry.eval = maxEval;
            table_entry.depth = depth;
            table_entry.alpha = alpha;
            table_entry.beta = beta;
            table[board] = table_entry;
        }
        return maxEval;
    }
    else{
        float minEval = INF;
        for(auto it = nextBoards.begin(); it != nextBoardsEnd; ++it){
            float eval = minimax_aux(*it, depth - (board.current_player != it->current_player ? 1 : 0), alpha, beta);
            if(minEval > eval){
                minEval = eval;
                move = (*it).last_move;
            }
            beta = (beta < eval) ? beta : eval;
            if(beta <= alpha)
                break;
        }
        auto entry = table.find(board);
        if(entry == table.end() || entry->second.depth < depth){
            Entry table_entry; 
            table_entry.eval = minEval;
            table_entry.depth = depth;
            table_entry.alpha = alpha;
            table_entry.beta = beta;
            table[board] = table_entry;
        }
        return minEval;
    }
}

float Minimax::minimax_aux(Board &board, unsigned short depth, float alpha, float beta){

    auto look = table.find(board);
    if(look != table.end() && look->second.depth >= depth){
        //cout<<"hit"<<endl;
        return look->second.eval;
    }
    
    if(depth == 0 || board.end_game(board.current_player))
        return board.eval();

    vector<Board> nextBoards = board.get_valid_boards(board.current_player);
    //sort(nextBoards.begin(),nextBoards.end(),compareBoard);

    auto nextBoardsEnd = nextBoards.end();
    if(board.current_player){
        float maxEval = -INF;
        for(auto it = nextBoards.begin(); it != nextBoardsEnd; ++it){
            float eval = minimax_aux(*it, depth - (board.current_player != it->current_player ? 1 : 0), alpha, beta);
            if(maxEval < eval){
                maxEval = eval;
            }
            alpha = (alpha > eval) ? alpha : eval;
            if(beta <= alpha)
                break;
        }
        auto entry = table.find(board);
        if(entry == table.end() || entry->second.depth < depth){
            Entry table_entry; 
            table_entry.eval = maxEval;
            table_entry.depth = depth;
            table_entry.alpha = alpha;
            table_entry.beta = beta;
            table[board] = table_entry;
        }
        return maxEval;
    }
    else{
        float minEval = INF;
        for(auto it = nextBoards.begin(); it != nextBoardsEnd; ++it){
            float eval = minimax_aux(*it, depth - (board.current_player != it->current_player ? 1 : 0), alpha, beta);
            if(minEval > eval){
                minEval = eval;
            }
            beta = (beta < eval) ? beta : eval;
            if(beta <= alpha)
                break;
        }
        auto entry = table.find(board);
        if(entry == table.end() || entry->second.depth < depth){
            Entry table_entry; 
            table_entry.eval = minEval;
            table_entry.depth = depth;
            table_entry.alpha = alpha;
            table_entry.beta = beta;
            table[board] = table_entry;
        }
        return minEval;
    }
}
