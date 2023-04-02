/******************************************************************************/
/*!
\file		Credits.h
\author		Bevan Lim Zhi Wei
\par        email: bevanzhiwei.lim@digipen.edu
\date       March 30, 2023
\brief		Bevan (Primary 100%)
This source file contains the necessary functions declarations for the execution
of the credits sequence for Badea 2

The credits sequence displays the people who made the game as well as our thanks to
those who have supported us

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#pragma once
#include "GameStateManager.h"
#include "GameStateList.h"
#include "AEEngine.h"
#include "Menu.h"

void credits_Load();

void credits_Initialize();

void credits_Update();

void credits_Draw();

void credits_Free();

void credits_Unload();