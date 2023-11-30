/* This file was automatically generated.  Do not edit! */
#undef INTERFACE

typedef struct mapobject{
	int x, y;
	char repr;
	void (*collide)(void * map);
} MapObject;

typedef struct map{
    int cool_down;
    int x, y;
    int alive;
    MapObject ** enemy;
    MapObject ** booster;
    MapObject * player;
} Map;

int IsGameAlive();
int IsGameWin();
void map_run();
void map_update();
void map_init(int x,int y);
void map_print();
void collide_with_freeze(void *ptr);
void collide_with_ghost(void *ptr);
