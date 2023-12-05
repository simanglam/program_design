#include <stdio.h>
#include <stdlib.h>

#include "include/raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "include/raygui.h"
#include "styles/dark/style_dark.h"
#include "styles/cyber/style_cyber.h"
#include "include/render.h"
#include "include/audio.h"
#include "include/game.h"

#define GAMENAME "PAC MAN"

#define Huge 60
#define huge 40
#define LARGE 30
#define Large 24
#define large 18
#define normal 16

#define SPACING 25

#define WindowWidth 800
#define WindowHeight 600
static const Rectangle SelectorRect = {100, 100, 600, 400};

enum GameMenu{
    WorldLabel = 1, WorldXLabel, WorldYLabel, BeansLabel, BeansAmountLabel, BeansDensityLabel, BoosterLabel, BoosterAmountLabel, BoosterDensityLabel
};

enum MusicMenu{
MusicLabel, VolumeLabel
};

Gui * MyGuiInit(){ // Init Gui Relate thing
    Gui * myGui = malloc(sizeof(Gui));
    if(myGui == NULL)
        exit(1);
    
    InitWindow(WindowWidth, WindowHeight, "Pac Man");
    SetTargetFPS(50);
    GuiLoadStyleCyber();
    MyAudioInit();
    myGui -> WorldX = 5;
    myGui -> WorldY = 5;
    myGui -> BeansAmount = 1;
    myGui -> BeansDensity = 1;
    myGui -> BoosterAmount = 1;
    myGui -> BoosterDensity = 1;
    myGui -> SliderBarValue = 100;
    myGui -> Scene = &RenderMainMenu;
    myGui -> Render = &Myrender; 
    return myGui;
    //PlayMusicStream(bg_music);
}

void Myrender(Gui * myGui){ // Interface for main function to call
    AudioUpdate();
    BeginDrawing();
        ClearBackground(BLACK);
        myGui -> Scene(myGui);
    EndDrawing();
}

void RenderMenuBar(Gui * myGui){ // render menubar
    DrawRectangle(0, 0, 800, 50, (Color){9, 54, 70, 255});
    if(GuiButton((Rectangle){0, 0, 100, 50}, "Main")) myGui -> Scene = &RenderMainMenu;
    //if(GuiButton((Rectangle){100, 0, 100, 50}, "Theme")) {myGui -> Scene = &RenderGame; myGui -> map = MapInit(myGui -> WorldX, myGui -> WorldY);}
    if(GuiButton((Rectangle){200, 0, 100, 50}, "Game")) myGui -> Scene = &RenderGameMenu;
    if(GuiButton((Rectangle){300, 0, 100, 50}, "Music")) myGui -> Scene = &RenderMusicMenu;
    if(GuiButton((Rectangle){400, 0, 100, 50}, "About")) myGui -> Scene = &RenderAbout;
}

void RenderMainMenu(Gui * myGui){ // render main menu
    RenderMenuBar(myGui);
    DrawRectangle(100, 200, 600, 100, (Color){9, 54, 70, 255});
    GuiSetStyle(DEFAULT, TEXT_SIZE, Huge);
    GuiDrawText(GAMENAME, (Rectangle){100, 200, 600, 100}, TEXT_ALIGN_MIDDLE, WHITE);
    GuiSetStyle(DEFAULT, TEXT_SIZE, huge);
    if(GuiButton((Rectangle){250, 400, 300, 75}, "START" )){
        myGui -> Scene = &RenderGame; myGui -> map = MapInit(myGui -> WorldX, myGui -> WorldY, myGui -> BeansAmount, myGui -> BoosterAmount);
    }
    GuiSetStyle(DEFAULT, TEXT_SIZE, normal);
}

