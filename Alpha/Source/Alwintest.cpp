#include "Alwintest.h"
#include "AEEngine.h"
#include <iostream>
#include <iomanip>
#include "UI_Manager.h"
#include <vector>
#include "Map/Map.h"
#include "GameObject.h"
#include "GameStateManager.h"
#include "GameStateList.h"
#include "Pathfinding/pathfinder.h"

#include <iostream>

namespace
{
	f32 winSizeX, winSizeY;
	s8 m_fontId;

	AEGfxTexture* pTex;
	AEGfxVertexList* pMesh;

	AEGfxTexture* grass_Tex;
	AEGfxVertexList* grass_Mesh;

	s32 click_pos_x;
	s32 click_pos_y;
	s32 pos_x, pos_y;
	s32 x, y;

	std::vector<GameObject*> go_list;
	game_map* test_map;
	int object_count;

	GameObject* cursor;
	GameObject* player;

	bool player_moving{ false };
	AEVec2 player_goal{0, 0};

	PathManager* pathingObj{};
	std::vector<AEVec2> player_path{};

	GameObject* FetchGO(GameObject::GAMEOBJECT_TYPE value)
	{
		for (auto it : go_list)
		{
			GameObject* go = (GameObject*)it;
			if (!go->active && go->type == value)
			{
				switch (go->type)
				{
					case GameObject::GAMEOBJECT_TYPE::GO_GRASS:
						go->tex = grass_Tex;
						break;
					default:
						go->tex = pTex;
						break;
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

void Alwintest_Load()
{
	grass_Tex = AEGfxTextureLoad("Assets/GrassTile.png");
	pTex = AEGfxTextureLoad("Assets/PlanetTexture.png");
	m_fontId = AEGfxCreateFont("Assets/Roboto-Regular.ttf", 16);
}

void Alwintest_Initialize()
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

	grass_Mesh = 0;
	AEGfxMeshStart();

	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFFFF00FF, 0.0f, 0.0f,
		0.5f, -0.5f, 0xFFFFFF00, 1.0f, 0.0f,
		-0.5f, 0.5f, 0xFF00FFFF, 0.0f, 1.0f);

	AEGfxTriAdd(
		0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 0.0f,
		0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 1.0f);

	grass_Mesh = AEGfxMeshEnd();

	test_map = new game_map(30, 30, AEGetWindowWidth(), AEGetWindowHeight(), 0); // automatically destroyed in deconstructor

	for (int i = 0; i < test_map->map_size; ++i)
	{
		GameObject* grass = FetchGO(GameObject::GO_GRASS);
		grass->position = test_map->GetWorldPos(i);
		grass->scale.x = test_map->GetTileSize();
		grass->scale.y = grass->scale.x;
	}

	player = FetchGO(GameObject::GO_PLAYER);
	player->scale.x = test_map->GetTileSize();
	player->scale.y = player->scale.x;
	player->active = false;

	cursor = FetchGO(GameObject::GO_PLANET);
	cursor->scale.x = test_map->GetTileSize();
	cursor->scale.y = cursor->scale.x;
	cursor->active = false;

	pathingObj = new PathManager(test_map);
}

void Alwintest_Update()
{
	// IMPORTANT SNIPPET:
		/***********************************************************************/
	s32 cursorX, cursorY;
	AEInputGetCursorPosition(&cursorX, &cursorY);
	f32 winX{ AEGfxGetWinMaxX() - AEGfxGetWinMinX() }, winY{ AEGfxGetWinMaxY() - AEGfxGetWinMinY() };
	//std::cout << winX << ", " <<  winY << std::endl;
	f32 cursorXN{ cursorX / winX * 2 - 1.f }, cursorYN{ cursorY / winY * -2 + 1.f }; // NORMALIZED COORDINATES
	//std::cout << std::setprecision(2) << std::setw(5) << cursorXN << ", " << std::setw(5) << cursorYN << std::endl;
	/************************************************************************/
	/*s8 const* testStr = "TEST TEXT";
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxPrint(m_fontId, (s8*)testStr, cursorXN, cursorYN, 2.f, 1.f, 0.f, 0.f);*/

	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);

	// Player Input
	s32 mouseX, mouseY;
	AEInputGetCursorPosition(&mouseX, &mouseY);
	AEVec2 mousePos{mouseX, mouseY};

	if (AEInputCheckTriggered(AEVK_LBUTTON))
	{
		if (test_map->IsInGrid(mousePos))
		{
			if (!player->active)
			{
				player->position = test_map->SnapCoordinates(mousePos);
				player->active = true;

				player_moving = false;
			}
			else
			{
				player_moving = true;
				//player_goal = test_map->snap_coordinates(mousePos);
				player_path = pathingObj->GetPath(AEVec2{ (float)test_map->GetX(test_map->WorldToIndex(player->position)), (float)test_map->GetY(test_map->WorldToIndex(player->position)) }, AEVec2{ (float)test_map->GetX(test_map->WorldToIndex(mousePos)), (float)test_map->GetY(test_map->WorldToIndex(mousePos)) });
				
				if (!player_path.empty())
				{
					for (auto& pos : player_path)
					{
						pos = test_map->GetWorldPos(test_map->GetIndex(pos.x, pos.y));
					}

					player_goal = player_path.front();
					player_path.erase(player_path.end() - 1);
					player_path.push_back(mousePos);
				}
			}
		}
	}
	if (test_map->IsInGrid(mousePos))
	{
		cursor->position = test_map->SnapCoordinates(mousePos);
		cursor->active = true;
	}
	else
		cursor->active = false;

	if (player_moving && !player_path.empty())
	{
		//if ((player->position.x <= player_goal.x - 0.1 || player->position.x >= player_goal.x + 0.1) && (player->position.y <= player_goal.y - 0.1 || player->position.y >= player_goal.y + 0.1))
		AEVec2 leng{};
		leng.x = player->position.x - player_goal.x;
		leng.y = player->position.y - player_goal.y;

		if (AEVec2Length(&leng) <= 5)
		{
			player_path.erase(player_path.begin());

			if (player_path.empty())
			{
				// reset once reached goal
				player_moving = false;
			}
			else
			{
				player_goal = player_path.front();
			}
		}
	}


	// GameObject Update
	for (GameObject* gameObj : go_list)
	{
		if (gameObj->active)
		{
			gameObj->Update();

			switch (gameObj->type)
			{
				case (GameObject::GAMEOBJECT_TYPE::GO_PLAYER):
					if (player_moving && !player_path.empty())
					{
						AEVec2 out{};
						AEVec2 norm{};
						AEVec2Set(&norm, (player_goal.x - player->position.x), (player_goal.y - player->position.y));
						AEVec2Normalize(&out, &norm);

						player->position.x += out.x * AEFrameRateControllerGetFrameTime() * 600;
						player->position.y += out.y * AEFrameRateControllerGetFrameTime() * 600;
					}
					break;
			}
		}
	}

	if (AEInputCheckTriggered(AEVK_RBUTTON))
	{
		AEInputGetCursorPosition(&click_pos_x, &click_pos_y);
		//click_check(click_pos_x, click_pos_y);
		//click_check(pos_x, pos_y);
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

void Alwintest_Draw()
{
	//Draw mesh
	AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);

	//Gameobjects Render
	for (GameObject* gameObj : go_list)
	{
		if (gameObj->active)
		{
			gameObj->Render();				
		}
	}
}

void Alwintest_Free()
{
	AEGfxMeshFree(pMesh);
}

void Alwintest_Unload()
{
	AEGfxTextureUnload(pTex);
}