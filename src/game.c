#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "include/audio.h"
#include "include/game.h"
#include "include/raylib.h"

static Map * GameMap = NULL;

void collide_with_ghost(void * ptr){// 定義撞到鬼會發生什麼事
	Map * map = ((Map *)ptr);
	map -> alive = 0;
}

void collide_with_freeze(void * ptr){// 定義撞到 freeze 效果的 booster 會發生什麼事 
	Map * map = ((Map *)ptr);
	map -> cool_down = 3;
}

static MapObject * make_ghost(int x, int y){// 將創建一個鬼的功能從主流城中剝離出來
	MapObject * ghost = malloc(sizeof(MapObject));
	ghost = malloc(sizeof(MapObject));
	ghost -> x = x;
	ghost -> y = y;
	ghost -> repr = 'G';
	ghost -> collide = &collide_with_ghost;
	return ghost;
}

void map_print(){
	printf("\e[1;1H\e[2J");
	char ** ptr;
	char * temp;

	ptr = malloc(GameMap -> x * sizeof(char *));
	for(int x = 0; x < GameMap -> x; x++)
		ptr[x] = malloc(GameMap -> y * sizeof(char));

	for(int y = 0; y < GameMap -> y; y++){
		for(int x = 0; x < GameMap -> x; x++){
			ptr[x][y] = '*';
		}
	}

	for(int i = 0; i < sizeof(GameMap -> enemy) / sizeof(GameMap -> enemy[0]); i++){
		if(GameMap -> enemy[i] != NULL){
			ptr[GameMap -> enemy[i] -> x][GameMap -> enemy[i] -> y] = GameMap -> enemy[i] -> repr;
		}
	}

	for(int i = 0; i < sizeof(GameMap -> booster) / sizeof(GameMap -> booster[0]); i++){
		if(GameMap -> booster[i] != NULL){
			ptr[GameMap -> booster[i] -> x][GameMap -> booster[i] -> y] = GameMap -> booster[i] -> repr;
		}
	}

	ptr[GameMap -> player -> x][GameMap -> player -> y] = GameMap -> player -> repr;

	for(int y = 0; y < GameMap -> y; y++){
		for(int x = 0; x < GameMap -> x; x++){
			if(ptr[x][y]){
				putchar(ptr[x][y]);
				putchar(' ');
			}
		}
		putchar('\n');
	}

	for(int x = 0; x < GameMap -> x; x++)
		free(ptr[x]);
	free(ptr);

}

void map_init(int x, int y){
	srand((unsigned int)time(NULL));
	GameMap = malloc(sizeof(Map));
	if (GameMap == NULL)
		return ;

	GameMap -> alive = 1;
	GameMap -> x = x;
	GameMap -> y = y;

	GameMap -> player = malloc(sizeof(MapObject));
	GameMap -> player -> repr = 'P';
	GameMap -> player -> x = 3;
	GameMap -> player -> y = 3;

	GameMap -> enemy =  malloc(sizeof(MapObject *) * 3);
	GameMap -> enemy[0] = make_ghost(2, 2);

	GameMap -> booster =  malloc(sizeof(MapObject *) * 3);
	GameMap -> booster[0] = malloc(sizeof(MapObject));
	GameMap -> booster[0] -> repr = 'B';
	GameMap -> booster[0] -> x = 0;
	GameMap -> booster[0] -> y = 0;
	GameMap -> booster[0] -> collide = &collide_with_freeze;
}

