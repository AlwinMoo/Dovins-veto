/******************************************************************************/
/*!
\file		Splashscreen.h
\author		Bevan Lim Zhi Wei
\par        email: bevanzhiwei.lim\@digipen.edu
\date       April 02, 2023
\brief		Bevan (Primary 100%)

This source file contains the necessary functions declarations
to execute the splashscreen portion of Badea 2.

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#pragma once
#include "AEEngine.h"
#include "GameStateManager.h"
#include "GameStateList.h"

void splashscreen_Load();

void splashscreen_Initialize();

void splashscreen_Update();

void splashscreen_Draw();

void splashscreen_Free();

void splashscreen_Unload();
