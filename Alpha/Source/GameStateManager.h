/******************************************************************************/
/*!
\file		GameStateManager.h
\author		Bevan Lim
\author		Alonzo Nalpon
\par        email: bevanzhiwei.lim@digipen.edu
\par		email: a.nalpon@digipen.edu
\date       January 26, 2023
\brief		Bevan (Primary 50%), Alonzo (Secondary 50%)

This source file contains the function declaration for the game state manager
for Badea 2

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#pragma once
typedef void(*FP)(void);

extern int current, previous, next;

extern FP fpLoad, fpInitialize, fpUpdate, fpDraw, fpFree, fpUnload;

void GSM_Initialize(int startingState);

void GSM_Update();