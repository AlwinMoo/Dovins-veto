/******************************************************************************/
/*!
\file		Tutorial.h
\author		Bevan Lim Zhi Wei
\par        email: bevanzhiwei.lim\@digipen.edu
\date       April 02, 2023
\brief		Bevan(Primary 100%)

This source file contains the necessary function declarations for the execution
of the tutorial segment of Badea 2.

The tutorial segment shows a series of pictures depicting the basics of Badea 2

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
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