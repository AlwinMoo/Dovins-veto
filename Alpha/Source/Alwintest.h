#pragma once
#include "GameObject.h"
#include "AEEngine.h"
#include "Map/Map.h"
#include "UI_Manager.h"

void Alwintest_Load();

void Alwintest_Initialize();

void Alwintest_Update();

void Alwintest_Draw();

void Alwintest_Free();

void Alwintest_Unload();

namespace
{
	std::vector<GameObject*> go_list;
	game_map* test_map;
	int object_count;
	GameObject* hoverStructure;
	bool validPlacement;
	UI::UI_Manager* uiManager;

	// Textures
	AEGfxTexture* turretTex;
	AEGfxTexture* grassTex;
	AEGfxTexture* nexusTex;
	AEGfxTexture* wallTex;
	AEGfxTexture* eraseTex;
	AEGfxTexture* grassBorderlessTex;
	AEGfxTexture* bulletTex;
	AEGfxTexture* playerTex;
	AEGfxTexture* enemyTex;
	AEGfxTexture* texTest;
	AEGfxVertexList* meshTest;

	UI::Button* nexusButton;
	bool nexusPlaced;
	bool buildPhase;

	int buildResource;
}