void map_update(){
	char ** ptr;
	char * temp;
	printf("\e[1;1H\e[2J");

	ptr = malloc(GameMap -> x * sizeof(char *));
	for(int x = 0; x < GameMap -> x; x++)
		ptr[x] = malloc(GameMap -> y * sizeof(char));

	for(int y = 0; y < GameMap -> y; y++){
		for(int x = 0; x < GameMap -> x; x++){
			ptr[x][y] = '*';
		}
	}

	for(int i = 0; i < sizeof(GameMap -> booster) / sizeof(GameMap -> booster[0]); i++){
		if(GameMap -> booster[i] != NULL){
			ptr[GameMap -> booster[i] -> x][GameMap -> booster[i] -> y] = GameMap -> booster[i] -> repr;
		}
	}

	if (!GameMap -> cool_down){
		int a = 0;
		
		for(int i = 0; i < sizeof(GameMap -> enemy) / sizeof(GameMap -> enemy[0]); i++){
			if(GameMap -> enemy[i] != NULL){

				while (1){
					a = rand() % 4;

					if (a == 0){
						if (GameMap -> enemy[i] -> x - 1 >= 0 && ptr[GameMap -> enemy[i] -> x - 1][ GameMap -> enemy[i] -> y] != 'B'){
							GameMap -> enemy[i] -> x -= 1;
							break;
						}
					}

					else if (a == 1){
						if ((GameMap -> enemy[i] -> y) + 1 < GameMap -> y && ptr[GameMap -> enemy[i] -> x][ GameMap -> enemy[i] -> y + 1] != 'B'){
							GameMap -> enemy[i] -> y += 1;
							break;
						}
					}

					else if (a == 2){
						if ((GameMap -> enemy[i] -> x) + 1 < GameMap -> x && ptr[GameMap -> enemy[i] -> x + 1][ GameMap -> enemy[i] -> y] != 'B'){
							GameMap -> enemy[i] -> x += 1;
							break;
						}
					}

					else if (a == 3){
						if ((GameMap -> enemy[i] -> y) - 1 >= 0 && ptr[GameMap -> enemy[i] -> x][ GameMap -> enemy[i] -> y - 1] != 'B'){
							GameMap -> enemy[i] -> y -= 1;
							break;
						}
					}
				}

				if((GameMap -> enemy[i] -> x == GameMap -> player -> x) && (GameMap -> enemy[i] -> y == GameMap -> player -> y)){
					GameMap -> enemy[i] -> collide(GameMap);
				}
			}
		}
	}

	else{

		for(int i = 0; i < sizeof(GameMap -> enemy) / sizeof(GameMap -> enemy[0]); i++){
			if(GameMap -> enemy[i] != NULL){
				if((GameMap -> enemy[i] -> x == GameMap -> player -> x) && (GameMap -> enemy[i] -> y == GameMap -> player -> y)){
					free(GameMap -> enemy[i]);
					GameMap -> enemy[i] = NULL;
				}
			}
		}

		--(GameMap -> cool_down);
	}

	for(int i = 0; i < sizeof(GameMap -> booster) / sizeof(GameMap -> booster[0]); i++){
		if(GameMap -> booster[i] != NULL){
			if((GameMap -> booster[i] -> x == GameMap -> player -> x) && (GameMap -> booster[i] -> y == GameMap -> player -> y)){
				GameMap -> booster[i] -> collide(GameMap);
				free(GameMap -> booster[i]);
				GameMap -> booster[i] = NULL;
				PlayBooster();
			}
		}

	}
	
	ptr[GameMap -> player -> x][GameMap -> player -> y] = GameMap -> player -> repr;

	for(int i = 0; i < sizeof(GameMap -> enemy) / sizeof(GameMap -> enemy[0]); i++){

		if(GameMap -> enemy[i] != NULL){
			ptr[GameMap -> enemy[i] -> x][GameMap -> enemy[i] -> y] = GameMap -> enemy[i] -> repr;
		}

	}

	for(int y = 0; y < GameMap -> y; y++){
		for(int x = 0; x < GameMap -> x; x++){
			if(ptr[x][y]){
				putchar(ptr[x][y]);
				putchar(' ');
			}
		}
		putchar('\n');
	}

	for(int x = 0; x < GameMap -> x; x++)
		free(ptr[x]);
	free(ptr);
}

void map_run(){
	char movement;

	if (IsKeyPressed(KEY_A)){
		if (GameMap -> player -> x - 1 >= 0){
			GameMap -> player -> x -= 1;
			map_update();
		}
	}
	else if (IsKeyPressed(KEY_S)){
		if ((GameMap -> player -> y) + 1 < GameMap -> y){
			GameMap -> player -> y += 1;
			map_update();
		}
	}
	else if (IsKeyPressed(KEY_D)){
		if ((GameMap -> player -> x) + 1 < GameMap -> x){
			GameMap -> player -> x += 1;
			map_update();
		}
	}
	else if (IsKeyPressed(KEY_W)){
		if ((GameMap -> player -> y) - 1 >= 0){
			GameMap -> player -> y -= 1;
			map_update();
		}
	}
}

int IsGameWin(){
	return 0;
}

int IsGameAlive(){
	return GameMap -> alive;
}