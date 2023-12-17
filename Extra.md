# Basic part

## Map system

Game map is store in a 2D array of pointer to a MapObject, a customized struct defined in game.h. Location of an object on map is randomly determined by using `rand()` fuction.

### Map object

MapObject is a customized struct defined in game.h. Here is the definition:

```c
typedef struct mapobject{
	int x, y;
	char repr;
	void (*collide)(void * map);
} MapObject;
```

The x & y is to store current postion. The repr is representative of the object on the map. The collide is a function pointer to provide scalability for future.

## Booster system

Three different booster are implemented:

* Freeze booster
* Extra life booster
* Less Monster booster

# Extra part

* Fully functional GUI.
* Self made music.