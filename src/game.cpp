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
    cout<<x_orig<<","<<y_orig<<" ->"<<x_dest<<","<<y_dest;
    if(t) cout<<"  "<< type;
}

void Move::set_vars(size_t x_o,size_t y_o,size_t x_d,size_t y_d){
    x_orig = x_o;
    y_orig = y_o;
    x_dest = x_d;
    y_dest = y_d;
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

void Board::get_valid_moves_aux(vector<Move> &moves, Move move, bool &capture, bool white_player){
    unsigned int r = valid_move_aux(move.x_orig, move.y_orig, move.x_dest, move.y_dest, white_player);
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

vector<Move> Board::get_valid_moves(bool white_player){
    vector<Move> moves = vector<Move>();
    ulong board;
    if(white_player) board = board_white;
    else if(!white_player) board = board_black;

    bool capture_possible = false;

    if(dropPiece == 0){
        size_t pos = 0;
        while(board != 0){
            bool piece = board & 1L;
            if(piece){
                size_t x = pos%BOARD_SIZE;
                size_t y = pos/BOARD_SIZE;
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
            ++pos;
            board = board >> 1;
        }
    }else{
        for(size_t y = 0; y < 2; ++y){
            for(size_t x = 1; x < BOARD_SIZE; ++x){
                if(white_player) get_valid_moves_aux(moves,Move(0,0,x, y + 6),capture_possible,true);
                if(!white_player) get_valid_moves_aux(moves,Move(0,0,x, y),capture_possible,false);
                
            }
        }
    }
    return moves;
}

vector<Board> Board::get_valid_boards(bool white_player){
    vector<Move> moves = get_valid_moves(white_player);
    vector<Board> boards = vector<Board> ();
    for(auto it = moves.begin(); it != moves.end(); ++it){
        Board temp = *this;
        Move m = *it;
        temp.move_piece(m.x_orig,m.y_orig,m.x_dest,m.y_dest,white_player,m.type);
        temp.last_move.set_vars(m.x_orig,m.y_orig,m.x_dest,m.y_dest);
        boards.push_back(temp);

    }

    return boards;
}

float Board::eval(){ // positive favours white, negative favours black

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
    vector<Move> moves = get_valid_moves(white_player);
    if(moves.size() == 0){ //TODO:: only need to check if threr is one move possible, not all
        //cout<<"No more possible moves for player"<<(current_player ? 1 : 2)<<endl;
        return true;
    }
    return false;
}

bool Board::end_game(bool white_player){
    if(board_white == 0 || board_black == 0){
        return true;
    }
    vector<Move> moves = get_valid_moves(white_player);
    if(moves.size() == 0){ //TODO:: only need to check if threr is one move possible, not all
        //cout<<"No more possible moves for player"<<(current_player ? 1 : 2)<<endl;
        return true;
    }
    return false;
}

bool Board::get_piece_white(size_t pos){
    if(pos>=BOARD_SIZE*BOARD_SIZE)
        return false;
    return (board_white >> pos) & 1L;
}
bool Board::get_piece_white(size_t x, size_t y){
    if(x>=BOARD_SIZE || y>=BOARD_SIZE)
        return false;
    return get_piece_white(y*BOARD_SIZE + x);
}
bool Board::get_piece_black(size_t pos){
    if(pos>=BOARD_SIZE*BOARD_SIZE)
        return false;
    return (board_black >> pos) & 1L;
}
bool Board::get_piece_black(size_t x, size_t y){
    if(x>=BOARD_SIZE || y>=BOARD_SIZE)
        return false;
    return get_piece_black(y*BOARD_SIZE + x);
}
bool Board::is_piece(size_t pos){
    if(pos>=BOARD_SIZE*BOARD_SIZE)
        return false;
    return get_piece_white(pos) || get_piece_black(pos);
}
bool Board::is_piece(size_t x, size_t y){
    if(x>=BOARD_SIZE || y>=BOARD_SIZE){
        return false;
    }
    return is_piece(y*BOARD_SIZE + x);
}
void Board::put_piece_white(size_t pos){
    if(pos>=BOARD_SIZE*BOARD_SIZE)
        return;
    board_white |= (1L << pos);
}
void Board::put_piece_white(size_t x, size_t y){
    if(x>=BOARD_SIZE || y>=BOARD_SIZE)
        return;
    put_piece_white(y*BOARD_SIZE + x);
}
bool Board::remove_piece_white(size_t pos){
    if(pos>=BOARD_SIZE*BOARD_SIZE || !get_piece_white(pos))
        return false;
    board_white = board_white & ~(1L << pos);
    return true;
}
bool Board::remove_piece_white(size_t x, size_t y){
    if(x>=BOARD_SIZE || y>=BOARD_SIZE)
        return false;
    return remove_piece_white(y*BOARD_SIZE + x);
}
void Board::put_piece_black(size_t pos){
    if(pos>=BOARD_SIZE*BOARD_SIZE)
        return;
    board_black |= (1L << pos);
}
void Board::put_piece_black(size_t x, size_t y){
    if(x>=BOARD_SIZE || y>=BOARD_SIZE)
        return;
    put_piece_black(y*BOARD_SIZE + x);
}
bool Board::remove_piece_black(size_t pos){
    if(pos>=BOARD_SIZE*BOARD_SIZE || !get_piece_black(pos))
        return false;
    board_black = board_black & ~(1L << pos);
    return true;
}
bool Board::remove_piece_black(size_t x, size_t y){
    if(x>=BOARD_SIZE || y>=BOARD_SIZE)
        return false;
    return remove_piece_black(y*BOARD_SIZE + x);
}


//CHECK if OKAY
bool Board::is_last_white(size_t x, size_t y){
    return y == 0;
}
bool Board::is_last_black(size_t x, size_t y){
    return y == BOARD_SIZE-1;
}
bool Board::is_drop_zone_white(size_t x, size_t y){
    return (y == BOARD_SIZE -1 || y == BOARD_SIZE - 2) && (x != 0 && x != BOARD_SIZE - 1) ;
}
bool Board::is_drop_zone_black(size_t x, size_t y){
    return (y == 0 || y == 1) && (x != 0 && x != BOARD_SIZE - 1) ;
}


bool Board::move_piece(size_t x_orig, size_t y_orig,size_t x_dest, size_t y_dest, bool white_player, int valid = 0){
    int is_valid;
    if(valid == 0)
        is_valid = valid_move(x_orig,y_orig,x_dest,y_dest,white_player);
    else 
        is_valid = valid;
    if(is_valid == false)
        return false;
    if(!(is_valid == DROP))  {
        if(white_player)
            move_piece_white(x_orig,y_orig,x_dest,y_dest);
        else if(!white_player)
            move_piece_black(x_orig,y_orig,x_dest,y_dest);
        
        if(is_valid == ORDINARY_MOVE){ //NORMAL MOVE
            if((white_player && is_last_white(x_dest,y_dest)) || (!white_player && is_last_black(x_dest,y_dest))){
                dropPiece = 2;
                if(white_player) remove_piece_white(x_dest,y_dest);
                else if(!white_player) remove_piece_black(x_dest,y_dest);
                vector<Move> moves = get_valid_moves(white_player);
                if(moves.empty()){
                    dropPiece = 0;
                    current_player = !current_player;
                    jumpingMove = -1;
                    capturingMove = -1;
                }
            }else{
                current_player = !current_player;
                jumpingMove = -1;
                capturingMove = -1;
            }
        }else if(is_valid == JUMPING_MOVE){ //JUMPING MOVE

            if((white_player && is_last_white(x_dest,y_dest)) || (!white_player && is_last_black(x_dest,y_dest))){
                dropPiece = 2;
                if(white_player) remove_piece_white(x_dest,y_dest);
                else if(!white_player) remove_piece_black(x_dest,y_dest);
                vector<Move> moves = get_valid_moves(white_player);
                if(moves.empty()){
                    dropPiece = 0;
                    current_player = !current_player;
                    jumpingMove = -1;
                    capturingMove = -1;
                }
            }else{
                jumpingMove = y_dest * BOARD_SIZE + x_dest;
                vector <Move> moves = get_valid_moves(white_player);
                if(moves.size() == 0){
                    current_player = !current_player;
                    jumpingMove = -1;
                    capturingMove = -1;
                }
            }



        }else if(is_valid == CAPTURE){ //CAPTURING
            if(white_player) remove_piece_black((x_orig + x_dest)/2 , (y_orig + y_dest)/2);
            else if(!white_player) remove_piece_white((x_orig + x_dest)/2 , (y_orig + y_dest)/2);


            capturingMove = y_dest * BOARD_SIZE + x_dest; 
            vector <Move> moves = get_valid_moves(white_player);
            if(moves.empty()){

                if((white_player && is_last_white(x_dest,y_dest)) || (!white_player && is_last_black(x_dest,y_dest))){
                    dropPiece = 2;
                    if(white_player) remove_piece_white(x_dest,y_dest);
                    else if(!white_player) remove_piece_black(x_dest,y_dest);
                    vector<Move> moves = get_valid_moves(white_player);
                    if(moves.empty()){
                        dropPiece = 0;
                        current_player = !current_player;
                        jumpingMove = -1;
                        capturingMove = -1;
                    }
                }else{
                    current_player = !current_player;
                    jumpingMove = -1;
                    capturingMove = -1;
                }
            }
        }
    }else { // DROP PIECE
        if(white_player) put_piece_white(x_dest, y_dest);
        else if(!white_player) put_piece_black(x_dest, y_dest);
        dropPiece--;
        if (dropPiece > 0) {
            vector <Move> moves = get_valid_moves(white_player);
            if (moves.empty()) {
                current_player = !current_player;
                jumpingMove = -1;
                capturingMove = -1;
                dropPiece = 0;
            }
        }
        else {
            current_player = !current_player;
            jumpingMove = -1;
            capturingMove = -1;
            dropPiece = 0;
        }

    }
    
    return true;
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
    vector<Move> moves = get_valid_moves(white_player);
    for(auto it = moves.begin(); it != moves.end();++it){
        Move m = *it;
        if(m.x_orig == x_orig && m.y_orig == y_orig && m.x_dest == x_dest && m.y_dest == y_dest)
            return m.type;
    }
    return 0;
}

unsigned int Board::valid_move_aux(size_t x_orig, size_t y_orig,size_t x_dest, size_t y_dest, bool white_player){
    
    //não pode ser maior que o tabuleiro
    if(x_dest >= BOARD_SIZE || y_dest >=BOARD_SIZE)
        return false;
    //se houver peça na posição de destino,
    if(is_piece(x_dest,y_dest)){
        return false;
    }
    if(dropPiece == 0){
        //se nao houver peça daquele jogador na posição de origem
        if((white_player && !get_piece_white(x_orig,y_orig)) || (!white_player && !get_piece_black(x_orig,y_orig)))
            return false;
        if(jumpingMove != -1){
            //cout<<"tenho que saltar "<<x_orig<<","<<y_orig<<"     "<<white_player<<endl;
            if(int(y_orig*BOARD_SIZE+x_orig) != jumpingMove)
                return false;
            //cout<<"passei"<<endl;
        }
        else if(capturingMove != -1){
            if(int(y_orig*BOARD_SIZE+x_orig) != capturingMove)
                return false;
        }

        int x_delta = x_dest - x_orig;
        int y_delta = y_dest - y_orig;

        //se não houver peça na posição de destino
        if((x_delta == -1 || x_delta == 0 || x_delta == 1) && (y_delta == -1 || y_delta == 1) && jumpingMove == -1 && capturingMove == -1){
            //é possivel fazer um ORDINARY MOVE
            if((white_player && y_delta == -1) || (!white_player && y_delta == 1))
                return ORDINARY_MOVE; //ORDINARY MOVE
        }
        else if((x_delta == -2 || x_delta == 0 || x_delta == 2)){
            int mid_x=x_orig+x_delta/2;
            int mid_y=y_orig+y_delta/2;
            // JUMPING MOVE ou alguns casos do CAPTURE
            if(y_delta == -2 || y_delta == 2){
                if(white_player && y_delta == -2){
                    if(get_piece_white(mid_x,mid_y) && capturingMove == -1)
                        return JUMPING_MOVE; //JUMP
                    else if(get_piece_black(mid_x,mid_y) && jumpingMove == -1)
                        return CAPTURE; //CAPTURE
                }
                else if(!white_player && y_delta == 2){
                    if(get_piece_black(mid_x,mid_y) && capturingMove == -1)
                        return JUMPING_MOVE; //JUMP
                    else if(get_piece_white(mid_x,mid_y) && jumpingMove == -1)
                        return CAPTURE; //CAPTURE
                }
            }else if(y_delta == 0 && x_delta != 0 && jumpingMove == -1){
                if((white_player && get_piece_black(mid_x,mid_y)) || (!white_player && get_piece_white(mid_x,mid_y)) )
                    return CAPTURE; //CAPTURE
            }
        }
    }else{
        if(white_player && is_drop_zone_white(x_dest,y_dest))
            return DROP;
        if(!white_player && is_drop_zone_black(x_dest,y_dest))
            return DROP;
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

Game::Game(int player1Mode, int player2Mode) : player1(player1Mode) , player2(player2Mode){
    board = Board();
    minimax = Minimax();
}

bool Game::make_move(size_t x_orig, size_t y_orig,size_t x_dest, size_t y_dest, bool white_player){
    return board.move_piece(x_orig,y_orig,x_dest,y_dest,board.current_player);
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
        cout<<m.x_orig<<","<<m.y_orig<<"->"<<m.x_dest<<","<<m.y_dest<<"   ";
    }
    cout<<endl;

    size_t x_orig=0,y_orig=0,x_dest,y_dest;
    if (board.dropPiece == 0) {
        cout<<"Origin x: "; cin>>x_orig;
        cout<<"Origin Y: "; cin>>y_orig;
    }

    cout<<"Destination x: "; cin>>x_dest;
    cout<<"Destination Y: "; cin>>y_dest;
    bool move = make_move(x_orig,y_orig,x_dest,y_dest,board.current_player);
    if(!move)
        cout<<"Invalid move, try another one"<<endl;
}

void Game::get_move_ai1(){ //makes first move available
    vector<Move> moves = board.get_valid_moves(board.current_player);
    Move m = moves[0];
    m.display();cout<<endl;
    make_move(m.x_orig,m.y_orig,m.x_dest,m.y_dest,board.current_player);
}

void Game::get_move_ai2(){ //makes first move available
    Move m;
    cout<<"Start minimax"<<endl;
    float alpha = -INF, beta = INF;
    cout<<"EVAL: \n"<<minimax.minimax(board,2,alpha,beta,1,m)<<endl;
    cout<<"End minimax"<<endl;
    m.display();cout<<endl;
    make_move(m.x_orig,m.y_orig,m.x_dest,m.y_dest,board.current_player);
}

void Game::get_move_ai3(){ //makes first move available
    Move m;
    cout<<"Start minimax"<<endl;
    float alpha = -INF, beta = INF;
    cout<<"EVAL: \n"<<minimax.minimax(board,4,alpha,beta,1,m)<<endl;
    cout<<"End minimax"<<endl;
    m.display();cout<<endl;
    make_move(m.x_orig,m.y_orig,m.x_dest,m.y_dest,board.current_player);
}

void Game::game_loop(){

    display(); 

    while (true)
    {   
            /*cout<<board.board_white<<endl;cout<<board.board_black<<endl;cout<<board.current_player<<endl;cout<<board.jumpingMove<<endl;cout<<board.capturingMove<<endl;*/
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


bool compareBoard(Board b1, Board b2 ){
    float v1 = b1.eval();
    float v2 = b2.eval();
    return (v1 < v2);
}


Minimax::Minimax(){

}

float Minimax::minimax(Board board, unsigned short depth, float alpha, float beta, bool maximizingPlayer, Move &move){
    
    if(depth == 0 || board.end_game(board.current_player))
        return board.eval();

    vector<Board> nextBoards = board.get_valid_boards(board.current_player);
    sort(nextBoards.begin(),nextBoards.end(),compareBoard);
    
    Move temp_move;

    auto nextBoardsEnd = nextBoards.end();
    if(maximizingPlayer == board.current_player){
        float maxEval = -INF;
        for(auto it = nextBoards.begin(); it != nextBoardsEnd; ++it){
            float eval = minimax_aux(*it, depth-1, alpha, beta, maximizingPlayer);
            if(maxEval < eval){
                maxEval = eval;
                move = (*it).last_move;
            }
            alpha = (alpha > eval) ? alpha : eval;
            if(beta <= alpha)
                break;
        }
        return maxEval;
    }
    else{
        float minEval = INF;
        for(auto it = nextBoards.begin(); it != nextBoardsEnd; ++it){
            float eval = minimax_aux(*it, depth-1, alpha, beta, maximizingPlayer);
            if(minEval > eval){
                minEval = eval;
                move = (*it).last_move;
            }
            beta = (beta < eval) ? beta : eval;
            if(beta <= alpha)
                break;
        }
        return minEval;
    }
}

float Minimax::minimax_aux(Board board, unsigned short depth, float alpha, float beta, bool maximizingPlayer){
    
    if(depth == 0 || board.end_game(board.current_player))
        return board.eval();

    vector<Board> nextBoards = board.get_valid_boards(board.current_player);
    sort(nextBoards.begin(),nextBoards.end(),compareBoard);

    auto nextBoardsEnd = nextBoards.end();
    if(maximizingPlayer == board.current_player){
        float maxEval = -INF;
        for(auto it = nextBoards.begin(); it != nextBoardsEnd; ++it){
            float eval = minimax_aux(*it, depth-1, alpha, beta, maximizingPlayer);
            if(maxEval < eval){
                maxEval = eval;
            }
            alpha = (alpha > eval) ? alpha : eval;
            if(beta <= alpha)
                break;
        }
        return maxEval;
    }
    else{
        float minEval = INF;
        for(auto it = nextBoards.begin(); it != nextBoardsEnd; ++it){
            float eval = minimax_aux(*it, depth-1, alpha, beta, maximizingPlayer);
            if(minEval > eval){
                minEval = eval;
            }
            beta = (beta < eval) ? beta : eval;
            if(beta <= alpha)
                break;
        }
        return minEval;
    }
}
