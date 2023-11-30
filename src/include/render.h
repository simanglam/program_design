/* This file was automatically generated.  Do not edit! */
#undef INTERFACE
void RenderMenuBar(void);
void RenderAbout();
void RenderMusicMenu();
void RenderGameMenu(void);
void RenderGame();
void RenderMainMenu(void);
void MYrender(void);
void MyGuiInit();
extern const int WindowHeight;
extern const int WindowWidth;

typedef struct gui
{
    int Scene;
    int WorldX;
    int WorldY;
    int BeansAmount;
    int BeansDensity;
    int BoosterAmount;
    int BoosterDensity;
    float SliderBarValue;
    void (*render)(struct gui *);
} Gui;

