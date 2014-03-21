#ifndef __Game_h__
#define __Game_h__

#include <time.h>
#include <stdlib.h>
#include <algorithm>
#include <vector>
#include "ncurses.h"

struct Tile {
    int value;
    int color; //not implemented yet
};

enum Moves {UP, DOWN, LEFT, RIGHT, NONE};

class Game {

    public:
        Game();
	Game(int grid_size);
	~Game();
	int get_move_count() {return move_counter;}
	void reset_game();
	bool is_game_over();
	bool is_game_won();
	void execute_move(int move);
	void execute_random_move();
	void print_game_board(WINDOW* window);

    private:
        Tile** game_board;
	int grid_size;
	int move_counter;
	void initialize();
	void remove_zero_entries(std::vector<int> &vector);
	void pad_with_zero(std::vector<int> & vector);
	void invert_vector(std::vector<int> & vector);
	void add_new_tile();
	void print_tile(WINDOW* window, Tile tile, int x_coord, int y_coord);
};


#endif
