/*
 * 2048 Solver
 *
 * Author: Michael Denny (denny902 - at - gmail - dot - com)
 *
 * This is the main program. It contains the menus and looping logic.
 * The display is implemented using ncurses. The game consists of a
 * "Game" object.
 * 
 * You can change the size of the grid by modifying the "grid_size"
 * variable. I might incorporate this into an interaction user selection
 * in the future. But probably not, because this program has already
 * answered the questions I had about the 2048 game.
 *
 * Also, there is code to allow for user input to play the game. It
 * is currently commented out. Just uncomment that code, and commit the
 * random input code and you'll have a functional 2048 CLI clone.
 *
 * Feel free to do whatever you want with this. It was just a weekend
 * curiosity. I will probably never touch it again.
 *
 */

#include "ncurses.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "Game.h"


int main(int argc, char** argv)
{
    srand(time(NULL));  //Seed rand()

    int grid_size = 4;  //Size of the playing grid

    Game* my_game = new Game(grid_size);  //Game object
    
    //Some variables
    int terminal_height,        //Size of Terminal window: rows
        terminal_width,		//Size of Terminal window: columns
	min_req_height,		//Minimum height (rows) needed to draw game
	min_req_width,		//Minimum width (columns) needed to draw game
	my_game_area_height,    //Size of playing area: depends on grid size
	my_game_area_width,	//Size of playing area: depends on grid size
	move_count,		//Track number of moves for current game
	last_move,              //Last user selected input *implemented and disabled*	
	my_game_counter;	//Track number of games played


    //Some static strings used for the program
    const char* title = "2048 Tester - Michael Denny";
    const char* count_string = "Number of Moves: ";
    const char* last_move_string = "Last Move: ";
   
    //Set up some variables
    move_count = 0;
    my_game_counter = 0;
    last_move = NONE;

   /*
    * Cacluate game area dimensions:
    *   The height and width are primarily defined by the grid
    *     size. I want an empty space before and after each row/column.
    *     This is why we start with (grid_size + 1). 
    *   Game is double spaced, so base height is multiplied by 2.
    *   Each game "tile" can consist of up to 4 digits. Including
    *     the space between tiles, so the width is multiplied by 5.
    */
    my_game_area_height = (grid_size + 1) * 2 + 1;
    my_game_area_width = (grid_size + 1 ) * 5;


    //Initialize the ncurses "window"   
    initscr();			//Initialize ncurses
    noecho();			//Dont echo user input (for manual play)
    keypad(stdscr, TRUE); 	//Enable some special keys (need dir. keys)
    getmaxyx(stdscr, terminal_height, terminal_width); //Get size of Terminal window
    

   /*
    * Define minimum required terminal size to display game:
    *   Height needs some run for title and for the stats,
    *     hence +7.
    *   Width is just the width of the playing area, unless
    *     the playing area is smaller than the largest string,
    *     which is currently the title.
    */
    min_req_height = my_game_area_height + 7;
    min_req_width = my_game_area_width;
    if (min_req_width < strlen(title))
	min_req_width = strlen(title);
   

    //If terminal window is too small, print error and gracefully close/exit
    if (row < min_req_height || col < min_req_width)
    {
	endwin();
	printf("Terminal window is too small.\n");
	printf("Grid size is %d.\n", grid_size):
	printf("Need %d rows, have %d rows.\n", min_req_height, terminal_height);
	printf("Need %d cols, have %d cols.\n", min_req_width, terminal_width);
	printf("Exiting...\n");
	exit(1);
    }
  
  
    //Print title 
    mvprintw(1, (col - strlen(title))/2, title);
    refresh();
    
    //Create ncurses window to display the game
    WINDOW* game_area = newwin(
    			my_game_area_height,		//Window height
    			my_game_area_width,		//Window width
			3,				//Window x coord
			(col - my_game_area_width)/2);  //Window y coord

    //Define game_area border
    wborder(game_area, '|', '|', '-', '-', '+', '+', '+', '+'); 
    wrefresh(game_area);

    //Print the game
    my_game->print_game_board(game_area);
    //Print move count
    mvprintw(3 + my_game_area_height + 1, (col - (strlen(count_string) + 2))/2, "%s%d", count_string, my_game->get_move_count());
    refresh();


    int input;
    while(!my_game->is_game_won())
    {
        while(!my_game->is_game_over())
        {
	    /*
	    input = getch();
	    switch(input)
	    {
		case KEY_LEFT:
		    last_move = LEFT;
		    break;
		case KEY_RIGHT:
		    last_move = RIGHT;
		    break;
		case KEY_UP:
		    last_move = UP;
		    break;
		case KEY_DOWN:
		    last_move = DOWN;
		    break;
		default:
		    last_move = NONE;
		    break;
	    }
	    */

	    my_game->execute_random_move();

	    my_game->print_game_board(game_area);
	    wrefresh(game_area);
	    mvprintw(3 + my_game_area_height + 1, (col - (strlen(count_string) + 2))/2, "%s%d", count_string, my_game->get_move_count());
	    refresh();
            }

        my_game_counter++;
        mvprintw(3 + my_game_area_height + 3, (col - (strlen("Games Played: "))+1)/2, "%s%d", "Games Played ", my_game_counter);
        refresh();
        if(!my_game->is_game_won())
	    my_game->reset_game();
    }
    getch(); 
    endwin();
    return 0;
}


