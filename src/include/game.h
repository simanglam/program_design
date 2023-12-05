/* This file was automatically generated.  Do not edit! */
#undef INTERFACE
#ifndef MAPOBJECT
#define MAPOBJECT
typedef struct mapobject{
	int x, y;
	char repr;
	void (*collide)(void * map);
} MapObject;

typedef struct map{
    int cool_down;
    int x, y;
    int alive;
    int playerScore;
    int winScore;
    MapObject *** world;
    MapObject ** enemy;
    MapObject * player;
} Map;

#endif

void MapRun(Map *map);
void MapUpdate(Map *map);
Map * MapInit(int x,int y, int BeansAmount, int BoosterAmount);
void CollideWithFrezze(void *ptr);
void CollideWithGhost(void *ptr);
void FreeMap(Map * map);
