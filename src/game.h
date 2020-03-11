#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>

#define BOARD_SIZE 8
#define INF 1000000

#define ORDINARY_MOVE 1
#define JUMPING_MOVE 2
#define CAPTURE 3
#define DROP 4

using namespace std;

class Move{
public:
    u_char x_orig;
    u_char y_orig;
    u_char x_dest;
    u_char y_dest;
    u_char type = 0;
    Move(): x_orig(0), y_orig(0), x_dest(0), y_dest(0) {};
    Move(u_char x_o,u_char y_o,u_char x_d,u_char y_d) : x_orig(x_o), y_orig(y_o), x_dest(x_d), y_dest(y_d) {};

    void set_vars(u_char x_o,u_char y_o,u_char x_d,u_char y_d);

    void display(bool t);

    bool operator == (Move const & m2);

};
 

class Board{

public: //private:

    //ulong board_white = 9110782265213845505;
    //ulong board_black = 33552384;
    ulong board_white = 9114834846030495744L;
    //ulong board_white = 9097946347427856640L;

    ulong board_black = 6717054L;
    int jumpingMove = -1; //-1 :  false, outro numero: posiçao da peça
    int capturingMove = -1; //-1 :  false, outro numero: posiçao da peça
    int dropPiece = 0;

    Move last_move;

    float evaluate_board1();
    float evaluate_board2();
    float evaluate_board3();

    bool get_piece_white(u_char pos);
    bool get_piece_white(u_char x, u_char y);
    bool get_piece_black(u_char pos);
    bool get_piece_black(u_char x, u_char y);
    bool is_piece(u_char pos);
    bool is_piece(u_char x, u_char y);

    void put_piece_white(u_char pos);
    void put_piece_white(u_char x, u_char y);
    bool remove_piece_white(u_char pos);
    bool remove_piece_white(u_char x, u_char y);
    void put_piece_black(u_char pos);
    void put_piece_black(u_char x, u_char y);
    bool remove_piece_black(u_char pos);
    bool remove_piece_black(u_char x, u_char y);

    void move_piece_white(u_char pos_orig, u_char pos_dest);
    void move_piece_black(u_char pos_orig, u_char pos_dest);
    void move_piece_white(u_char x_orig, u_char y_orig,u_char x_dest, u_char y_dest);
    void move_piece_black(u_char x_orig, u_char y_orig,u_char x_dest, u_char y_dest);

    void get_valid_moves_aux(vector<Move> &moves, Move move, bool &capture, bool white_player);

    unsigned int valid_move(Move move, bool white_player);
    unsigned int valid_move_aux(const Move &move, bool white_player);

    bool is_last_white(u_char x, u_char y);
    bool is_last_black(u_char x, u_char y);

    bool is_drop_zone_white(u_char x, u_char y);
    bool is_drop_zone_black(u_char x, u_char y);

    bool operator==(const Board &b2) const;

public:

    bool current_player = 1;

    Board();
    Board(const Board &old);

    bool move_piece(const Move &move, bool white_player, int valid = false);
    
    vector <Move> get_valid_moves(bool white_player);
    vector <Board> get_valid_boards(bool white_player);
    bool any_move_available(bool white_player);

    bool gameover(bool white_player);
    bool end_game(bool white_player);

    float eval() const;

    void display();

};

/**
 * @brief Minimax class, to calculate de best move for a given board
 * 
 */


struct KeyHasher{

    std::size_t operator()(const Board& k) const
    {
        using std::size_t;
        using std::hash;

        return hash<ulong>()(k.board_white ^ (k.board_black  << 1) >> 1) ^ k.current_player ^ ((k.capturingMove << 2) ^ k.jumpingMove) >> k.dropPiece; 
    }

    bool operator() (const Board& k1, const Board& k2) const{
        return k1 == k2;
    } 

};

struct Entry{
    float eval;
    size_t depth;
    float alpha;
    float beta; 
};

class Minimax{
private:

    std::unordered_map<Board,Entry,KeyHasher> table;

    static bool compareBoards(Board b1, Board b2 );


public:

    size_t cuts;

    Minimax();

    float minimax(Board &board, unsigned short depth, float alpha, float beta,Move &move);
    float minimax_aux(Board &board, unsigned short depth, float alpha, float beta);

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
    void get_move_ai3();
    bool make_move(Move move, bool white_player);

    void game_loop();

    void display();    

};

