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
        int previous = game_board[row][0].value;

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
        int previous = game_board[0][column].value;

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


//Execute move
//  This is the heart of the Game, executing a move.
//  This function accepts 4 different moves (as ints, see the move enum)
//  If the move is valid, the move is executed and another random empty
//    tile is chosen to continue the game (add a new "2" tile). Also, the
//    move counter is updated.
//  If the move is not valid, nothing is done and the move counter is not
//    incremented
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
    

    //Move logic:
    //  Lets say we are moving Left:
    //  Now lets start with the first row:
    //  Pull each value of the row out, and store into a vector.
    //  The move will collapse the values, meaning that all tiles
    //    that are empty will disapper (or be moved to the end, however
    //    you want to think about it)
    //  The move will also coalesce adjacent values if they are identical.
    //  So...
    //    1.) For each row, pull values out into a vector
    //    2.) Remove all the zeros from that vector
    //    3.) Traverse vector, coalescing same values into a single tile (of 2* value)
    //    4.) Pad the end of the vector with zeros, so it is the correct size for a row
    //    5.) Write out of the values of the vector back into the row
    //
    //  For a move to the right, we just invert the vector before and after we modify it.
    //
    //  For up and down moves, we use columns instead of rows (and do inversions on down)


    //Move up or down
    if (move == UP || move == DOWN)
    {
	//Extract each column into a vector
	for (int column = 0; column < grid_size; column++)
	{
	    for (int column_element = 0; column_element < grid_size; column_element++)
	        vector.push_back(game_board[column_element][column].value);  //extract tile values
	    
	    //If moving down, invert the vector
	    if (move == DOWN)
	        invert_vector(vector);

	    
	    //Iterate through vector to Check if a valid move is being made
	    int index = 0;
	    bool found_empty_space = false;
	    while (index < vector.size())
	    {
		//  Move is made if we find an empty space followed
		//    by a non empty space.
		if (!found_empty_space && vector[index] == 0)
		    found_empty_space = !found_empty_space;

		else if (found_empty_space && vector[index] != 0)
		{
		    legal_move = true;
		    break;
		}
		index++;		    
	    }
	   
	    //Remove the empty tiles from the vector
	    remove_zero_entries(vector);
	    
	    //Perform the move for this column by coalescing adjacent identical
	    //  values
	    index = 0;	    
	    while (index < vector.size() - 1)
	    {
	        if (vector[index] == vector[index + 1])  	//if the next value matchs this value
		{
		    vector[index] *= 2;				//double this value
		    vector.erase(vector.begin() + index + 1);   //remove the next value
		}
		index++;
	    }
	   
	    //After the move has been made, pad the vector with empty tiles
	    //  to match the length of the column
	    pad_with_zero(vector);

	    //If moving down, re-invert the vector
	    if (move == DOWN)
	        invert_vector(vector);

	    //write the values in the vector back into the column they originated from
	    for (int element = 0; element < grid_size; element++)
		    game_board[element][column].value = vector[element];
	    
	    //clean up before we start the next column
	    vector.clear();
	}
    }


    else //move == right or left
    {
	//Extract values of each row out into a vector (one row at a time)
	for (int row = 0; row < grid_size; row++)
	{
	    for (int row_element = 0; row_element < grid_size; row_element++)
	        vector.push_back(game_board[row][row_element].value);

	    //If we are moving right, invert the vector before we begin
	    if (move == RIGHT)
	        invert_vector(vector);
		 
	   
	    //Iterate through the vector and check if a valid move is being made 
	    int index = 0;
	    bool found_empty_space = false;
	    while (index < vector.size())
	    {
		//  Move is made if we find an empty space followed
		//    by a non empty space.
		if (!found_empty_space && vector[index] == 0)
		    found_empty_space = !found_empty_space;

		else if (found_empty_space && vector[index] != 0)
		{
		    legal_move = true;
		    break;
		}

		index++;		    
	    }


	    //Remove empty tiles from the vector
	    remove_zero_entries(vector);
	     
	 	
	   
	    //Perform the move for this column by coalescing adjacent identical
	    //  values
	    index = 0;	    
	    while (index < vector.size() - 1)
	    {
	        if (vector[index] == vector[index + 1])		//If this value equals the next value
		{
		    vector[index] *= 2;				//Double this value
		    vector.erase(vector.begin() + index + 1);	//delete next value
		}
		index++;
	    }
	    
	    //Pad the vector with empty tiles in order to make the vector the same
	    //  size as a row
	    pad_with_zero(vector);
	    	    

	    //If we are moving right, re-invert the vector
	    if (move == RIGHT)
		invert_vector(vector);

	    //Write the values of the vector back out into the row the values originated from
	    for (int element = 0; element < grid_size; element++)
		    game_board[row][element].value = vector[element];
	    
	    //Clean up vector to prepare for use with the next row
	    vector.clear();
	}
    }


    //If we determined that a valid, legal move was preformed, add a new tile
    //  to continue the game, and increment the move counter.
    if(legal_move)
    {
        add_new_tile();
	move_counter++;
    }
}


//Add new tile
//  After every move, a random empty tile is chosen and its value is
//  set at "2"
void Game::add_new_tile() {
        
	//Locations of empty tiles are stored in a vector of <int,int> pairs
	std::vector<std::pair<int,int> > blank_tiles;

	//Iterate through each tile, storing the locations of any empty tiles we find
        for (int row = 0; row < grid_size; row++)
	    for (int column = 0; column < grid_size; column++)
	        if (game_board[row][column].value == 0)			//If empty
	   	    blank_tiles.push_back(std::make_pair(row, column)); //Save location
        

	//Pick a random entry from the vector. Update the blank tile at that loation by
	//  setting its value at "2"
	if (blank_tiles.size() > 0)
        {
            int random = rand() % blank_tiles.size();
	    game_board[blank_tiles[random].first][blank_tiles[random].second].value = 2;
        }
}



//Remove zero entries from a given vector (by reference)
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


//Pad a give vector (by reference) with zeros until it is the same size
//  as this.grid_size
void Game::pad_with_zero(std::vector<int> &vector)
{
    while (vector.size() < grid_size)
	vector.push_back(0);
}


//Invert a given vector (by reference)
void Game::invert_vector(std::vector<int> &vector)
{
    std::reverse(vector.begin(), vector.end());
}


//Print the game board to a given ncurses window (by reference)
void Game::print_game_board(WINDOW* window)
{
    //For each tile, print the tile to the ncurses window
    for (int row = 0; row < grid_size; row++)
	for (int column = 0; column < grid_size; column++)
	    print_tile(window, game_board[row][column], 2 + (row * 2), 3 + (column * 5));
}


//Print a specific tile to a given ncurses window (by reference)
//  Tile is printed to window location (x_coord,y_coord)
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
