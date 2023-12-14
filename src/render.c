#include <stdio.h>
#include <stdlib.h>

#include "include/raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "include/raygui.h"

#include "styles/ashes/style_ashes.h"
#include "styles/bluish/style_bluish.h"
#include "styles/candy/style_candy.h"
#include "styles/cherry/style_cherry.h"
#include "styles/cyber/style_cyber.h"
#include "styles/dark/style_dark.h"
#include "styles/enefete/style_enefete.h"
#include "styles/jungle/style_jungle.h"
#include "styles/lavanda/style_lavanda.h"
#include "styles/sunny/style_sunny.h"
#include "styles/terminal/style_terminal.h"

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
	WorldLabel = 1, WorldXLabel, WorldYLabel, BeansLabel, BeansAmountLabel, BoosterLabel, BoosterAmountLabel
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
	myGui -> Theme = 4;
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
	if(GuiButton((Rectangle){100, 0, 100, 50}, "Theme")) myGui -> Scene = &RenderTheme;
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

void RenderTheme(Gui * myGui){
	RenderMenuBar(myGui);

	DrawRectangleRec(SelectorRect, (Color){9, 54, 70, 255});
	GuiSetStyle(DEFAULT, TEXT_SIZE, Large);

	GuiDrawText("Theme", (Rectangle){SelectorRect.x + 25, SelectorRect.y + SPACING * VolumeLabel, 100, 25}, TEXT_ALIGN_LEFT, WHITE);
	GuiComboBox((Rectangle){SelectorRect.x + 125, SelectorRect.y + SPACING * VolumeLabel, 200, 25}, "Ashes;Bluish;Candy;Cherry;Cyber;Dark;Enefete;Jungle;Lavanda;Sunny;Terminal", &(myGui -> Theme));

	GuiSetStyle(DEFAULT, TEXT_SIZE, normal);

	if(GuiButton((Rectangle){SelectorRect.x + 25, SelectorRect.y + SPACING * 3, 100, 25},"Apply")){
		switch (myGui -> Theme){
			case 0:
				GuiLoadStyleAshes();
				break;
			case 1:
				GuiLoadStyleBluish();
				break;
			case 2:
				GuiLoadStyleCandy();
				break;
			case 3:
				GuiLoadStyleCherry();
				break;
			case 4:
				GuiLoadStyleCyber();
				break;
			case 5:
				GuiLoadStyleDark();
				break;
			case 6:
				GuiLoadStyleEnefete();
				break;
			case 7:
				GuiLoadStyleJungle();
				break;
			case 8:
				GuiLoadStyleLavanda();
				break;
			case 9:
				GuiLoadStyleSunny();
				break;
			case 10:
				GuiLoadStyleTerminal();
				break;
		
			default:
				break;
		}
	};

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

	GuiDrawText("Booster", (Rectangle){SelectorRect.x + 25, SelectorRect.y + SPACING * BoosterLabel, 100, 25}, TEXT_ALIGN_LEFT, WHITE);

	GuiDrawText("Amount", (Rectangle){SelectorRect.x + 50, SelectorRect.y + SPACING * BoosterAmountLabel, 100, 25}, TEXT_ALIGN_LEFT, WHITE);
	GuiComboBox((Rectangle){SelectorRect.x + 150, SelectorRect.y + SPACING * BoosterAmountLabel , 125, 25}, "Small;Default;Many", &(myGui -> BoosterAmount));

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
		myGui -> Scene = &RenderResult;
		return ;
	}
	MapRun(myGui -> map);
	
	for(int y = 0; y < myGui -> map -> y; y++){
		for(int x = 0; x < myGui ->map -> x; x++){
			if(myGui -> map -> world[x][y] != NULL){
				GuiDrawText(&myGui -> map -> world[x][y] -> repr, (Rectangle){xOffset + x * 10, yOffset + y * 10, 25, 25}, TEXT_ALIGN_CENTER, (myGui -> map -> world[x][y] -> repr == 'B') ? SKYBLUE : WHITE);
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
	temp[3] = (char)((int)temp[3] + myGui -> map -> alive / 10);
	temp[4] = (char)((int)temp[3] + myGui -> map -> alive % 10);

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

	GuiDrawText(temp, (Rectangle){xOffset + 10 * myGui -> map -> x, yOffset - 25, 40, 25}, TEXT_ALIGN_LEFT, WHITE);
	free(temp);
}

void RenderResult(Gui * myGui){
	GuiSetStyle(DEFAULT, TEXT_SIZE, Huge);

	GuiDrawText((myGui -> map -> alive) ? "You Win!" : "You Lose", (Rectangle){WindowWidth / 2 - 125, WindowHeight / 2 - 100, 250, 200}, TEXT_ALIGN_CENTER, WHITE);

	if (GuiButton((Rectangle){WindowWidth / 2 - 25, WindowHeight * 3 / 4 - 25, 50, 50}, GuiIconText(ICON_HOUSE, NULL))){
		myGui -> Scene = &RenderMainMenu;
		FreeMap(myGui -> map);
		myGui -> map = NULL;
	}

	GuiSetStyle(DEFAULT, TEXT_SIZE, normal);
}

void FreeGui(Gui *myGui){
	if (myGui -> map != NULL){
		FreeMap(myGui -> map);
	}
	free(myGui);
}