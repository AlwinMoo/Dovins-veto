/******************************************************************************/
/*!
\file		Alwintest.h
\author		Alwin Moo
\author		Alvin Yeo
\author		Bevan Lim
\author		Alonzo Nalpon
\par        email: moo.g\@digipen.edu
\par        email: a.yeo\@digipen.edu
\par        email: \@digipen.edu
\par        email: \@digipen.edu
\date       January 10, 2023
\brief		Alwin (25%), Alvin (25%), Bevan (25%), Alonzo (25%)
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

void Alwintest_Load();

void Alwintest_Initialize();

void Alwintest_Update();

void Alwintest_Draw();

void Alwintest_Free();

void Alwintest_Unload();

