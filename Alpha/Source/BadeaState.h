/******************************************************************************/
/*!
\file		BadeaState.h
\author		Alwin Moo
\author		Alvin Yeo
\author		Bevan Lim
\author		Alonzo Nalpon
\par        email: moo.g@digipen.edu
\par        email: a.yeo@digipen.edu
\par        email: bevanzhiwei.lim@digipen.edu
\par        email: a.nalpon@digipen.edu
\date       January 10, 2023
\brief		Alwin (30%), Alvin (30%), Bevan (20%), Alonzo (20%)
			Alwin: AI, FetchGo, Balancing, Pathfinding
			Alvin: Game Flow, Build Phase, Death Phase, Particles, Sound
			Bevan: player skills
			Alonzo: UI
			Main game loop and functions

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#pragma once
#include "GameObject.h"
#include "AEEngine.h"
#include "Map/Map.h"
#include "UI_Manager.h"
#include "UI_TextAreaTable.h"

void BadeaState_Load();

void BadeaState_Initialize();

void BadeaState_Update();

void BadeaState_Draw();

void BadeaState_Free();

void BadeaState_Unload();

