#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "include/audio.h"
#include "include/game.h"
#include "include/raylib.h"

void CollideWithGhost(void * ptr){// 定義撞到鬼會發生什麼事
	Map * map = ((Map *)ptr);
	map -> alive = 0;
}

void CollideWithFrezze(void * ptr){// 定義撞到 freeze 效果的 booster 會發生什麼事 
	Map * map = ((Map *)ptr);
	map -> cool_down = 3;
}

static MapObject * MakeGhost(int x, int y){// 將創建一個鬼的功能從主流程中剝離出來
	MapObject * ghost = malloc(sizeof(MapObject));
	if (ghost == NULL)
		exit(1);
	ghost -> x = x;
	ghost -> y = y;
	ghost -> repr = 'G';
	ghost -> collide = &CollideWithGhost;
	return ghost;
}

Map * MapInit(int x, int y){
	srand((unsigned int)time(NULL));
	Map * map = malloc(sizeof(Map));
	if (map == NULL)
		exit(1);

	map -> alive = 1;
	map -> x = x;
	map -> y = y;

	map -> world = (MapObject ***)malloc(x * sizeof(MapObject ***));
	MapObject ** world_array = (MapObject **)malloc(x * y * sizeof(MapObject **));
	if (map -> world == NULL || world_array == NULL)
		exit(1);

	for(int i = 0; i < x; i++){
		map -> world[i] = world_array + (i * y);
	}
	for(int y = 0; y < map -> y; y++){
        for(int x = 0; x < map -> x; x++){
            map -> world[y][x] = NULL;
        }
    }

	map -> player = (MapObject *)malloc(sizeof(MapObject));
	if (map -> player == NULL)
		exit(1);
	map -> player -> repr = 'P';
	map -> player -> x = 3;
	map -> player -> y = 3;

	map -> world[0][0] = malloc(sizeof(MapObject));
	map -> world[0][0] -> collide = &CollideWithFrezze;
	map -> world[0][0] -> repr = 'B';

	map -> enemy =  (MapObject **)malloc(sizeof(MapObject *) * 3);
	if (map -> enemy == NULL)
		exit(1);
	map -> enemy[0] = MakeGhost(2, 2);

	for(int i = 0; i < sizeof(map -> enemy) / sizeof(map -> enemy[0]); i++){
		if(map -> enemy[i] != NULL){
			map -> world[map -> enemy[i] -> x][map -> enemy[i] -> y] = map -> enemy[i];
		}
	}

	return map;
}

void MapUpdate(Map * map){

	int oldX, oldY;
	if (!map -> cool_down){
		int a = 0;
		
		for(int i = 0; i < sizeof(map -> enemy) / sizeof(map -> enemy[0]); i++){
			if(map -> enemy[i] != NULL){
				oldX = map -> enemy[i] -> x; oldY = map -> enemy[i] -> y;
				do{
					map -> enemy[i] -> x = oldX;  map -> enemy[i] -> y = oldY;
					a = rand() % 4;
					if (a == 0){
						map -> enemy[i] -> x -= 1;
					}
					else if (a == 1){
						map -> enemy[i] -> y += 1;
					}
					else if (a == 2){
						map -> enemy[i] -> x += 1;
					}
					else if (a == 3){
						map -> enemy[i] -> y -= 1;
					}
					if(map -> enemy[i] -> x >= 0 && map -> enemy[i] -> x < map -> x  && map -> enemy[i] -> y >= 0 && map -> enemy[i] -> y < map -> y){
						if(map -> world[map -> enemy[i] -> x][map -> enemy[i] -> y] == NULL){
							break;
						}
					}
				} while (1);
				map -> world[map -> enemy[i] -> x][map -> enemy[i] -> y] = map -> world[oldX][oldY];
				map -> world[oldX][oldY] = NULL;
			}
		}
	}

	else{
		for(int i = 0; i < sizeof(map -> enemy) / sizeof(map -> enemy[0]); i++){
			if(map -> enemy[i] != NULL){
				if((map -> enemy[i] -> x == map -> player -> x) && (map -> enemy[i] -> y == map -> player -> y)){
					map -> world[map -> player -> x][map -> player -> y] = NULL;
					free(map -> enemy[i]);
					map -> enemy[i] = NULL;
				}
			}
		}
		--(map -> cool_down);
	}

	if(map -> world[map -> player -> x][map -> player -> y] != NULL){
		map -> world[map -> player -> x][map -> player -> y] -> collide(map);
		free(map -> world[map -> player -> x][map -> player -> y]);
		map -> world[map -> player -> x][map -> player -> y] = NULL;
	}
}

void MapRun(Map * map){
	if (IsKeyPressed(KEY_A)){
		if (map -> player -> x - 1 >= 0){
			map -> player -> x -= 1;
			MapUpdate(map);
		}
	}
	else if (IsKeyPressed(KEY_S)){
		if ((map -> player -> y) + 1 < map -> y){
			map -> player -> y += 1;
			MapUpdate(map);
		}
	}
	else if (IsKeyPressed(KEY_D)){
		if ((map -> player -> x) + 1 < map -> x){
			map -> player -> x += 1;
			MapUpdate(map);
		}
	}
	else if (IsKeyPressed(KEY_W)){
		if ((map -> player -> y) - 1 >= 0){
			map -> player -> y -= 1;
			MapUpdate(map);
		}
	}
}

void FreeMap(Map * map){
	for(int y = 0; y < map -> y; y++){
        for(int x = 0; x < map -> x; x++){
            if(map -> world[y][x] != NULL){
				free(map -> world[y][x]);
			}
        }
    }
	free(map -> world[0]);
	free(map -> world);
	free(map -> enemy);
	free(map -> player);
	free(map);
}