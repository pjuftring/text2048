
#include "text2048.h"

/*
void extract_map(unsigned int array[GAME_COLS][GAME_COLS]){
	int i;

	for(i = 0; i < GAME_COLS; i++){
		array[0][i] = map[i] & 0xFF;
		array[1][i] = (map[i] >> 8) & 0xFF;
		array[2][i] = (map[i] >> 16) & 0xFF;
		array[3][i] = (map[i] >> 24) & 0xFF;
	}
}

void compress_map(const unsigned int array[GAME_COLS][GAME_COLS]){
	int i;

	for(i = 0; i < GAME_COLS; i++){
		map[i] = array[0][i] | (array[1][i] << 8) | 
			(array[2][i] << 16) | (array[3][i] << 24);
	}
}
*/

int map_check_move_possible(void)
{
	int x,y;
	for(y = 0; y < GAME_COLS; y++){
		for(x = 0; x < GAME_COLS-1; x++){
			if(map[x][y] == map[x+1][y] || map[x][y] == 0)
				return 1;
		}
		if(map[GAME_COLS-1][y] == 0)
			return 1;
	}
	for(x = 0; x < GAME_COLS; x++){
		for(y = 0; y < GAME_COLS-1; y++){
			if(map[x][y] == map[x][y+1] || map[x][y] == 0)
				return 1;
		}
		if(map[x][GAME_COLS-1] == 0)
			return 1;
	}
	return 0;
}

int toleft(int line){
	int i = 0, j = 0, actions = 0;
	for(i = 0; i < GAME_COLS; i++){
		if(map[i][line] != 0){
			if(i != j){
				map[j][line] = map[i][line];
				map[i][line] = 0;
				actions++;
			}
			j++;
		}
	}
	return actions;
}

int map_leftop(void){
	int i,line;
	int actions = 0;

	for(line = 0; line < GAME_COLS; line++){
		actions += toleft(line);
		for(i = 0; i < GAME_COLS-1; i++){
			if(map[i][line] != 0 && map[i][line] == map[i+1][line]){
				map[i][line]++;
				map[i+1][line] = 0;
				actions++;
			}
		}
		actions += toleft(line);
	}

	return actions;
}

void map_transpose(void){
	int x,y;
	for(y = 0; y < GAME_COLS; y++){
		for(x = y+1; x < GAME_COLS; x++){
			swap_int(&map[x][y], &map[y][x]);
		}
	}
}

void map_rotate_left(int times){
	int x,y;

	times %= 4;
	if(times == 0) return;

	map_transpose();
	for(x = 0; x < GAME_COLS; x++){
		for(y = 0; y < GAME_COLS/2; y++){
			swap_int(&map[x][y], &map[x][GAME_COLS-y-1]);
		}
	}

	map_rotate_left(times-1);	
}

int map_upop(void){
	map_rotate_left(1);
	int actions = map_leftop();
	map_rotate_left(3);
	return actions;
}

int map_downop(void){
	map_rotate_left(3);
	int actions = map_leftop();
	map_rotate_left(1);
	return actions;
}

int map_rightop(void){
	map_rotate_left(2);
	int actions = map_leftop();
	map_rotate_left(2);
	return actions;
}