void RenderGameMenu(Gui * myGui){
    RenderMenuBar(myGui);
    GuiSetStyle(DEFAULT, TEXT_SIZE, Large);
    DrawRectangleRec(SelectorRect, (Color){9, 54, 70, 255});
    
    GuiDrawText("World", (Rectangle){SelectorRect.x + 25 , SelectorRect.y + SPACING * WorldLabel, 125, 25}, TEXT_ALIGN_LEFT, WHITE);

    GuiDrawText("World Width ", (Rectangle){SelectorRect.x + 50, SelectorRect.y + SPACING * WorldXLabel, 125, 25}, TEXT_ALIGN_LEFT, WHITE);
    GuiSpinner((Rectangle){SelectorRect.x + 175, SelectorRect.y + SPACING * WorldXLabel, 125, 25}, NULL, &(myGui -> WorldX), 5, 40, false);

    GuiDrawText("World Height", (Rectangle){SelectorRect.x + 50, SelectorRect.y + SPACING * WorldYLabel, 125, 25}, TEXT_ALIGN_LEFT, WHITE);
    GuiSpinner((Rectangle){SelectorRect.x + 175, SelectorRect.y + SPACING * WorldYLabel, 125, 25}, NULL, &(myGui -> WorldY), 5, 40, false);

    GuiDrawText("Beans", (Rectangle){SelectorRect.x + SPACING, SelectorRect.y + SPACING * BeansLabel, 125, 25}, TEXT_ALIGN_LEFT, WHITE);

    GuiDrawText("Amount", (Rectangle){SelectorRect.x + 50, SelectorRect.y + SPACING * BeansAmountLabel, 125, 25}, TEXT_ALIGN_LEFT, WHITE);
    GuiComboBox((Rectangle){SelectorRect.x + 150, SelectorRect.y + SPACING * BeansAmountLabel, 125, 25}, "Small;Default;Many", &(myGui -> BeansAmount));

    GuiDrawText("Density", (Rectangle){SelectorRect.x + 50, SelectorRect.y + SPACING * BeansDensityLabel, 100, 25}, TEXT_ALIGN_LEFT, WHITE);
    GuiComboBox((Rectangle){SelectorRect.x + 150, SelectorRect.y + SPACING * BeansDensityLabel, 125, 25}, "Small;Default;Many", &(myGui -> BeansDensity));

    GuiDrawText("Booster", (Rectangle){SelectorRect.x + 25, SelectorRect.y + SPACING * BoosterLabel, 100, 25}, TEXT_ALIGN_LEFT, WHITE);

    GuiDrawText("Amount", (Rectangle){SelectorRect.x + 50, SelectorRect.y + SPACING * BoosterAmountLabel, 100, 25}, TEXT_ALIGN_LEFT, WHITE);
    GuiComboBox((Rectangle){SelectorRect.x + 150, SelectorRect.y + SPACING * BoosterAmountLabel , 125, 25}, "Small;Default;Many", &(myGui -> BoosterAmount));

    GuiDrawText("Density", (Rectangle){SelectorRect.x + 50, SelectorRect.y + SPACING * BoosterDensityLabel, 100, 25}, TEXT_ALIGN_LEFT, WHITE);
    GuiComboBox((Rectangle){SelectorRect.x + 150, SelectorRect.y + SPACING * BoosterDensityLabel, 125, 25}, "Small;Default;Many", &(myGui -> BoosterDensity));

    GuiSetStyle(DEFAULT, TEXT_SIZE, normal);
}

void RenderMusicMenu(Gui * myGui){
    RenderMenuBar(myGui);
    DrawRectangleRec(SelectorRect, (Color){9, 54, 70, 255});
    GuiSetStyle(DEFAULT, TEXT_SIZE, Large);

    GuiDrawText("Volume", (Rectangle){SelectorRect.x + 25, SelectorRect.y + SPACING * VolumeLabel, 100, 25}, TEXT_ALIGN_LEFT, WHITE);
    GuiSliderBar((Rectangle){SelectorRect.x + 125, SelectorRect.y + SPACING * VolumeLabel, 100, 25}, NULL, TextFormat("%i", (int)myGui -> SliderBarValue), &(myGui -> SliderBarValue), 0, 200);

    GuiSetStyle(DEFAULT, TEXT_SIZE, normal);

    if(GuiButton((Rectangle){SelectorRect.x + 25, SelectorRect.y + SPACING * 3, 100, 25},"Apply")){
        SetVolume(myGui -> SliderBarValue / 100);
    };
}

