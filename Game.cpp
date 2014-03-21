#include "Game.h"

Game::Game() {
    this->grid_size = 4;
    this->initialize();
    move_counter = 0;
}

Game::Game(int grid_size) {
    this->grid_size = grid_size;
    this->initialize();
    move_counter = 0;
}

Game::~Game() {
    for (int row = 0; row < grid_size; row++)
	free(game_board[row]);
    free(game_board);
}

void Game::initialize() {
    game_board = (Tile**)malloc(sizeof(Tile*) * grid_size);
    for (int row = 0; row < grid_size; row++)
	game_board[row] = (Tile*)malloc(sizeof(Tile) * grid_size);
    for (int row = 0; row < grid_size; row++)
	for (int column = 0; column < grid_size; column++)
	{
	    game_board[row][column].value = 0;
	    game_board[row][column].color = 0;
	}

    int random1 = rand() % grid_size;
    int random2 = rand() % grid_size;
    game_board[random1][random2].value = 2;
}

void Game::reset_game()
{
    move_counter = 0;
    for (int row = 0; row < grid_size; row++)
	for (int column = 0; column < grid_size; column++)
	    game_board[row][column].value = 0;

    int random1 = rand()%grid_size;
    int random2 = rand()%grid_size;
    game_board[random1][random2].value = 2;
}

bool Game::is_game_won()
{
    for (int row = 0; row < grid_size; row++)
	for (int column = 0; column < grid_size; column++)
	    if (game_board[row][column].value >= 2048)
		return true;
    return false;
}


bool Game::is_game_over()
{
    //if there are any empty space, game is not over
    for (int row = 0; row < grid_size; row++)
	for (int column = 0; column < grid_size; column++)
	{
	    if (game_board[row][column].value == 0)
		return false;
	    else if (game_board[row][column].value >= 2048)
		return true;
        }

    //if any rows have adjacent pairs that match, game is not over
    for (int row = 0; row < grid_size; row++)
    {
        int temp = -1;
	for (int column = 0; column < grid_size; column++)
	{
	    if (temp < 0)
		temp = game_board[row][column].value;
	    else if(temp == game_board[row][column].value)
		return false;
	    else
		temp = game_board[row][column].value;
	}
    }

    //if any columns have adjacent pairs that match, game is not over
    for (int column = 0; column < grid_size; column++)
    {
        int temp = -1;
	for (int row = 0; row < grid_size; row++)
	{
	    if (temp < 0)
		temp = game_board[row][column].value;
	    else if(temp == game_board[row][column].value)
		return false;
	    else
		temp = game_board[row][column].value;
	}
    }

    //if we get here, there were no valid moves available
    return true;
}


void Game::execute_random_move() {

    int random = rand() % grid_size;
    execute_move(random);
}


void Game::execute_move(int move)
{
    //we need to check and track if we are preforming a legal move.
    //  There are cases where the game is not over, but certain
    //  moves are not allowed. To prevent spawning new tiles and
    //  incrementing the move counter in this scenario, we need to 
    //  check and track if a valid move was preformed.
    bool legal_move = false;

    //temporary vector to hold values from a single row or column
    std::vector<int> vector;
    
    //move up or down
    if (move == UP || move == DOWN)
    {
	//extract each row
	for (int column = 0; column < grid_size; column++)
	{
	    for (int row = 0; row < grid_size; row++)
	        vector.push_back(game_board[row][column].value);
	    
	    //if moving down, invert the vector
	    if (move == DOWN)
	        invert_vector(vector);

	    int index = 0;
	    bool found_empty_space = false;
	    
	    //check if a move is being made
	    //  move is made if we find an empty space followed
	    //  by a non empty space.
	    while (index < vector.size())
	    {
		if (!found_empty_space && vector[index] == 0)
		    found_empty_space = !found_empty_space;

		else if (found_empty_space && vector[index] != 0)
		{
		    legal_move = true;
		    break;
		}
		index++;		    
	    }
	   
	    remove_zero_entries(vector);
	    //perform the move for this column
	    index = 0;	    
	    while (index < vector.size() - 1)
	    {
	        if (vector[index] == vector[index + 1])
		{
		    vector[index] *= 2;
		    vector.erase(vector.begin() + index + 1);
		}
		index++;
	    }
	   
	    pad_with_zero(vector);

	    if (move == DOWN)
	        invert_vector(vector);

	    for (int row = 0; row < grid_size; row++)
		    game_board[row][column].value = vector[row];
	    
	    vector.clear();
	}
    }


    else //move == right or left
    {
	for (int row = 0; row < grid_size; row++)
	{
	    for (int column = 0; column < grid_size; column++)
	        vector.push_back(game_board[row][column].value);
	    if (move == RIGHT)
	        invert_vector(vector);
		 
	    int index = 0;
	    bool found_empty_space = false;

            //check if a move is being made
	    while (index < vector.size())
	    {
		if (!found_empty_space && vector[index] == 0)
		    found_empty_space = !found_empty_space;
		else if (found_empty_space && vector[index] != 0)
		{
		    legal_move = true;
		    break;
		}
		index++;		    
	    }

	    remove_zero_entries(vector);
	     
	    index = 0;	    
	   
	    while (index < vector.size() - 1)
	    {
	        if (vector[index] == vector[index + 1])
		{
		    vector[index] *= 2;
		    vector.erase(vector.begin() + index + 1);
		}
		index++;
	    }
	    
	    pad_with_zero(vector);
	    	    
	    if (move == RIGHT)
		invert_vector(vector);

	    for (int column = 0; column < grid_size; column++)
		    game_board[row][column].value = vector[column];
	    
	    vector.clear();
	}
    }

    if(legal_move)
    {
        add_new_tile();
	move_counter++;
    }
}

void Game::add_new_tile() {
        
	std::vector<std::pair<int,int> > blank_tiles;
        for (int row = 0; row < grid_size; row++)
	    for (int column = 0; column < grid_size; column++)
	        if (game_board[row][column].value == 0)
	   	    blank_tiles.push_back(std::make_pair(row, column));
        
	if (blank_tiles.size() > 0)
        {
            int random = rand() % blank_tiles.size();
	    game_board[blank_tiles[random].first][blank_tiles[random].second].value = 2;
        }
}

void Game::remove_zero_entries(std::vector<int> &vector)
{
    int index = 0;
    while (index < vector.size() - 1)
    {
	if (vector[index] == 0)
	    vector.erase(vector.begin() + index);
	else
	    index++;
    }
}

void Game::pad_with_zero(std::vector<int> &vector)
{
    while (vector.size() < grid_size)
	vector.push_back(0);
}

void Game::invert_vector(std::vector<int> &vector)
{
    std::reverse(vector.begin(), vector.end());
}

void Game::print_game_board(WINDOW* window)
{
    for (int row = 0; row < grid_size; row++)
	for (int column = 0; column < grid_size; column++)
	    print_tile(window, game_board[row][column], 2 + (row * 2), 3 + (column * 5));
}

void Game::print_tile(WINDOW* window, Tile tile, int x_coord, int y_coord)
{
    if (tile.value == 0)
	mvwprintw(window, x_coord, y_coord, "----");
    else if (tile.value < 10)
	mvwprintw(window, x_coord, y_coord, "%2d  ", tile.value);
    else if (tile.value < 100)
	mvwprintw(window, x_coord, y_coord, "%3d ", tile.value);
    else 
	mvwprintw(window, x_coord, y_coord, "%4d", tile.value);
}



