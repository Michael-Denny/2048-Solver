#include "ncurses.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

enum Color {WHITE, RED, GREEN, CYAN, YELLOW, BLUE};

enum Direction {UP, DOWN, LEFT, RIGHT, NONE};
const char* direction_string[] = {"Up", "Down", "Left", "Right", "None"};


struct Node {
    int value;
    enum Color color;
};

//globals
int GRID_SIZE = 4;

//protos
void print_game(Node** array, WINDOW*);
void print_node(Node node, WINDOW*, int pos_y, int pos_x);
void init_node_array(Node** &);
void free_node_array(Node** &);
int max(int[]);

int max(int a[])
{
    int rtn_val = 0;
    for (int i = 0; i < sizeof(a)/sizeof(int); i++)
        if (a[i] > rtn_val)
	    rtn_val = a[i];

    return rtn_val;
}

void print_game(Node** array, WINDOW* window)
{
    for (int i = 0; i < GRID_SIZE; i++)
	for (int j = 0; j < GRID_SIZE; j++)
	{
	    print_node(array[i][j], window, 2 + (i * 2), 3 + (j * 5));
	}

    wrefresh(window);
}

void print_node(Node node, WINDOW* window, int pos_y, int pos_x)
{
    if (node.value == 0)
	mvwprintw(window, pos_y, pos_x, "----");
    else if (node.value < 10)
        mvwprintw(window, pos_y, pos_x, "%2d  ", node.value);  
    else if (node.value < 100)
        mvwprintw(window, pos_y, pos_x, "%3d ", node.value);  
    else
        mvwprintw(window, pos_y, pos_x, "%4d", node.value);  
}


void free_node_array(Node** &array)
{
    for (int i = 0; i < GRID_SIZE; i++)
	free(array[i]);
    free(array);
}

void init_node_array(Node** &array)
{
    array = (Node**)malloc(sizeof(Node*) * GRID_SIZE);
    for (int i = 0; i < GRID_SIZE; i++)
	array[i] = (Node*)malloc(sizeof(Node) * GRID_SIZE);
    for (int i = 0; i < GRID_SIZE; i++)
	for (int j = 0; j < GRID_SIZE; j++)
	{
	    array[i][j].value = 0;
	    array[i][j].color = WHITE;
	}

    int random1 = rand() % 4;
    int random2 = rand() % 4;
    array[random1][random2].value = 2;

}

int main(int argc, char** argv)
{
    srand(time(NULL));  //seed rand()

    Node** node_array;
    
    int row,
        col,
	cur_row,
	cur_col,
	min_req_height,
	min_req_width,
	game_area_height,
	game_area_width,
	move_count,
	last_move;


    const char* title = "2048 Tester - Michael Denny";
    const char* count_string = "Number of Moves: ";
    const char* last_move_string = "Last Move: ";
    move_count = 0;
    last_move = NONE;
    game_area_height = (GRID_SIZE + 1) * 2 + 1;
    game_area_width = (GRID_SIZE + 1 ) * 5;
   
    init_node_array(node_array);
    
    initscr();
   
    getmaxyx(stdscr, row, col);
   
    min_req_height = game_area_height + 6;
    min_req_width = max( new int[4] {game_area_width, strlen(title), strlen(count_string) + 4, strlen(last_move_string) + 5 } ); 
   
    if (row < min_req_height || col < min_req_width)
    {
	endwin();
	printf("Terminal window is too small.\nGrid size is %d.\nNeed %d rows, have %d rows.\nNeed %d cols, have %d cols.\nExiting...\n",GRID_SIZE, min_req_height, row, min_req_width, col);
	exit(1);
    }
   
    mvprintw(1, (col - strlen(title))/2, title);
    refresh();
    
    WINDOW* game_area = newwin(game_area_height,
    				game_area_width,
				3,
				(col - game_area_width)/2);
    wborder(game_area, '|', '|', '-', '-', '+', '+', '+', '+'); 
    wrefresh(game_area);

    while(1)
    {

    print_game(node_array, game_area);
    mvprintw(3 + game_area_height + 1, (col - (strlen(count_string) + 2))/2, "%s%d", count_string, move_count);
    refresh();
    mvprintw(3 + game_area_height + 2, (col - (strlen(last_move_string) + 4))/2, "%s%s", last_move_string, direction_string[last_move]);
    refresh();
    getch();
    }
    endwin();
    free_node_array(node_array);
    return 0;
}


