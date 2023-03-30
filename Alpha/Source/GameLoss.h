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