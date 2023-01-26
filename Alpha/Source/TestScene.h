#ifndef TEST_SCENE_H
#define TEST_SCENE_H
#include "GameObject.h"
#include "AEEngine.h"
#include "Map/Map.h"

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

	AEGfxTexture* planetTex;
}

#endif // !TEST_SCENE_H
