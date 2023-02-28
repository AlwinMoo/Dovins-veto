#ifndef TEST_SCENE_H
#define TEST_SCENE_H
#include "GameObject.h"
#include "AEEngine.h"
#include "Map/Map.h"
#include "UI_Manager.h"
void TestScene_Load();

void TestScene_Initialize();

void TestScene_Update();

void TestScene_Draw();

void TestScene_Free();

void TestScene_Unload();

namespace
{
	std::vector<GameObject*> go_list;
	game_map* test_map;
	int object_count;
	GameObject* hoverStructure;
	bool validPlacement;
	UI::UI_Manager* uiManager;

	// Textures
	AEGfxTexture*		turretTex;
	AEGfxTexture*		grassTex;
	AEGfxTexture*		nexusTex;
	AEGfxTexture*		wallTex;
	AEGfxTexture*		grassBorderlessTex;
	AEGfxTexture*		bulletTex;
	AEGfxTexture*		playerTex;
	AEGfxTexture*		enemyTex;
	AEGfxTexture*		texTest;
	AEGfxVertexList*	meshTest;

	UI::Button* nexusButton;
	bool nexusPlaced;
	bool buildPhase;
}

#endif // !TEST_SCENE_H
