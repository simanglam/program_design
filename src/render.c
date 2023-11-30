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

const int WindowWidth = 800;
const int WindowHeight = 600;

static const Rectangle SelectorRect = {100, 100, 600, 400};

static int Scene = 0; // trace current scene

static int WorldX = 5;
static int WorldY = 5;
static int BeansAmount = 1;
static int BeansDensity = 1;
static int BoosterAmount = 1;
static int BoosterDensity = 1;
static float SliderBarValue = 100;

enum GameMenu{
    WorldLabel = 1, WorldXLabel, WorldYLabel, BeansLabel, BeansAmountLabel, BeansDensityLabel, BoosterLabel, BoosterAmountLabel, BoosterDensityLabel
};

enum MusicMenu{
MusicLabel, VolumeLabel
};

void MyGuiInit(){ // Init Gui Relate thing
    InitWindow(WindowWidth, WindowHeight, "Pac Man");
    SetTargetFPS(50);
    GuiLoadStyleCyber();
    MyAudioInit();
    //PlayMusicStream(bg_music);
}

void MYrender(void){ // Interface for main function to call
    AudioUpdate();
    BeginDrawing();
        ClearBackground(BLACK);
        switch (Scene)
        {
            case 0:
                RenderMainMenu();
                break;
            case 1:
                RenderGame();
                break;
            case 2:
                RenderGameMenu();
                break;
            case 3:
                RenderMusicMenu();
                break;
            case 4:
                RenderAbout();
                break;
            case 5:
                RenderGame();
                break;
        }
    EndDrawing();
}

void RenderMenuBar(void){ // render menubar
    DrawRectangle(0, 0, 800, 50, (Color){9, 54, 70, 255});
    if(GuiButton((Rectangle){0, 0, 100, 50}, "Main")) Scene = 0;
    if(GuiButton((Rectangle){100, 0, 100, 50}, "Theme")) {Scene = 1; map_init(WorldX, WorldY);}
    if(GuiButton((Rectangle){200, 0, 100, 50}, "Game")) Scene = 2;
    if(GuiButton((Rectangle){300, 0, 100, 50}, "Music")) Scene = 3;
    if(GuiButton((Rectangle){400, 0, 100, 50}, "About")) Scene = 4;
}

void RenderMainMenu(void){ // render main menu
    RenderMenuBar();
    DrawRectangle(100, 200, 600, 100, (Color){9, 54, 70, 255});
    GuiSetStyle(DEFAULT, TEXT_SIZE, Huge);
    GuiDrawText(GAMENAME, (Rectangle){100, 200, 600, 100}, TEXT_ALIGN_MIDDLE, WHITE);
    GuiSetStyle(DEFAULT, TEXT_SIZE, huge);
    if(GuiButton((Rectangle){250, 400, 300, 75}, "START" )) {Scene = 1; map_init(WorldX, WorldY);}
    GuiSetStyle(DEFAULT, TEXT_SIZE, normal);
}

