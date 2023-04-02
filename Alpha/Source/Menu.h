/******************************************************************************/
/*!
\file		Menu.h
\author		Bevan Lim Zhi Wei -> Initialized buttons, positions, gamestate manager
\author		Alonzo Nalpon -> Quit UI overlay, prompting for confirmation to quitting
\par        email: bevanzhiwei.lim@digipen.edu
\par		email: a.nalpon@digipen.edu
\date       March 29, 2023
\brief		Bevan (Primary 70%), Alonzo (Secondary 30%)

This source file contains the necessary functions declarations
to load the Menu state for Badea2

start_button changes game state to our first game level

tutorial_button changes game state to our how to play level

credits_button changes game state to credits

exit_button prompts you for confirmation then exits the game when confirmed

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#pragma once
#include "GameStateManager.h"
#include "GameStateList.h"
#include "AEEngine.h"

void menu_Load();

void menu_Initialize();

void menu_Update();

void menu_Draw();

void menu_Free();

void menu_Unload();

void generic_draw(AEGfxVertexList*, AEGfxTexture*, f32, f32, f32, f32, f32);

AEGfxVertexList* basic_mesh();