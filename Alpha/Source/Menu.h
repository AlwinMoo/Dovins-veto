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