#include <time.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct mapobject{
	int x, y; // 所在的位置
	char repr;
	void (*collide)(void * map); //這是 functional pointer 也可不用這個技巧，但就比較麻煩一點
} MapObject;

typedef struct map{
    int cool_down;
    int x, y;
    int alive;
    MapObject ** enemy;
    MapObject ** booster;
    MapObject * player;
} Map;


void collide_with_ghost(void * ptr){// 定義撞到鬼會發生什麼事
	Map * map = ((Map *)ptr);
	map -> alive = 0;
}

void collide_with_freeze(void * ptr){// 定義撞到 freeze 效果的 booster 會發生什麼事 
	Map * map = ((Map *)ptr);
	map -> cool_down = 3;
}

MapObject * make_ghost(int x, int y){// 將創建一個鬼的功能從主流城中剝離出來
	MapObject * ghost = malloc(sizeof(MapObject));

	ghost = malloc(sizeof(MapObject));
	ghost -> x = x;
	ghost -> y = y;
	ghost -> repr = 'G';
	ghost -> collide = &collide_with_ghost; // functional pointer

	return ghost;

}

Map * map_init(){
	srand((unsigned int)time(NULL)); // 讓隨機更隨機
	Map * map = malloc(sizeof(Map)); // 動態分配一塊記憶體給 Map
	if (map == NULL)
		return NULL;

	int x = 4, y = 4;

	do{
		printf("Plz enter the size of the world border: ");
		scanf("%d %d", &x, &y); // 跟玩家確認地圖長寬
		putchar('\n');
	} while ((x < 5 || y < 5));

	map -> alive = 1;
	map -> x = x;
	map -> y = y;

	map -> player = malloc(sizeof(MapObject));
	map -> player -> repr = 'P';
	map -> player -> x = 3;
	map -> player -> y = 3;

	map -> enemy =  malloc(sizeof(MapObject *) * 3);
	map -> enemy[0] = make_ghost(2, 2);

	map -> booster =  malloc(sizeof(MapObject *) * 3);
	map -> booster[0] = malloc(sizeof(MapObject));
	map -> booster[0] -> repr = 'B';
	map -> booster[0] -> x = 0;
	map -> booster[0] -> y = 0;
	map -> booster[0] -> collide = &collide_with_freeze;
	printf("%d %d %d", sizeof(map -> enemy) / sizeof(map -> enemy[0]), sizeof(map -> enemy), sizeof(map -> enemy[0]));
	return map;
}

void map_update(Map * map){
	printf("\e[1;1H\e[2J");
	char ** ptr;
	char * temp;
	// 動態分配出二維陣列當地圖
	ptr = malloc(map -> x * sizeof(char *));
	for(int x = 0; x < map -> x; x++)
		ptr[x] = malloc(map -> y * sizeof(char));

	for(int y = 0; y < map -> y; y++){
		for(int x = 0; x < map -> x; x++){
			ptr[x][y] = '*'; // 把地圖全部填上空白元素
		}
	}

	for(int i = 0; i < sizeof(map -> booster) / sizeof(map -> booster[0]); i++){
		if(map -> booster[i] != NULL){
			ptr[map -> booster[i] -> x][map -> booster[i] -> y] = map -> booster[i] -> repr; // 把 booster 放上地圖
		}
	}

	if (!map -> cool_down){
		int a = 0;
		
		for(int i = 0; i < sizeof(map -> enemy) / sizeof(map -> enemy[0]); i++){
			if(map -> enemy[i] != NULL){
				while (1){
					a = rand() % 4;

					if (a == 0){
						if (map -> enemy[i] -> x - 1 >= 0 && ptr[map -> enemy[i] -> x - 1][ map -> enemy[i] -> y] != 'B'){
							map -> enemy[i] -> x -= 1;
							break;
						}
					}

					else if (a == 1){
						if ((map -> enemy[i] -> y) + 1 < map -> y && ptr[map -> enemy[i] -> x][ map -> enemy[i] -> y + 1] != 'B'){
							map -> enemy[i] -> y += 1;
							break;
						}
					}

					else if (a == 2){
						if ((map -> enemy[i] -> x) + 1 < map -> x && ptr[map -> enemy[i] -> x + 1][ map -> enemy[i] -> y] != 'B'){
							map -> enemy[i] -> x += 1;
							break;
						}
					}

					else if (a == 3){
						if ((map -> enemy[i] -> y) - 1 >= 0 && ptr[map -> enemy[i] -> x][ map -> enemy[i] -> y - 1] != 'B'){
							map -> enemy[i] -> y -= 1;
							break;
						}
					}
				}
				if((map -> enemy[i] -> x == map -> player -> x) && (map -> enemy[i] -> y == map -> player -> y)){
					map -> enemy[i] -> collide(map);
				}
			}
		}
	}

	else{

		for(int i = 0; i < sizeof(map -> enemy) / sizeof(map -> enemy[0]); i++){
			if(map -> enemy[i] != NULL){
				if((map -> enemy[i] -> x == map -> player -> x) && (map -> enemy[i] -> y == map -> player -> y)){
					free(map -> enemy[i]);
					map -> enemy[i] = NULL;
				}
			}
		}

		--(map -> cool_down);
	}

	for(int i = 0; i < sizeof(map -> booster) / sizeof(map -> booster[0]); i++){

		if(map -> booster[i] != NULL){
			if((map -> booster[i] -> x == map -> player -> x) && (map -> booster[i] -> y == map -> player -> y)){
				collide_with_freeze(map);
				free(map -> booster[i]);
				map -> booster[i] = NULL;
			}
		}

	}
	
	ptr[map -> player -> x][map -> player -> y] = map -> player -> repr;

	for(int i = 0; i < sizeof(map -> enemy) / sizeof(map -> enemy[0]); i++){

		if(map -> enemy[i] != NULL){
			ptr[map -> enemy[i] -> x][map -> enemy[i] -> y] = map -> enemy[i] -> repr;
		}

	}

	for(int y = 0; y < map -> y; y++){

		for(int x = 0; x < map -> x; x++){
			if(ptr[x][y]){
				putchar(ptr[x][y]);
				putchar(' ');
			}
		}

		putchar('\n');
	}

	for(int x = 0; x < map -> x; x++)
		free(ptr[x]);
	free(ptr);
}

void map_run(Map * map){
	char movement;

	while (1){
		scanf("%c", &movement);

		if (movement == 'a'){
			if (map -> player -> x - 1 >= 0){
				map -> player -> x -= 1;
				break;
			}
		}

		else if (movement == 's'){
			if ((map -> player -> y) + 1 < map -> y){
				map -> player -> y += 1;
				break;
			}
		}

		else if (movement == 'd'){
			if ((map -> player -> x) + 1 < map -> x){
				map -> player -> x += 1;
				break;
			}
		}

		else if (movement == 'w'){
			if ((map -> player -> y) - 1 >= 0){
				map -> player -> y -= 1;
				break;
			}
		}
	}

	map_update(map);
}

int main(){
	Map * map = map_init();
	while (map -> alive)
	{
		map_run(map);
	}
	return 0;
}