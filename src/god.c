#include <stdio.h>
#include <stdlib.h>

#include "include/raylib.h"
#include "include/render.h"
#include "include/audio.h"
#include "include/game.h"

typedef struct game
{
    Map * map;
    Gui * gui;
} Game;
