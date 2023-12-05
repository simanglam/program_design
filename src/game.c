#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "include/audio.h"
#include "include/game.h"
#include "include/raylib.h"

void CollideWithGhost(void * map){// 定義撞到鬼會發生什麼事
	Map * ptr = map;
	ptr -> alive -= 1;
}

void CollideWithBean(void * map){// 定義撞到 freeze 效果的 booster 會發生什麼事 
	Map * ptr = map;
	ptr -> playerScore += 1;
}

void CollideWithFrezze(void * map){// 定義撞到 freeze 效果的 booster 會發生什麼事
	Map * ptr = map;
	ptr -> cool_down = 3;
}

void CollideWithExtra(void * map){// 定義撞到 freeze 效果的 booster 會發生什麼事
	Map * ptr = map;
	ptr -> alive += 1;
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

static MapObject * MakeBooster(int type){// 將創建一個鬼的功能從主流程中剝離出來
	MapObject * booster = malloc(sizeof(MapObject));
	if (booster == NULL)
		exit(1);
	booster -> repr = 'B';
	switch (type)
	{
	case 0:
		booster -> collide = &CollideWithFrezze;
		break;
	
	case 1:
		booster -> collide = &CollideWithExtra;
		break;
	
	default:
		booster -> collide = &CollideWithFrezze;
		break;
	}
	return booster;
}

static MapObject * MakeBean(){// 將創建一個鬼的功能從主流程中剝離出來
	MapObject * Bean = malloc(sizeof(MapObject));
	if (Bean == NULL)
		exit(1);
	Bean -> repr = 'S';
	Bean -> collide = &CollideWithBean;
	return Bean;
}

Map * MapInit(int x,int y, int BeansAmount, int BoosterAmount){
	srand((unsigned int)time(NULL));
	int tempX = 0, tempY = 0;
	Map * map = malloc(sizeof(Map));
	if (map == NULL)
		exit(1);

	map -> alive = 1;
	map -> x = x;
	map -> y = y;
	
	map -> winScore = ((x * y) / (7 + BeansAmount - 1));

	map -> enemy =  (MapObject **)malloc(sizeof(MapObject *) * (x * y) / 7);

	if (map -> enemy == NULL)
		exit(1);
	
	map -> world = (MapObject ***)malloc(x * sizeof(MapObject ***));
	MapObject ** world_array = (MapObject **)malloc(x * y * sizeof(MapObject **));
	if (map -> world == NULL || world_array == NULL)
		exit(1);

	for(int i = 0; i < map -> x; i++){
		map -> world[i] = world_array + (i * y);
	}

	for(int y = 0; y < map -> y; y++){
        for(int x = 0; x < map -> x; x++){
            map -> world[x][y] = NULL;
        }
    }
	
	for(int i = 0; i < (x * y) / 7; i++){
		do{
			tempX = rand() % map -> x; tempY = rand() % map -> y;
		} while (map -> world[tempX][tempY] != NULL);
		map -> enemy[i] = MakeGhost(tempX, tempY);
		map -> world[tempX][tempY] = map -> enemy[i];
	}

	for(int i = 0; i < ((x * y) / (7 + BeansAmount - 1)); i++){
		do{
			tempX = rand() % map -> x; tempY = rand() % map -> y;
		} while (map -> world[tempX][tempY] != NULL);
		map -> world[tempX][tempY] = MakeBooster(rand() % 2);
	}

	for(int i = 0; i < map -> winScore; i++){
		do{
			tempX = rand() % map -> x; tempY = rand() % map -> y;
		} while (map -> world[tempX][tempY] != NULL);
		map -> world[tempX][tempY] = MakeBean();
	}

	map -> player = (MapObject *)malloc(sizeof(MapObject));
	if (map -> player == NULL)
		exit(1);
	map -> player -> repr = 'P';

	do{
		tempX = rand() % map -> x; tempY = rand() % map -> y;
	} while (map -> world[tempX][tempY] != NULL);

	map -> player -> x = tempX;
	map -> player -> y = tempY;

	return map;
}

void MapUpdate(Map * map){

	int oldX, oldY;
	if (!map -> cool_down){
		int a = 0;
		
		for(int i = 0; i < (map -> x * map -> y) / 7; i++){
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
							map -> world[map -> enemy[i] -> x][map -> enemy[i] -> y] = map -> world[oldX][oldY];
							map -> world[oldX][oldY] = NULL;
							break;
						}
						else if(map -> world[(oldX - 1 >= 0) ? oldX - 1 : oldX][oldY] != NULL && map -> world[(oldX + 1 < map -> x) ? oldX + 1 : oldX][oldY] != NULL && map -> world[oldX][(oldY - 1 >= 0) ? oldY - 1 : oldY]!= NULL && map -> world[oldX][(oldY + 1 < map -> y) ? oldY + 1 : oldY] != NULL){
							map -> enemy[i] -> x = oldX;  map -> enemy[i] -> y = oldY;
							break;
						}
					}
				} while (1);
			}
		}
	}

	else{
		for(int i = 0; i < (map -> x * map -> y) / 7; i++){
			if(map -> enemy[i] != NULL){
				if((map -> enemy[i] -> x == map -> player -> x) && (map -> enemy[i] -> y == map -> player -> y)){
					free(map -> world[map -> player -> x][map -> player -> y]);
					map -> world[map -> player -> x][map -> player -> y] = NULL;
					map -> enemy[i] = NULL;
				}
			}
		}
		--(map -> cool_down);
	}

	if(map -> world[map -> player -> x][map -> player -> y] != NULL){
		map -> world[map -> player -> x][map -> player -> y] -> collide(map);

		for(int i = 0; i < (map -> x * map -> y) / 7; i++){
			if(map -> enemy[i] != NULL){
				if((map -> enemy[i] -> x == map -> player -> x) && (map -> enemy[i] -> y == map -> player -> y)){
					map -> enemy[i] = NULL;
				}
			}
		}
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
            if(map -> world[x][y] != NULL){
				free(map -> world[x][y]);
			}
        }
    }
	free(map -> world[0]);
	free(map -> world);
	free(map -> enemy);
	free(map -> player);
	free(map);
}