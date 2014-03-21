#include "ncurses.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "Game.h"

const char* direction_string[] = {"Up   ", "Down ", "Left ", "Right", "None "};


int main(int argc, char** argv)
{
    srand(time(NULL));  //seed rand()

    int grid_size = 4;

    Game* my_game = new Game(grid_size);
    
    int row,
        col,
	cur_row,
	cur_col,
	min_req_height,
	min_req_width,
	my_game_area_height,
	my_game_area_width,
	move_count,
	last_move,
	my_game_counter;


    const char* title = "2048 Tester - Michael Denny";
    const char* count_string = "Number of Moves: ";
    const char* last_move_string = "Last Move: ";
    move_count = 0;
    my_game_counter = 0;
    last_move = NONE;
    my_game_area_height = (grid_size + 1) * 2 + 1;
    my_game_area_width = (grid_size + 1 ) * 5;
   
    initscr();
    noecho();
    keypad(stdscr, TRUE); 
    getmaxyx(stdscr, row, col);
   
    min_req_height = my_game_area_height + 7;
    min_req_width = my_game_area_width;
    if (min_req_width < strlen(title))
	min_req_width = strlen(title);
   
    if (row < min_req_height || col < min_req_width)
    {
	endwin();
	printf("Terminal window is too small.\nGrid size is %d.\nNeed %d rows, have %d rows.\nNeed %d cols, have %d cols.\nExiting...\n",grid_size, min_req_height, row, min_req_width, col);
	exit(1);
    }
   
    mvprintw(1, (col - strlen(title))/2, title);
    refresh();
    
    WINDOW* my_game_area = newwin(my_game_area_height,
    				my_game_area_width,
				3,
				(col - my_game_area_width)/2);
    wborder(my_game_area, '|', '|', '-', '-', '+', '+', '+', '+'); 
    wrefresh(my_game_area);


    my_game->print_game_board(my_game_area);
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

	    my_game->print_game_board(my_game_area);
	    wrefresh(my_game_area);
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


