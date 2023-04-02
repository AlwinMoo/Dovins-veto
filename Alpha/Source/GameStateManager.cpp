#include "AEEngine.h"
#include "GameStateManager.h"
#include "GameStateList.h"
#include "Bevantest.h"
#include "TestScene.h"
#include "Alwintest.h"
#include "Splashscreen.h"
#include "Menu.h"
#include "Credits.h"
#include "GameLoss.h"
#include "Tutorial.h"

int current = 0, previous = 0, next = 0;

FP fpLoad = nullptr, fpInitialize = nullptr, fpUpdate = nullptr, fpDraw = nullptr, fpFree = nullptr, fpUnload = nullptr;

void GSM_Initialize(int startingState)
{
	current = previous = next = startingState;
}

void GSM_Update()
{
	switch (current)
	{
		case GS_LEVEL1:
			fpLoad = Alwintest_Load;
			fpInitialize = Alwintest_Initialize;
			fpUpdate = Alwintest_Update;
			fpDraw = Alwintest_Draw;
			fpFree = Alwintest_Free;
			fpUnload = Alwintest_Unload;
			break;
		case GS_SPLASH:
			fpLoad = splashscreen_Load;
			fpInitialize = splashscreen_Initialize;
			fpUpdate = splashscreen_Update;
			fpDraw = splashscreen_Draw;
			fpFree = splashscreen_Free;
			fpUnload = splashscreen_Unload;
			break;
		case GS_MENU:
			fpLoad = menu_Load;
			fpInitialize = menu_Initialize;
			fpUpdate = menu_Update;
			fpDraw = menu_Draw;
			fpFree = menu_Free;
			fpUnload = menu_Unload;
			break;
		case GS_CREDITS:
			fpLoad = credits_Load;
			fpInitialize = credits_Initialize;
			fpUpdate = credits_Update;
			fpDraw = credits_Draw;
			fpFree = credits_Free;
			fpUnload = credits_Unload;
			break;
		case GS_GAMELOSS:
			fpLoad = gameLoss_Load;
			fpInitialize = gameLoss_Initialize;
			fpUpdate = gameLoss_Update;
			fpDraw = gameLoss_Draw;
			fpFree = gameLoss_Free;
			fpUnload = gameLoss_Unload;
			break;
		case GS_TUTORIAL:
			fpLoad = tutorial_Load;
			fpInitialize = tutorial_Initialize;
			fpUpdate = tutorial_Update;
			fpDraw = tutorial_Draw;
			fpFree = tutorial_Free;
			fpUnload = tutorial_Unload;
			break;
		case GS_RESTART:
			break;
		case GS_QUIT:
			break;
		default:
			break;
	}
}