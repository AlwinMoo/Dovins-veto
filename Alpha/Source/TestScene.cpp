#include "TestScene.h"
#include <iostream>

namespace
{
		GameObject* FetchGO(GameObject::GAMEOBJECT_TYPE value)
		{
			for (auto it : go_list)
			{
				GameObject* go = (GameObject*)it;
				if (!go->active)
				{
					switch (go->type)
					{
					default:
						go->tex = planetTex;
					}
					go->active = true;
					++object_count;
					return go;
				}
			}
			GameObject* go{ new GameObject(value) };
			go_list.push_back(go);

			//CODE TO INITIALISE GO SPECIFIC VARIABLES

			return FetchGO(value);

		}
}

void TestScene_Load()
{
	planetTex = AEGfxTextureLoad("Assets/PlanetTexture.png");
}

void TestScene_Initialize()
{
	srand(time(NULL));

	test_map = new game_map(10, 10, 1600, 900, true);
	hoverStructure = FetchGO(GameObject::GO_HOVER_STRUCTURE);
	hoverStructure->alpha = 0.5f;
	hoverStructure->rotation = rand() % 360;
	hoverStructure->scale.x = test_map->get_tile_size();
	hoverStructure->scale.y = hoverStructure->scale.x;
	validPlacement = false;
}

void TestScene_Update()
{
	// Player Input
	s32 mouseX, mouseY;
	AEInputGetCursorPosition(&mouseX, &mouseY);
	float mouseXGrid = static_cast<int>(mouseX / test_map->get_tile_size()) * test_map->get_tile_size() + (test_map->get_tile_size() * 0.5);
	float mouseYGrid = static_cast<int>(mouseY / test_map->get_tile_size()) * test_map->get_tile_size() + (test_map->get_tile_size() * 0.5);

	if (AEInputCheckTriggered(AEVK_LBUTTON) && validPlacement)
	{
		GameObject* test = FetchGO(GameObject::GO_PLANET);
		test->position.x = mouseXGrid;
		test->position.y = mouseYGrid;
		test->rotation = hoverStructure->rotation;
		hoverStructure->rotation = rand() % 360;
		test->scale.x = test_map->get_tile_size();
		test->scale.y = test->scale.x;
	}

	if (hoverStructure->position.x != mouseXGrid
		|| hoverStructure->position.y != mouseYGrid)
	{
		hoverStructure->position.x = mouseXGrid;
		hoverStructure->position.y = mouseYGrid;
	}

	// GameObject Update
	for (GameObject* gameObj : go_list)
	{
		if (gameObj->active)
		{
			if (gameObj->type == GameObject::GO_PLANET)
				gameObj->Update();
		}
	}

	// GameObject Collision (NON-GRID BASED, SHOULD CHANGE)
	bool hoverCollided = false;
	for (GameObject* gameObj : go_list)
	{
		if (gameObj != hoverStructure)
		{
			if (hoverStructure->position.x == gameObj->position.x &&
				hoverStructure->position.y == gameObj->position.y)
			{
				hoverStructure->color.Set(1.0f, 0.5f, 0.5f);
				hoverCollided = true;
				validPlacement = false;
				break;
			}
		}
	}
	if (!hoverCollided)
	{
		hoverStructure->color.Set(1.0f, 1.0f, 1.0f);
		validPlacement = true;
	}
}

void TestScene_Draw()
{
	//s32 cursorX, cursorY;
	//s8 text[] = "TEST";
	//AEInputGetCursorPosition(&cursorX, &cursorY);
	//f32 fcursorX, fcursorY;
	//
	//AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
	//
	//// IMPORTANT SNIPPET:
	///***********************************************************************/
	//f32 winX{ AEGfxGetWinMaxX() - AEGfxGetWinMinX() }, winY{ AEGfxGetWinMaxY() - AEGfxGetWinMinY() };
	////std::cout << winX << ", " <<  winY << std::endl;
	//f32 cursorXN{ cursorX / winX * 2 - 1.f }, cursorYN{ cursorY / winY * -2 + 1.f }; // NORMALIZED COORDINATES
	//std::cout << cursorXN << ", " << cursorYN << std::endl;
	///************************************************************************/
	//s8 const* testStr = "BUILD PHASE";
	//AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	////AEGfxPrint(m_fontId, (s8*)testStr, cursorXN, cursorYN, 2.f, 1.f, 0.f, 0.f);

	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
	for (GameObject* gameObj : go_list)
	{
		//Gameobjects Render
		if (gameObj->active)
		{
			if (gameObj->type == GameObject::GO_PLANET)
				gameObj->Render();
		}
	}

	// Render above
	hoverStructure->Render();
}

void TestScene_Free()
{
	
}

void TestScene_Unload()
{

}