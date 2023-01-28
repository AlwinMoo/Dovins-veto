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

	PathManager* pathingObj = new PathManager(test_map);
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

	test_map = new game_map(10, 10, AEGetWindowWidth(), AEGetWindowHeight(), true); // automatically destroyed in deconstructor

	for (int i = 0; i < test_map->map_size; ++i)
	{
		GameObject* grass = FetchGO(GameObject::GO_GRASS);
		grass->position = test_map->get_worldpos(i);
		grass->scale.x = test_map->get_tile_size();
		grass->scale.y = grass->scale.x;
	}

	player = FetchGO(GameObject::GO_PLAYER);
	player->scale.x = test_map->get_tile_size();
	player->scale.y = player->scale.x;
	player->active = false;

	cursor = FetchGO(GameObject::GO_PLANET);
	cursor->scale.x = test_map->get_tile_size();
	cursor->scale.y = cursor->scale.x;
	cursor->active = false;
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
		if (test_map->is_in_grid(mousePos))
		{
			if (!player->active)
			{
				player->position = test_map->snap_coordinates(mousePos);
				player->active = true;

				player_moving = false;
			}
			else
			{
				player_moving = true;
				//player_goal = test_map->snap_coordinates(mousePos);
				player_path = pathingObj->GetPath(AEVec2{static_cast<float>(test_map->get_x(std::floor(player->position.x/test_map->get_tile_size()))), static_cast<float>(test_map->get_y(std::floor(player->position.y / test_map->get_tile_size()))) }, AEVec2{ static_cast<float>(test_map->get_x(std::floor(mousePos.x / test_map->get_tile_size()))), static_cast<float>(test_map->get_y(std::floor(mousePos.y / test_map->get_tile_size()))) });
					
				for (auto &pos : player_path)
				{
					pos = test_map->get_worldpos(test_map->get_index(pos.x, pos.y));
				}

				player_goal = player_path.front();
				player_path.erase(player_path.begin());
			}
		}
	}
	if (test_map->is_in_grid(mousePos))
	{
		cursor->position = test_map->snap_coordinates(mousePos);
		cursor->active = true;
	}
	else
		cursor->active = false;

	if (player_moving)
	{
		if (player->position.x == player_goal.x && player->position.y == player_goal.y)
		{
			if (player_path.empty())
			{
				player_moving = false;
			}
			else
			{
				player_goal = player_path.front();
				player_path.erase(player_path.begin());
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
						AEVec2Lerp(&player->position, &player->position, &player_goal, 0.1);
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