void RenderGameMenu(void){
    RenderMenuBar();
    GuiSetStyle(DEFAULT, TEXT_SIZE, Large);
    DrawRectangleRec(SelectorRect, (Color){9, 54, 70, 255});
    
    GuiDrawText("World", (Rectangle){SelectorRect.x + 25 , SelectorRect.y + SPACING * WorldLabel, 125, 25}, TEXT_ALIGN_LEFT, WHITE);

    GuiDrawText("World Width ", (Rectangle){SelectorRect.x + 50, SelectorRect.y + SPACING * WorldXLabel, 125, 25}, TEXT_ALIGN_LEFT, WHITE);
    GuiSpinner((Rectangle){SelectorRect.x + 175, SelectorRect.y + SPACING * WorldXLabel, 125, 25}, NULL, &WorldX, 5, 40, false);

    GuiDrawText("World Height", (Rectangle){SelectorRect.x + 50, SelectorRect.y + SPACING * WorldYLabel, 125, 25}, TEXT_ALIGN_LEFT, WHITE);
    GuiSpinner((Rectangle){SelectorRect.x + 175, SelectorRect.y + SPACING * WorldYLabel, 125, 25}, NULL, &WorldY, 5, 40, false);

    GuiDrawText("Beans", (Rectangle){SelectorRect.x + SPACING, SelectorRect.y + SPACING * BeansLabel, 125, 25}, TEXT_ALIGN_LEFT, WHITE);

    GuiDrawText("Amount", (Rectangle){SelectorRect.x + 50, SelectorRect.y + SPACING * BeansAmountLabel, 125, 25}, TEXT_ALIGN_LEFT, WHITE);
    GuiComboBox((Rectangle){SelectorRect.x + 150, SelectorRect.y + SPACING * BeansAmountLabel, 125, 25}, "Small;Default;Many", &BeansAmount);

    GuiDrawText("Density", (Rectangle){SelectorRect.x + 50, SelectorRect.y + SPACING * BeansDensityLabel, 100, 25}, TEXT_ALIGN_LEFT, WHITE);
    GuiComboBox((Rectangle){SelectorRect.x + 150, SelectorRect.y + SPACING * BeansDensityLabel, 125, 25}, "Small;Default;Many", &BeansDensity);

    GuiDrawText("Booster", (Rectangle){SelectorRect.x + 25, SelectorRect.y + SPACING * BoosterLabel, 100, 25}, TEXT_ALIGN_LEFT, WHITE);

    GuiDrawText("Amount", (Rectangle){SelectorRect.x + 50, SelectorRect.y + SPACING * BoosterAmountLabel, 100, 25}, TEXT_ALIGN_LEFT, WHITE);
    GuiComboBox((Rectangle){SelectorRect.x + 150, SelectorRect.y + SPACING * BoosterAmountLabel , 125, 25}, "Small;Default;Many", &BoosterAmount);

    GuiDrawText("Density", (Rectangle){SelectorRect.x + 50, SelectorRect.y + SPACING * BoosterDensityLabel, 100, 25}, TEXT_ALIGN_LEFT, WHITE);
    GuiComboBox((Rectangle){SelectorRect.x + 150, SelectorRect.y + SPACING * BoosterDensityLabel, 125, 25}, "Small;Default;Many", &BoosterDensity);

    GuiSetStyle(DEFAULT, TEXT_SIZE, normal);
}

void RenderMusicMenu(){
    RenderMenuBar();
    DrawRectangleRec(SelectorRect, (Color){9, 54, 70, 255});
    GuiSetStyle(DEFAULT, TEXT_SIZE, Large);

    GuiDrawText("Volume", (Rectangle){SelectorRect.x + 25, SelectorRect.y + SPACING * VolumeLabel, 100, 25}, TEXT_ALIGN_LEFT, WHITE);
    GuiSliderBar((Rectangle){SelectorRect.x + 125, SelectorRect.y + SPACING * VolumeLabel, 100, 25}, NULL, TextFormat("%i", (int)SliderBarValue), &SliderBarValue, 0, 200);

    GuiSetStyle(DEFAULT, TEXT_SIZE, normal);

    if(GuiButton((Rectangle){SelectorRect.x + 25, SelectorRect.y + SPACING * 3, 100, 25},"Apply")){
        SetVolume(SliderBarValue / 100);
    };
}

void RenderAbout(){
    RenderMenuBar();
    GuiSetStyle(DEFAULT, TEXT_SIZE, Large);
    DrawRectangleRec(SelectorRect, (Color){9, 54, 70, 255});
    GuiDrawText("Pac-Man\nMade by Simanglam.\nUsing Raylib and Raygui for GUI.\nFinal Project of Program Design I.", SelectorRect, TEXT_ALIGN_CENTER, WHITE);
    GuiSetStyle(DEFAULT, TEXT_SIZE, normal);
}


void RenderGame(){
    if(IsGameAlive())
	    map_run();
    else if (IsGameWin()){
        Scene = 0;
    }
    else{
        Scene = 0;
    }
}