void RenderAbout(Gui * myGui){
    RenderMenuBar(myGui);
    GuiSetStyle(DEFAULT, TEXT_SIZE, Large);
    DrawRectangleRec(SelectorRect, (Color){9, 54, 70, 255});
    GuiDrawText("Pac-Man\nMade by Simanglam.\nUsing Raylib and Raygui for GUI.\nFinal Project of Program Design I.", SelectorRect, TEXT_ALIGN_CENTER, WHITE);
    GuiSetStyle(DEFAULT, TEXT_SIZE, normal);
}


void RenderGame(Gui * myGui){
    int xOffset = (WindowWidth / 2) - (10 * myGui -> map -> x / 2), yOffset = (WindowHeight / 2) - (10 * myGui -> map -> y / 2);
    if(!(myGui -> map -> alive) || myGui -> map -> playerScore == myGui -> map -> winScore){
        myGui -> Scene = &RenderMainMenu;
        FreeMap(myGui -> map);
        myGui -> map = NULL;
        return ;
    }
	MapRun(myGui -> map);
    
    for(int y = 0; y < myGui -> map -> y; y++){
        for(int x = 0; x < myGui ->map -> x; x++){
            if(myGui -> map -> world[x][y] != NULL){
                GuiDrawText(&myGui -> map -> world[x][y] -> repr, (Rectangle){xOffset + x * 10, yOffset + y * 10, 25, 25}, TEXT_ALIGN_CENTER, WHITE);
            }
            else if ((x != myGui -> map -> player -> x) || (y != myGui -> map -> player -> y)){
                GuiDrawText("*", (Rectangle){xOffset + x * 10, yOffset + y * 10, 25, 25}, TEXT_ALIGN_CENTER, GRAY);
            }
        }
    }
    GuiDrawText("P", (Rectangle){xOffset + myGui -> map -> player -> x * 10, yOffset + myGui -> map -> player -> y * 10, 25, 25}, TEXT_ALIGN_CENTER, YELLOW);

    char * temp = malloc(sizeof(char) * 5);

    temp[0] = 'H';
    temp[1] = 'P';
    temp[2] = ':';
    temp[3] = '0';
    temp[4] = '0';
    temp[4] = (char)((int)temp[3] + myGui -> map -> alive);

    GuiDrawText(temp, (Rectangle){xOffset - 25, yOffset - 25, 40, 25}, TEXT_ALIGN_RIGHT, WHITE);

    temp[0] = '0';
    temp[1] = '0';
    temp[2] = '/';
    temp[3] = '0';
    temp[4] = '0';
    temp[0] = (char)((int)temp[0] + myGui -> map -> playerScore / 10);
    temp[1] = (char)((int)temp[1] + myGui -> map -> playerScore % 10);
    temp[3] = (char)((int)temp[3] + myGui -> map -> winScore / 10);
    temp[4] = (char)((int)temp[4] + myGui -> map -> winScore % 10);

    GuiDrawText(temp, (Rectangle){xOffset + 25, yOffset - 25, 40, 25}, TEXT_ALIGN_RIGHT, WHITE);
    /*
    for(int i = 0; i < sizeof(myGui -> map -> booster) / sizeof(myGui -> map -> booster[0]); i++){
        if(myGui -> map -> booster[i] != NULL)
            GuiDrawText("B", (Rectangle){myGui -> map -> booster[i] -> x * 10, myGui -> map -> booster[i] -> y * 10, 25, 25}, TEXT_ALIGN_CENTER, WHITE);
    }
    for(int i = 0; i < sizeof(myGui -> map -> enemy) / sizeof(myGui -> map -> enemy[0]); i++){
        if(myGui -> map -> enemy[i] != NULL)
            GuiDrawText("G", (Rectangle){myGui -> map -> enemy[i] -> x * 10, myGui -> map -> enemy[i] -> y * 10, 25, 25}, TEXT_ALIGN_CENTER, WHITE);
    }
    */
}