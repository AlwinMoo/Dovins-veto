/******************************************************************************/
/*!
\file		GameLoss.h
\author		Bevan Lim Zhi Wei -> Initialized buttons, positions, gamestate manager
\author		Alonzo Nalpon -> Quit UI overlay, prompting for confirmation to quitting
\par        email: bevanzhiwei.lim@digipen.edu
\par		email: a.nalpon@digipen.edu
\date       April 02, 2023
\brief		Bevan (Primary 70%), Alonzo (Secondary 30%)

This source file contains the necessary functions declaration
to load the GameLoss state for Badea2

res_button changes game state to our first game level

credits_button changes game state to credits

Menu_button prompts you for confirmation then exits the game when confirmed

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#pragma once
#include "GameStateList.h"
#include "GameStateManager.h"
#include "AEEngine.h"
#include "Menu.h"
#include "Alwintest.h"

extern bool playerloss;

void gameLoss_Load();

void gameLoss_Initialize();

void gameLoss_Update();

void gameLoss_Draw();

void gameLoss_Free();

void gameLoss_Unload();