#include <iostream>
#include <vector>

#define BOARD_SIZE 8

using namespace std;

class Board{

private:

    int board[BOARD_SIZE*BOARD_SIZE];

public:

    Board();

    void make_move(/*decide parameters*/);
    vector <Board> get_valid_moves(int current_player);

    bool gameover(int current_player);

    float evaluate_board1();
    float evaluate_board2();
    float evaluate_board3();

    void display();

};

class Game{

private:

    Board board;
    int current_player; //player 1 or 2
    int player1 , player2; // 0 - Human;    1 - CPU level 1;    2 - CPU level 2;    3 - CPU level 3;    diferent minimax aproaches, depth, prunning, optimizations

    int movement_phase; // for moves that is needed more than one phase, placing two pieces, deciding where to eat next, and so on

    unsigned int move_count;
    

    void find_best_move();

public:

    Game();

    void get_move();
    void make_move();

    void display();
    

};


/**
 * @brief Minimax class, to calculate de best move for a given board
 * 
 */

class Minimax{

public:

    Minimax();

    float minimax(Board board, unsigned short depth, float alpha, float beta, bool maximizingPlayer);

};