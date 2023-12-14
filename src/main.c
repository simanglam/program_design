#include <stdio.h>
#include <stdlib.h>
#include "include/raylib.h"
#include "include/render.h"

int main(){
    Gui * game = MyGuiInit();

    while (!WindowShouldClose()){
        game -> Render(game);
    }
    FreeGui(game);
    CloseAudioDevice();
    CloseWindow();
    
}