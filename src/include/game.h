/* This file was automatically generated.  Do not edit! */
#undef INTERFACE
#ifndef MAPOBJECT
#define MAPOBJECT
typedef struct mapobject{
	int x, y;
	char repr;
	void (*collide)(void * map); // 利用 function pionter 讓 collide 事件的實作可以更有彈性
} MapObject;

typedef struct map{
    int cool_down;
    int x, y;
    int alive;
    int playerScore;
    int winScore;
    int enemyCount;
    MapObject *** world; // 儲存世界地圖，讓 collide 偵測比較簡單
    MapObject ** enemy; // 儲存敵人資訊，讓更新時不需再次複製地圖
    MapObject * player;
} Map;

#endif

void MapRun(Map *map);
void MapUpdate(Map *map);
Map * MapInit(int x,int y, int BeansAmount, int BoosterAmount);
void CollideWithFrezze(void *ptr);
void CollideWithGhost(void *ptr);
void FreeMap(Map * map);
