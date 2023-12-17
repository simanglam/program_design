#include <stdio.h>
#include <stdlib.h>
#include "include/raylib.h"
#include "include/render.h"

int main(){
    Gui * game = MyGuiInit();

    while (!WindowShouldClose()){ // 如果遊戲視窗沒有收到關閉訊號
        game -> Render(game);
    }

    FreeGui(game);
    CloseAudioDevice();
    CloseWindow();
}