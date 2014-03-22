/*
 * Game.h
 *
 * Author: Michael Denny
 *
 * This class implements a slightly customizable "2048" clone.
 * 2048 is a popular game, you can google it for more information.
 * This version allows for a varying size of square playing grid.
 * The standard size is 4x4.
 *
 */


#include "Game.h"

//Default constructor. Creates game with 4x4 grid playing area.
Game::Game() {
    this->grid_size = 4;
    this->initialize();
    move_counter = 0;
}

//Constructor which lets you set the grid size to an arbitrary int.
Game::Game(int input_grid_size) {

    //ensure we get a sensible grid_size as input
    if (input_grid_size <= 1)
	input_grid_size = 4;  //fallback to default if needed

    this->grid_size = grid_size;
    this->initialize();
    move_counter = 0;
}

//Destructor
//  The game board is of type Tile**, a 2D array of pointers to Tile objects.
//  We need to iterate through the Tile** and free all the Tile*'s.
//  Finally, we free the Tile**.
Game::~Game() {
    for (int row = 0; row < grid_size; row++)
	free(game_board[row]);
    free(game_board);
}


//Initialize the game
//  The game board a 2D array of pinters to Tile objects. A Tile is a
//    struct which is defined in Game.h. We need to malloc space for
//    our Tile** gameboard. While we are at it, we need to initialize
//    the data members for all the Tile objects.
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

    //The game board has been created and is empty. We need to
    //  populate the first non-empty Tile with '2'. We choose
    //  a random Tile to start off the game.
    int random1 = rand() % grid_size;
    int random2 = rand() % grid_size;
    game_board[random1][random2].value = 2;
}


//Reset the game
//  This function resets the Game object to its starting state.
//    This means that all the Tiles are "emptied" (set to zero) and
//    a random Tile is choosen to begin the next game.
//  After calling reset_game(), the Game object will be in a
//    similiar state as it was just after initialize() was called.
void Game::reset_game()
{
    //reset move counter
    move_counter = 0;

    //Zero out all the Tiles
    for (int row = 0; row < grid_size; row++)
	for (int column = 0; column < grid_size; column++)
	    game_board[row][column].value = 0;

    //Select random tile to start the next game with
    int random1 = rand()%grid_size;
    int random2 = rand()%grid_size;
    game_board[random1][random2].value = 2;
}


//Check if the game is won.
//  This is typically called after is_game_over() returns true.
//  We iterate through each Tile, checking their values. 
//  If anyone of them is 2048 or larger (should be impossible to
//    be larger), then the game is won and we return true. 
//  If we get through all the Tiles without seeing 2048, then the
//    game was lost and we return false.
bool Game::is_game_won()
{
    for (int row = 0; row < grid_size; row++)
	for (int column = 0; column < grid_size; column++)
	    if (game_board[row][column].value >= 2048)
		return true;
    return false;
}


//Check if the game is over
//  This function returns whether or not the game is over.
//  We know the game is not over if either of the two
//    following conditions are met:
//    *There is an emtpy Tile/space
//    *There are two adjacent (non-diagonal) Tiles with
//       the same value
//  We know that the game is over if neither of those conditions
//    are met. But was also know the game is over if we find a
//    Tile with 2048. So we check for that when we check for
//    empty spaces.
bool Game::is_game_over()
{
    //If there are any empty space, game is not over.
    //If there are any 2048 spaces, the game is over.
    for (int row = 0; row < grid_size; row++)
	for (int column = 0; column < grid_size; column++)
	{
	    if (game_board[row][column].value == 0)
		return false;
	    else if (game_board[row][column].value >= 2048)
		return true;
        }


    //If any rows have adjacent pairs that match, game is not over
    for (int row = 0; row < grid_size; row++)  //for each row...
    {
	//initialize previous value with the first element in the row
        int previous = game_board[row][0];

	//Iterate through the rest of the row, comparing current value with
	//  the previous value. Return true if we have a match. Update
	//  previous if we do not.
	for (int row_element = 1; row_element < grid_size; row_element++)
	{
	    if(previous == game_board[row][row_element].value) //if previous == current
		return false;
	    else					       //else update previous
		previous = game_board[row][row_element].value;
	}
    }

    //If any columns have adjacent pairs that match, game is not over
    for (int column = 0; column < grid_size; column++)  //for each column...
    {
	//initialize previous value with the first element in the row
        previous = game_board[0][column];

	//Iterate through the rest of the row, comparing current value with
	//  the previous value. Return true if we have a match. Update
	//  previous if we do not.
	for (int column_element = 1; column_element < grid_size; column_element++)
	{
	    if(previous == game_board[column_element][column].value) 	//if (previous == current) return false
		return false;
	    else					       		//else update previous
		previous = game_board[column_element][column].value;
	}
    }

    //If we get here, there were no valid moves available. So the game is over.
    return true;
}


//Execute random move
//  Get random int in range [0,3].
//  Call execute_move() with that random number.
void Game::execute_random_move()
{
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
