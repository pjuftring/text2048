#include <ctype.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <curses.h>

#include "text2048.h"
#include "map_ops.h"

void swap_int(int *const a, int *const b){
	if(a != b){
		register int tmp = *a;
		*a = *b;
		*b = tmp;
	}
}

unsigned int map[GAME_COLS][GAME_COLS] = {0}; 

int pow2(int exp)
{
	int x = 1;
	if(exp < 0) return 0;
	while(exp--)
		x *= 2;
	return x;
}

void drawmap(void)
{
	int x,y;
	char str[50];

	clear();

	for(y = 0; y < GAME_COLS; y++){
		for(x = 0; x < GAME_COLS; x++){
			const int text_y = 2*y+2, text_x = 8*x+2;

			if(map[x][y] == 0){
				color_set(2+map[x][y]/2, 0);
				mvaddstr(text_y, text_x, "       ");
				
			}else{
				if(map[x][y] % 2 == 1){
					color_set(1, 0);
					mvaddstr(text_y, text_x, "       ");
					sprintf(str, "%5d", pow2(map[x][y]));
					color_set(2+map[x][y]/2, 0);
					mvaddstr(text_y, text_x+1, str);
				}else{
					sprintf(str, " %5d ", pow2(map[x][y]));
					color_set(2+map[x][y]/2, 0);
					mvaddstr(text_y, text_x, str);
				}
			}
		}
	}
}

void shuffle(int vec[], const int len)
{
	int i,pos1,pos2;
	for(i = 0; i < 2*len; i++){
		pos1 = rand() % GAME_COLS;
		pos2 = rand() % GAME_COLS;
		
		swap_int(&vec[pos1], &vec[pos2]);
	}

}

void gameover(void)
{
	drawmap();

	color_set(9, 0);
	mvaddstr(LINES-4,2,"GAME OVER");
	mvaddstr(LINES-3,2,"Press Q to exit");
	mvaddstr(LINES-2,2,"Press any other key to play again");

	refresh();
	
	int c;
	do{
		c = getch();
	}while(c == KEY_LEFT || c == KEY_RIGHT || c == KEY_UP || c == KEY_DOWN);
	/* will prevent accidental new game */
	
	if(c == 'q' || c == 'Q')
		exit(0);
}

int spawn_number(void)
{
	int line_vec[GAME_COLS], col_vec[GAME_COLS];
	int i,j;
	
	for(i = 0; i < GAME_COLS; i++)
		line_vec[i] = i;
	for(i = 0; i < GAME_COLS; i++)
		col_vec[i] = i;

	shuffle(col_vec,GAME_COLS);
	shuffle(line_vec,GAME_COLS);

	for(j = 0; j < GAME_COLS; j++){
		for(i = 0; i < GAME_COLS; i++){
			const int x = col_vec[i], y = line_vec[j];
			
			if(map[x][y] == 0){
				int w = (rand() % 40)/4;
				if(w == 0)
					map[x][y] = 2;
				else
					map[x][y] = 1;

				if(!map_check_move_possible()){
					gameover();
					return 0;
				}
				
				return 1;
			}
		}
	}

	return 0;
}

void resetmap(void)
{
	int x,y;
	for(y = 0; y < GAME_COLS; y++)
		for(x = 0; x < GAME_COLS; x++)
			map[x][y] = 0;
	spawn_number();
}

int main(int argc, char **argv)
{
	initscr();
	atexit((void (*)(void)) endwin);
	start_color();
	init_pair(1, COLOR_WHITE, COLOR_RED); 
	init_pair(2, COLOR_WHITE, COLOR_YELLOW); 
	init_pair(3, COLOR_WHITE, COLOR_GREEN); 
	init_pair(4, COLOR_WHITE, COLOR_BLUE); 
	init_pair(5, COLOR_WHITE, COLOR_MAGENTA);
	init_pair(6, COLOR_YELLOW, COLOR_CYAN);
	init_pair(7, COLOR_YELLOW, COLOR_BLACK);
	init_pair(8, COLOR_GREEN, COLOR_BLACK);
	init_pair(9, COLOR_RED, COLOR_BLACK);
	init_pair(10, COLOR_WHITE, COLOR_BLACK);
	attron(A_BOLD);
	curs_set(0);
	cbreak();
	keypad(stdscr, TRUE);

	resetmap();

	int key;
	for(;;){
		drawmap();

		color_set(10, 0);
		mvaddstr(LINES-3,2,"Use arrow keys to play");
		mvaddstr(LINES-2,2,"Press Q to exit");

		refresh();

		key = getch();
		if(tolower(key) == 'q') break;
		

		switch(key){
		case KEY_LEFT:
			if(map_leftop() && !spawn_number())
				resetmap();
			break;
		case KEY_RIGHT:
			if(map_rightop() && !spawn_number())
				resetmap();
			break;
		case KEY_UP:
			if(map_upop() && !spawn_number())
				resetmap();
			break;
		case KEY_DOWN:
			if(map_downop() && !spawn_number())
				resetmap();
			break;
		}
	}

	return 0;
}
