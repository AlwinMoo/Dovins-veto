#include "Bevantest.h"
#include "AEEngine.h"
#include <iostream>
#include <iomanip>
#include "UI_Manager.h"
f32 winSizeX, winSizeY;
s8 m_fontId;


#include <vector>
#include "Map/Map.h"
#include "GameObject.h"
#include "GameStateManager.h"
#include "GameStateList.h"
#include <iostream>

AEGfxTexture* pTex;
AEGfxVertexList* pMesh;
s32 click_pos_x;
s32 click_pos_y;
s32 pos_x, pos_y;
s32 x, y;

namespace
{
	std::vector<GameObject*> go_list;
	game_map* test_map;
	int object_count;

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
					go->tex = pTex;
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

void Bevantest_Load()
{
	pTex = AEGfxTextureLoad("Assets/PlanetTexture.png");
}

void Bevantest_Initialize()
{
	winSizeX = AEGfxGetWinMaxX() - AEGfxGetWinMinX();
	winSizeY = AEGfxGetWinMaxY() - AEGfxGetWinMinY();

	pos_x = 400;
	pos_y = 300;
	click_pos_x = 400;
	click_pos_y = 300;
	x = 0;
	y = 0;
	pMesh = 0;
	AEGfxMeshStart();

	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFFFF00FF, 0.0f, 0.0f,
		0.5f, -0.5f, 0xFFFFFF00, 1.0f, 0.0f,
		-0.5f, 0.5f, 0xFF00FFFF, 0.0f, 1.0f);

	AEGfxTriAdd(
		0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 0.0f,
		0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 1.0f);

	pMesh = AEGfxMeshEnd();

	test_map = new game_map(10, 10, 1600, 900, true); // automatically destroyed in deconstructor

	for (int i = 0; i < test_map->map_size; ++i)
	{
		GameObject* test = FetchGO(GameObject::GO_PLANET);
		test->position = test_map->GetWorldPos(i);
		test->scale.x = test_map->GetTileSize();
		test->scale.y = test->scale.x;
	}
}

void Bevantest_Update()
{
	//// IMPORTANT SNIPPET:
	//	/***********************************************************************/
	//s32 cursorX, cursorY;
	//AEInputGetCursorPosition(&cursorX, &cursorY);
	//f32 winX{ AEGfxGetWinMaxX() - AEGfxGetWinMinX() }, winY{ AEGfxGetWinMaxY() - AEGfxGetWinMinY() };
	////std::cout << winX << ", " <<  winY << std::endl;
	//f32 cursorXN{ cursorX / winX * 2 - 1.f }, cursorYN{ cursorY / winY * -2 + 1.f }; // NORMALIZED COORDINATES
	//std::cout << std::setprecision(2) << std::setw(5) << cursorXN << ", " << std::setw(5) << cursorYN << std::endl;
	///************************************************************************/
	//s8 const* testStr = "TEST TEXT";
	//AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	//AEGfxPrint(m_fontId, (s8*)testStr, cursorXN, cursorYN, 2.f, 1.f, 0.f, 0.f);

	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);

	// Player Input
	s32 mouseX, mouseY;
	AEInputGetCursorPosition(&mouseX, &mouseY);
	if (AEInputCheckTriggered(AEVK_LBUTTON))
	{
		/*GameObject* test = FetchGO(GameObject::GO_PLANET);
		test->position.x = test_map->GetWorldX(static_cast<int>(mouseX / test_map->GetTileSize()));
		test->position.y = test_map->GetWorldY(static_cast<int>(mouseY / test_map->GetTileSize()));
		test->rotation = rand() % 360;
		test->scale.x = test_map->GetTileSize();
		test->scale.y = test->scale.x;*/

		if (test_map->GetIndex(static_cast<int>(mouseX / test_map->GetTileSize()), static_cast<int>(mouseY / test_map->GetTileSize())) < test_map->map_size && test_map->GetIndex(static_cast<int>(mouseX / test_map->GetTileSize()), static_cast<int>(mouseY / test_map->GetTileSize())) >= 0)
		{
			std::cout << "Legal: " << test_map->GetIndex(static_cast<int>(mouseX / test_map->GetTileSize()), static_cast<int>(mouseY / test_map->GetTileSize())) << std::endl;
		}
		else
		{
			std::cout << "Illegal" << std::endl;
		}
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

	if (AEInputCheckTriggered(AEVK_RBUTTON))
	{
		AEInputGetCursorPosition(&click_pos_x, &click_pos_y);
		//click_check(click_pos_x, click_pos_y);
		//click_check(pos_x, pos_y);
	}
	if (AEInputCheckTriggered(AEVK_LBUTTON))
	{
		next = GS_LEVEL2;
	}

	// Your own update logic goes here
	if (pos_x < click_pos_x)
	{
		x += 2;
		pos_x += 2;
	}

	else if (pos_x > click_pos_x)
	{
		x -= 2;
		pos_x -= 2;
	}

	if (pos_y < click_pos_y)
	{
		y -= 2;
		pos_y += 2;
	}

	else if (pos_y > click_pos_y)
	{
		y += 2;
		pos_y -= 2;
	}

	// Your own rendering logic goes here
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1.0f);
	AEGfxTextureSet(pTex, 0, 0);

	//scale matrix
	AEMtx33 scale{ 0 };
	AEMtx33Scale(&scale, 150.0f, 150.0f);

	//translation matrix
	AEMtx33 translate{ 0 };
	AEMtx33Trans(&translate, x, y);

	//rotation matrix
	AEMtx33 rotation{ 0 };
	AEMtx33Rot(&rotation, 100.0f);

	AEMtx33 transform{ 0 };
	AEMtx33Concat(&transform, &rotation, &scale);
	AEMtx33Concat(&transform, &translate, &transform);

	//setting the transform
	AEGfxSetTransform(transform.m);
}

void Bevantest_Draw()
{
	//Draw mesh
	AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);

	//Gameobjects Render
	for (GameObject* gameObj : go_list)
	{
		if (gameObj->active)
		{
			if (gameObj->type == GameObject::GO_PLANET)
				gameObj->Render();
		}
	}
}

void Bevantest_Free()
{
	AEGfxMeshFree(pMesh);
}

void Bevantest_Unload()
{
	AEGfxTextureUnload(pTex);
}