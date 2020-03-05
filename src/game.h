#include <iostream>
#include <vector>

#define BOARD_SIZE 8
#define INF 1000000

#define ORDINARY_MOVE 1
#define JUMPING_MOVE 2
#define CAPTURE 3

using namespace std;

class Move{
public:
    size_t x_orig;
    size_t y_orig;
    size_t x_dest;
    size_t y_dest;
    unsigned int type = 0;
    Move(): x_orig(0), y_orig(0), x_dest(0), y_dest(0) {};
    Move(size_t x_o,size_t y_o,size_t x_d,size_t y_d) : x_orig(x_o), y_orig(y_o), x_dest(x_d), y_dest(y_d) {};

    void display(bool t);
};


class Board{

public: //private:

    //ulong board_white = 9110782265213845505;
    //ulong board_black = 33552384;
    ulong board_white = 9114834846030495744L;
    ulong board_black = 6717054L;
    int jumpingMove = -1; //-1 :  false, outro numero: posiçao da peça
    int capturingMove = -1; //-1 :  false, outro numero: posiçao da peça

    float evaluate_board1();
    float evaluate_board2();
    float evaluate_board3();

    bool get_piece_white(size_t pos);
    bool get_piece_white(size_t x, size_t y);
    bool get_piece_black(size_t pos);
    bool get_piece_black(size_t x, size_t y);
    bool is_piece(size_t pos);
    bool is_piece(size_t x, size_t y);

    void put_piece_white(size_t pos);
    void put_piece_white(size_t x, size_t y);
    bool remove_piece_white(size_t pos);
    bool remove_piece_white(size_t x, size_t y);
    void put_piece_black(size_t pos);
    void put_piece_black(size_t x, size_t y);
    bool remove_piece_black(size_t pos);
    bool remove_piece_black(size_t x, size_t y);

    void move_piece_white(size_t pos_orig, size_t pos_dest);
    void move_piece_black(size_t pos_orig, size_t pos_dest);
    void move_piece_white(size_t x_orig, size_t y_orig,size_t x_dest, size_t y_dest);
    void move_piece_black(size_t x_orig, size_t y_orig,size_t x_dest, size_t y_dest);

    void get_valid_moves_aux(vector<Move> &moves, Move move, bool &capture, bool white_player);

    unsigned int valid_move(size_t x_orig, size_t y_orig,size_t x_dest, size_t y_dest, bool white_player);
    unsigned int valid_move_aux(size_t x_orig, size_t y_orig,size_t x_dest, size_t y_dest, bool white_player);

public:

    bool current_player = 1;

    Board();
    Board(const Board &old);

    bool move_piece(size_t x_orig, size_t y_orig,size_t x_dest, size_t y_dest, bool white_player);
    
    vector <Move> get_valid_moves(bool white_player);
    vector <Board> get_valid_boards(bool white_player);

    bool gameover(bool white_player);
    bool end_game(bool white_player);

    float eval();

    void display();

};

/**
 * @brief Minimax class, to calculate de best move for a given board
 * 
 */

class Minimax{

public:

    Minimax();

    float minimax(Board board, unsigned short depth, float alpha, float beta, bool maximizingPlayer,Move &move);

};

class Game{

private:

    //bool white_player = 1; //player white or black, player 1 or 2
    int player1 , player2; // 0 - Human;    1 - CPU level 1;    2 - CPU level 2;    3 - CPU level 3;    diferent minimax aproaches, depth, prunning, optimizations

    //int movement_phase; // for moves that is needed more than one phase, placing two pieces, deciding where to eat next, and so on

    //unsigned int move_count;

    void find_best_move();

public:

    Board board;
    Minimax minimax;

    Game(int player1Mode, int player2Mode);

    void get_move();
    void get_move_human();
    void get_move_ai1();
    void get_move_ai2();
    bool make_move(size_t x_orig, size_t y_orig,size_t x_dest, size_t y_dest, bool white_player);

    void game_loop();

    void display();    

};

