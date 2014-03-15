#include "ncurses.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <vector>
#include <algorithm>

enum Color {WHITE, RED, GREEN, CYAN, YELLOW, BLUE};

enum Direction {UP, DOWN, LEFT, RIGHT, NONE};
const char* direction_string[] = {"Up   ", "Down ", "Left ", "Right", "None "};


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
void execute_move(Node** &, int move, int &move_count);
bool check_game_over(Node**);

void execute_move(Node** &array, int move, int &move_count)
{
    bool legal_move = false;
    std::vector<int> temp_vector;
    //move up or down
    if (move == UP || move == DOWN)
    {
	for (int j = 0; j < GRID_SIZE; j++)
	{
	    for (int i = 0; i < GRID_SIZE; i++)
	        temp_vector.push_back(array[i][j].value);
	    if (move == DOWN)
		std::reverse(temp_vector.begin(), temp_vector.end());
	    
	    int index = 0;
	    
	    //check if a move is being made
	    bool free_space = false;
	    while (index < temp_vector.size())
	    {
		if (!free_space && temp_vector[index] == 0)
		    free_space = !free_space;
		else if (free_space && temp_vector[index] != 0)
		{
		    legal_move = true;
		    break;
		}
		index++;		    
	    }

	    index = 0;
	    while (index < temp_vector.size() - 1)
	    {
		if (temp_vector[index] == 0)
		    temp_vector.erase(temp_vector.begin() + index);
		else
		    index++;
	    }
	    
	    index = 0;	    
	    while (index < temp_vector.size() - 1)
	    {
	        if (temp_vector[index] == temp_vector[index + 1])
		{
		    temp_vector[index] *= 2;
		    temp_vector.erase(temp_vector.begin() + index + 1);
		}
		index++;
	    }
	   
	    while (temp_vector.size() < GRID_SIZE)
		temp_vector.push_back(0);
	   
	    if (move == DOWN)
		std::reverse(temp_vector.begin(), temp_vector.end());
	    
	    for (int i = 0; i < GRID_SIZE; i++)
		    array[i][j].value = temp_vector[i];
	    
	    temp_vector.clear();
	}
    }


    else //move == right or left
    {
	for (int i = 0; i < GRID_SIZE; i++)
	{
	    for (int j = 0; j < GRID_SIZE; j++)
	        temp_vector.push_back(array[i][j].value);
	    if (move == RIGHT)
		std::reverse(temp_vector.begin(), temp_vector.end());
	    int index = 0;

            //check if a move is being made
	    bool free_space = false;
	    while (index < temp_vector.size())
	    {
		if (!free_space && temp_vector[index] == 0)
		    free_space = !free_space;
		else if (free_space && temp_vector[index] != 0)
		{
		    legal_move = true;
		    break;
		}
		index++;		    
	    }

	    index = 0;
	    while (index < temp_vector.size() - 1)
	    {
		if (temp_vector[index] == 0)
		    temp_vector.erase(temp_vector.begin() + index);
		else
		    index++;
	    }
	    
	    index = 0;	    
	   
	    while (index < temp_vector.size() - 1)
	    {
	        if (temp_vector[index] == temp_vector[index + 1])
		{
		    temp_vector[index] *= 2;
		    temp_vector.erase(temp_vector.begin() + index + 1);
		}
		index++;
	    }
	    
	    while (temp_vector.size() < GRID_SIZE)
		temp_vector.push_back(0);
	    
	    if (move == RIGHT)
		std::reverse(temp_vector.begin(), temp_vector.end());

	    for (int j = 0; j < GRID_SIZE; j++)
		    array[i][j].value = temp_vector[j];
	    
	    temp_vector.clear();
	}
    }

    //add new tile
    if(legal_move)
    {
        std::vector<std::pair<int,int> > blank_tiles;
        for (int i = 0; i < GRID_SIZE; i++)
	    for (int j = 0; j < GRID_SIZE; j++)
	        if (array[i][j].value == 0)
	   	    blank_tiles.push_back(std::make_pair(i,j));
        if (blank_tiles.size() > 0)
        {
            int random = rand() % blank_tiles.size();
	    array[blank_tiles[random].first][blank_tiles[random].second].value = 2;
        }
	move_count++;
    }

}

bool check_game_over(Node** array)
{
    //if there are any empty space, game is not over
    for (int i = 0; i < GRID_SIZE; i++)
	for (int j = 0; j < GRID_SIZE; j++)
	    if (array[i][j].value == 0)
		return false;

    //if any rows have adjacent pairs that match, game is not over
    for (int i = 0; i < GRID_SIZE; i++)
    {
        int temp = -1;
	for (int j = 0; j < GRID_SIZE; j++)
	{
	    if (temp < 0)
		temp = array[i][j].value;
	    else if(temp == array[i][j].value)
		return false;
	    else
		temp = array[i][j].value;
	}
    }

    //if any columns have adjacent pairs that match, game is not over
    for (int j = 0; j < GRID_SIZE; j++)
    {
        int temp = -1;
	for (int i = 0; i < GRID_SIZE; i++)
	{
	    if (temp < 0)
		temp = array[i][j].value;
	    else if(temp == array[i][j].value)
		return false;
	    else
		temp = array[i][j].value;
	}
    }
    return true;
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

    bool game_over = false;
    
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
    noecho();
    keypad(stdscr, TRUE); 
    getmaxyx(stdscr, row, col);
   
    min_req_height = game_area_height + 6;
    min_req_width = game_area_width;
    if (min_req_width < strlen(title))
	min_req_width = strlen(title);
   
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


    print_game(node_array, game_area);
    mvprintw(3 + game_area_height + 1, (col - (strlen(count_string) + 2))/2, "%s%d", count_string, move_count);
    refresh();
    mvprintw(3 + game_area_height + 2, (col - (strlen(last_move_string) + 4))/2, "%s%s", last_move_string, direction_string[last_move]);
    refresh();


    int input;
    while(!game_over)
    {
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

	if (last_move != NONE);
	execute_move(node_array, last_move, move_count);
	game_over = check_game_over(node_array);

        print_game(node_array, game_area);
        mvprintw(3 + game_area_height + 1, (col - (strlen(count_string) + 2))/2, "%s%d", count_string, move_count);
        refresh();
        mvprintw(3 + game_area_height + 2, (col - (strlen(last_move_string) + 4))/2, "%s%s", last_move_string, direction_string[last_move]);
        refresh();
    }
    endwin();
    free_node_array(node_array);
    return 0;
}


