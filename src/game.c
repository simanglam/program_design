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

void CollideWithBean(void * map){// 定義撞到 Score 會發生什麼事
	Map * ptr = map;
	ptr -> playerScore += 1;
}

void CollideWithFrezze(void * map){// 定義撞到 freeze 效果的 booster 會發生什麼事
	Map * ptr = map;
	ptr -> cool_down = 3;
	PlayBooster();
}

void CollideWithExtra(void * map){// 定義撞到 freeze 效果的 booster 會發生什麼事
	Map * ptr = map;
	ptr -> alive += 1;
}

void CollideWithLess(void * map){// 定義撞到 freeze 效果的 booster 會發生什麼事
	Map * ptr = map;
	for(int i = 0; i < ptr -> enemyCount; i++){
		if(ptr -> enemy[i] != NULL){
			free(ptr -> world[ptr -> player -> x][ptr -> player -> y]);
			ptr -> world[ptr -> player -> x][ptr -> player -> y] = NULL;
			ptr -> enemy[i] = NULL;
			break;
		}
	}
}

/*
上面用 void * 的原因是因為 MapObject 早於 Map 定義。詳見 game.h
*/

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

	case 2:
		booster -> collide = &CollideWithLess;
		break;
	
	default:
		booster -> collide = &CollideWithFrezze;
		break;
	}
	return booster;
}

static MapObject * MakeBean(){// 將創建一個 Score 的功能從主流程中剝離出來
	MapObject * Bean = malloc(sizeof(MapObject));
	if (Bean == NULL)
		exit(1);
	Bean -> repr = 'S';
	Bean -> collide = &CollideWithBean;
	return Bean;
}

Map * MapInit(int x,int y, int BeansAmount, int BoosterAmount){
	srand((unsigned int)time(NULL)); // 設定隨機種子
	int tempX = 0, tempY = 0;
	Map * map = malloc(sizeof(Map));
	if (map == NULL)
		exit(1);

	map -> alive = 1;
	map -> x = x;
	map -> y = y;
	
	map -> winScore = ((x * y) / (7 + BeansAmount - 1));
	
	map -> enemyCount = (x * y) / 7;
	map -> enemy =  (MapObject **)malloc(sizeof(MapObject *) * map -> enemyCount);

	if (map -> enemy == NULL)
		exit(1);
	
	map -> world = (MapObject ***)malloc(x * sizeof(MapObject ***));
	MapObject ** world_array = (MapObject **)malloc(x * y * sizeof(MapObject **));
	if (map -> world == NULL || world_array == NULL)
		exit(1);

	for(int i = 0; i < map -> x; i++){
		map -> world[i] = world_array + (i * y); // 把兩個一維陣列組成二維陣列
	}

	for(int y = 0; y < map -> y; y++){
        for(int x = 0; x < map -> x; x++){
            map -> world[x][y] = NULL; // init array
        }
    }
	
	for(int i = 0; i < map -> enemyCount; i++){ // 隨機產生 Ghost
		do{
			tempX = rand() % map -> x; tempY = rand() % map -> y;
		} while (map -> world[tempX][tempY] != NULL);
		map -> enemy[i] = MakeGhost(tempX, tempY);
		map -> world[tempX][tempY] = map -> enemy[i];
	}

	for(int i = 0; i < ((x * y) / (7 + BoosterAmount - 1)); i++){ // 隨機產生 Booster
		do{
			tempX = rand() % map -> x; tempY = rand() % map -> y;
		} while (map -> world[tempX][tempY] != NULL);
		map -> world[tempX][tempY] = MakeBooster(rand() % 3);
	}

	for(int i = 0; i < map -> winScore; i++){ // 隨機產生 Score
		do{
			tempX = rand() % map -> x; tempY = rand() % map -> y;
		} while (map -> world[tempX][tempY] != NULL);
		map -> world[tempX][tempY] = MakeBean();
	}

	map -> player = (MapObject *)malloc(sizeof(MapObject));
	if (map -> player == NULL)
		exit(1);
	map -> player -> repr = 'P';

	do{ // 隨機放置玩家
		tempX = rand() % map -> x; tempY = rand() % map -> y;
	} while (map -> world[tempX][tempY] != NULL);

	map -> player -> x = tempX;
	map -> player -> y = tempY;

	return map;
}

void MapUpdate(Map * map){

	int oldX, oldY;
	if (!map -> cool_down){ // 如果不是吃到 freeze booster
		int a = 0;
		
		for(int i = 0; i < map -> enemyCount; i++){
			if(map -> enemy[i] != NULL){// 更新 enemy
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
						// 如果隨機移動後在世界範圍內
						if(map -> world[map -> enemy[i] -> x][map -> enemy[i] -> y] == NULL){
							// 如果目的地沒有其他東西，就成功移動，並跳出迴圈
							map -> world[map -> enemy[i] -> x][map -> enemy[i] -> y] = map -> world[oldX][oldY];
							map -> world[oldX][oldY] = NULL;
							break;
						}
						// 如果四面都有東西，就直接跳出
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
		for(int i = 0; i < map -> enemyCount; i++){
			if(map -> enemy[i] != NULL){
				if((map -> enemy[i] -> x == map -> player -> x) && (map -> enemy[i] -> y == map -> player -> y)){
					free(map -> world[map -> player -> x][map -> player -> y]);
					map -> world[map -> player -> x][map -> player -> y] = NULL;
					map -> enemy[i] = NULL;
				}
			}
		}
		--(map -> cool_down);
		if(map -> cool_down == 0)
			PlayBg();
	}

	if(map -> world[map -> player -> x][map -> player -> y] != NULL){ // 如果玩家與其他東西 collide
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

void MapRun(Map * map){ // Facade 模式，讓使用者只需呼叫此函數，而不用管偵測鍵盤、與更新函數的實作
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

void FreeMap(Map * map){// Free map to prevent memory leak
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