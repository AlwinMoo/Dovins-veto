#pragma once
#include "GameStateManager.h"
#include "GameStateList.h"
#include "Menu.h"
#include "UI_Manager.h"
#include "UI_TextAreaTable.h"

namespace
{
	enum tutorial
	{
		SCENE1 = 1,
		SCENE2,
		SCENE3,
		SCENE4,
		SCENE5,
		SCENE6,
		SCENE7,
		SCENE8,
		SCENE9,
		SCENE10,
		SCENE11,
		CHANGE
	};
}

void tutorial_Load();

void tutorial_Initialize();

void tutorial_Update();

void tutorial_Draw();

void tutorial_Free();

void tutorial_Unload();