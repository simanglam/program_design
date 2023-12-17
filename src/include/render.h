/* This file was automatically generated.  Do not edit! */
#undef INTERFACE
#include <game.h>

#ifndef GUI
#define GUI
typedef struct gui{
    int WorldX;
    int WorldY;
    int Theme;
    int BeansAmount;
    int BeansDensity;
    int BoosterAmount;
    int BoosterDensity;
    float SliderBarValue;
    void (*Scene)(struct gui *); // state 設計模式，為未來保留可擴充性
    void (*Render)(struct gui *); // 為未來保留可擴充性
    Map * map;
} Gui;

#endif

void RenderMenuBar(Gui *myGui);
void RenderAbout(Gui *myGui);
void RenderMusicMenu(Gui *myGui);
void RenderGameMenu(Gui *myGui);
void RenderGame(Gui *myGui);
void RenderMainMenu(Gui *myGui);
void Myrender(Gui *myGui);
void RenderTheme(Gui *myGui);
void RenderResult(Gui *myGui);
void FreeGui(Gui *myGui);
Gui *MyGuiInit();
