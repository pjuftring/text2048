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

void draw(void)
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

	color_set(10, 0);
	mvaddstr(LINES-3,2,"Use arrow keys to play");
	mvaddstr(LINES-2,2,"Press Q to exit");

	refresh();
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
				return 1;
			}
		}
	}

	return 0;
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

	spawn_number();

	int key;
	for(;;){
		draw();

		key = getch();
		if(tolower(key) == 'q') break;
		

		switch(key){
		case KEY_LEFT:
			if(map_leftop())
				spawn_number();
			break;
		case KEY_RIGHT:
			if(map_rightop())
				spawn_number();
			break;
		case KEY_UP:
			if(map_upop())
				spawn_number();
			break;
		case KEY_DOWN:
			if(map_downop())
				spawn_number();
			break;
		}
	}

	return 0;
}
