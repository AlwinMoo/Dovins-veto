#include "TestScene.h"
#include <iostream>
#include "Rendering.hpp"
#include "GameStateList.h"
#include "GameStateManager.h"
#include "Pathfinding/pathfinder.h"

namespace
{
	GameObject* FetchGO(GameObject::GAMEOBJECT_TYPE value)
	{
		for (auto it : go_list)
		{
			GameObject* go = (GameObject*)it;
			if (!go->active && go->type == value)
			{
				go->tex = planetTex;
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

	//player
	GameObject* player;

	bool player_moving{ false };
	AEVec2 player_goal{ 0, 0 };

	//turret
	bool placeTurret{ false };

	GameObject* turret;

	float debounce{};
}

#pragma region UI_CALLBACK_DECLARATIONS
void EndTurnButton();
#pragma endregion

void TestScene_Load()
{
	planetTex = AEGfxTextureLoad("Assets/PlanetTexture.png");
	grassTex = AEGfxTextureLoad("Assets/GrassTile.png");
	bulletTex = AEGfxTextureLoad("Assets/YellowTexture.png");
	grassBorderlessTex = AEGfxTextureLoad("Assets/GrassTileBorderless.png");
	playerTex = AEGfxTextureLoad("Assets/PlayerTexture.png");
	enemyTex = AEGfxTextureLoad("Assets/EnemyTexture.png");
}

void TestScene_Initialize()
{
	// UI MANAGER
	{
		f32 screenWidthX = AEGfxGetWinMaxX() - AEGfxGetWinMinX();
		f32 screenHeightY = AEGfxGetWinMaxY() - AEGfxGetWinMinY();
		uiManager = new UI::UI_Manager();
		uiManager->SetWinDim(screenWidthX, screenHeightY);
	}
	srand(time(NULL));

	test_map = new game_map(10, 10, AEGetWindowWidth(), AEGetWindowHeight(), 4);

	for (int i = 0; i < test_map->width * test_map->height; i++)
	{
		GameObject* temp = FetchGO(GameObject::GO_TILE);
		temp->scale.x = test_map->GetTileSize();
		temp->scale.y = test_map->GetTileSize();
		temp->position = test_map->GetWorldPos(i);
		temp->tex = grassTex;
	}

	for (int i = 0; i < 50; i++) // make 50 bullets to instantiate later
	{
		GameObject* temp = FetchGO(GameObject::GO_BULLET);
		temp->scale.x = 10;
		temp->scale.y = 10;
		temp->tex = bulletTex;
		temp->active = false;
	}

	for (int i = 0; i < 10; i++)
	{
		GameObject* temp = FetchGO(GameObject::GO_ENEMY);
		temp->scale.x = test_map->GetTileSize();
		temp->scale.y = test_map->GetTileSize();
		temp->tex = enemyTex;
		temp->active = false;
	}

	hoverStructure = FetchGO(GameObject::GO_HOVER_STRUCTURE);
	hoverStructure->alpha = 0.5f;
	hoverStructure->rotation = rand() % 360;
	hoverStructure->scale.x = test_map->GetTileSize();
	hoverStructure->scale.y = hoverStructure->scale.x;
	validPlacement = false;

	// UI MANAGER INITIALIZER
	{
		f32 screenWidthX = AEGfxGetWinMaxX() - AEGfxGetWinMinX();
		f32 screenWidthY = AEGfxGetWinMaxY() - AEGfxGetWinMinY();
		const AEVec2 buttonPos{ screenWidthX / 4, screenWidthY / 4 };
		const AEVec2 buttonSize{ 50.f, 50.f };
		uiManager->CreateButton(buttonPos, buttonSize, UI::END_PHASE_BUTTON, EndTurnButton);
	}

	player = FetchGO(GameObject::GO_PLAYER);
	player->scale.x = test_map->GetTileSize();
	player->scale.y = player->scale.x;
	player->active = false;
	player->tex = playerTex;

	buildPhase = true;
}

void TestScene_Update()
{
	debounce += AEFrameRateControllerGetFrameTime() * AEFrameRateControllerGetFrameRate(); // 1 sec

	// Player Input
	s32 mouseX, mouseY;
	AEInputGetCursorPosition(&mouseX, &mouseY);
	AEVec2 absMousePos{};
	AEVec2Set(&absMousePos, mouseX, mouseY);

	//if (turret && turret->active)
	//	std::cout << absMousePos.x << ',' << absMousePos.y << '[' << turret->position.x << ',' << turret->position.y << ']' << std::endl;

	AEVec2 mouse_pos{};
	AEVec2Set(&mouse_pos, mouseX, mouseY);
	//float mouseYGrid = static_cast<int>(mouseY / test_map->get_tile_size()) * test_map->get_tile_size() + (test_map->get_tile_size() * 0.5);
	{
		AEVec2 invert_mouse = mouse_pos;
		invert_mouse.y = uiManager->m_winDim.y - mouse_pos.y;
		uiManager->Update(invert_mouse, AEInputCheckTriggered(AEVK_LBUTTON));
	}
	mouse_pos = test_map->SnapCoordinates(mouse_pos);
	//float mouseYGrid = static_cast<int>(mouseY / test_map->GetTileSize()) * test_map->GetTileSize() + (test_map->GetTileSize() * 0.5);

	if (buildPhase)
	{
		// Place Structure

		if (placeTurret) //if we have been given the turret flag
		{
			if (AEInputCheckTriggered(AEVK_LBUTTON) && validPlacement) // and a left click is detected
			{
				placeTurret = false;

				turret = FetchGO(GameObject::GO_TURRET);
				turret->position = hoverStructure->position;
				turret->rotation = hoverStructure->rotation;
				turret->scale = hoverStructure->scale;

				test_map->AddItem(game_map::TILE_TYPE::TILE_PLANET, test_map->WorldToIndex(mouse_pos), hoverStructure->gridScale.x, hoverStructure->gridScale.y);

			}
		}
		else if (AEInputCheckTriggered(AEVK_LBUTTON) && validPlacement)
		{
			GameObject* test = FetchGO(GameObject::GO_PLANET);
			test->position = hoverStructure->position;
			test->rotation = hoverStructure->rotation;
			test->scale = hoverStructure->scale;
			hoverStructure->rotation = rand() % 360;

			test_map->AddItem(game_map::TILE_TYPE::TILE_PLANET, test_map->WorldToIndex(mouse_pos), hoverStructure->gridScale.x, hoverStructure->gridScale.y);
		}

		// Change selected structure
		if (AEInputCheckTriggered(AEVK_1))
		{
			hoverStructure->gridScale = { 1, 1 };
			hoverStructure->scale = { test_map->GetTileSize(), test_map->GetTileSize() };
			hoverStructure->position = mouse_pos;
			hoverStructure->position.x += test_map->GetTileSize() * 0.5f * (hoverStructure->gridScale.x - 1);
			hoverStructure->position.y += test_map->GetTileSize() * 0.5f * (hoverStructure->gridScale.y - 1);
		}
		else if (AEInputCheckTriggered(AEVK_2))
		{
			hoverStructure->gridScale = { 2, 2 };
			hoverStructure->scale = { test_map->GetTileSize() * 2, test_map->GetTileSize() * 2 };
			hoverStructure->position = mouse_pos;
			hoverStructure->position.x += test_map->GetTileSize() * 0.5f * (hoverStructure->gridScale.x - 1);
			hoverStructure->position.y += test_map->GetTileSize() * 0.5f * (hoverStructure->gridScale.y - 1);
		}
		else if (AEInputCheckTriggered(AEVK_3))
		{
			hoverStructure->gridScale = { 3, 3 };
			hoverStructure->scale = { test_map->GetTileSize() * 3, test_map->GetTileSize() * 3 };
			hoverStructure->position = mouse_pos;
			hoverStructure->position.x += test_map->GetTileSize() * 0.5f * (hoverStructure->gridScale.x - 1);
			hoverStructure->position.y += test_map->GetTileSize() * 0.5f * (hoverStructure->gridScale.y - 1);
		}
		else if (AEInputCheckTriggered(AEVK_4))
		{
			hoverStructure->gridScale = { 1, 1 };
			hoverStructure->scale = { test_map->GetTileSize(), test_map->GetTileSize() };
			hoverStructure->position = mouse_pos;
			hoverStructure->position.x += test_map->GetTileSize() * 0.5f * (hoverStructure->gridScale.x - 1);
			hoverStructure->position.y += test_map->GetTileSize() * 0.5f * (hoverStructure->gridScale.y - 1);

			placeTurret = true;
		}

		// Update position of hover structure
		if (hoverStructure->position.x != mouse_pos.x
			|| hoverStructure->position.y != mouse_pos.y)
		{
			hoverStructure->position = mouse_pos;
			hoverStructure->position.x += test_map->GetTileSize() * 0.5f * (hoverStructure->gridScale.x - 1);
			hoverStructure->position.y += test_map->GetTileSize() * 0.5f * (hoverStructure->gridScale.y - 1);
		}

		// Turn on/off hover if in/out the grid
		if (test_map->IsInGrid(mouse_pos, hoverStructure->gridScale.x, hoverStructure->gridScale.y))
		{
			hoverStructure->active = true;
			validPlacement = true;
		}
		else
		{
			hoverStructure->active = false;
			validPlacement = false;
		}

		// GameObject Collision (GRID BASED)
		if (test_map->IsOccupied(test_map->WorldToIndex(mouse_pos), hoverStructure->gridScale.x, hoverStructure->gridScale.y))
		{
			hoverStructure->color.Set(1.0f, 0.2f, 0.2f);
			validPlacement = false;
		}
		else if (hoverStructure->active)
		{
			hoverStructure->color.Set(1.0f, 1.0f, 1.0f);
			validPlacement = true;
		}

		// CHANGE TO DEFEND MODE
		if (AEInputCheckTriggered(AEVK_0))
		{
			EndTurnButton();	// NOTE: Clicking button calls this
		}
	}
	else
	{
		if (AEInputCheckTriggered(AEVK_RBUTTON) && !test_map->IsOccupied(test_map->WorldToIndex(mouse_pos)))
		{
			if (test_map->IsInGrid(absMousePos))
			{
				if (!player->active)
				{
					player->position = test_map->SnapCoordinates(absMousePos);
					player->active = true;

					player_moving = false;
				}
				else
				{
					player_moving = true;
					//player_goal = test_map->snap_coordinates(mousePos);
					PathManager pathingObj(test_map);
					player->Path = pathingObj.GetPath(AEVec2{ (float)test_map->GetX(test_map->WorldToIndex(player->position)), (float)test_map->GetY(test_map->WorldToIndex(player->position)) }, AEVec2{ (float)test_map->GetX(test_map->WorldToIndex(absMousePos)), (float)test_map->GetY(test_map->WorldToIndex(absMousePos)) });

					if (!player->Path.empty())
					{
						for (auto& pos : player->Path)
						{
							pos = test_map->GetWorldPos(test_map->GetIndex(pos.x + test_map->tile_offset, pos.y));
						}

						player->Path.erase(player->Path.end() - 1);
						player->Path.push_back(absMousePos);
						player_goal = player->Path.front();
					}
					else
					{
						AEVec2 leng{};
						leng.x = player->position.x - absMousePos.x;
						leng.y = player->position.y - absMousePos.y;

						if (AEVec2Length(&leng) <= 5)
						{
							player->Path.push_back(absMousePos);
							player_goal = player->Path.front();
						}
					}
				}
			}
		}

		if (player_moving && !player->Path.empty())
		{
			AEVec2 leng{};
			leng.x = player->position.x - player_goal.x;
			leng.y = player->position.y - player_goal.y;

			if (AEVec2Length(&leng) <= 5)
			{
				player->Path.erase(player->Path.begin());

				if (player->Path.empty())
				{
					// reset once reached goal
					player_moving = false;
				}
				else
				{
					player_goal = player->Path.front();
				}
			}
		}

		if (AEInputCheckTriggered(AEVK_SPACE))
		{
			GameObject* temp = FetchGO(GameObject::GO_BULLET);
			temp->position = turret->position;
			temp->active = true;
			temp->scale.x = 10;
			temp->scale.y = 10;
			temp->tex = bulletTex;

			AEVec2Set(&temp->direction, AESinDeg(turret->rotation), AECosDeg(turret->rotation));
			AEVec2Normalize(&temp->direction, &temp->direction);
		}

		if (AEInputCheckTriggered(AEVK_P))
		{
			GameObject* temp = FetchGO(GameObject::GO_ENEMY);
			temp->position = test_map->GetWorldPos(0);
			temp->scale.x = test_map->GetTileSize();
			temp->scale.y = test_map->GetTileSize();
			temp->tex = enemyTex;
			temp->active = true;

		}
	}
	

	// Quit Game
	if (AEInputCheckTriggered(AEVK_Q))
	{
		next = GS_QUIT;
	}

	// GameObject Update
	for (GameObject* gameObj : go_list)
	{
		if (gameObj->active)
		{
			gameObj->Update();

			switch (gameObj->type)
			{
			case (GameObject::GAMEOBJECT_TYPE::GO_ENEMY):
			{
				if (gameObj->position.x > AEGetWindowWidth() || gameObj->position.x < 0 || gameObj->position.y > AEGetWindowHeight() || gameObj->position.y < 0)
					gameObj->active = false;

				if (debounce >= 100.0f)
				{
					debounce = 0.0f;
					std::cout << "Hit" << std::endl;
					PathManager pathmaker(test_map);
					gameObj->Path = pathmaker.GetPath(AEVec2{ (float)test_map->GetX(test_map->WorldToIndex(gameObj->position)), (float)test_map->GetY(test_map->WorldToIndex(gameObj->position)) }, AEVec2{ (float)test_map->GetX(test_map->WorldToIndex(player->position)), (float)test_map->GetY(test_map->WorldToIndex(player->position)) });
					if (!gameObj->Path.empty())
					{
						gameObj->Path.erase(gameObj->Path.end() - 1);
						for (auto& pos : gameObj->Path)
						{
							pos = test_map->GetWorldPos(test_map->GetIndex(pos.x + test_map->tile_offset, pos.y));
						}
					}
				}

				AEVec2 result{ 0,0 };
				AEVec2Sub(&result, &player->position, &gameObj->position);
				gameObj->rotation = AERadToDeg(atan2f(result.x, result.y)); // rotate to face player

				if (!gameObj->Path.empty())
				{
					AEVec2 out{};
					AEVec2 norm{};
					AEVec2Set(&norm, (gameObj->Path[0].x - gameObj->position.x), (gameObj->Path[0].y - gameObj->position.y));
					AEVec2Normalize(&out, &norm);
					/*AEVec2Set(&gameObj->direction, AESin(gameObj->rotation), AECos(gameObj->rotation));
					AEVec2Normalize(&gameObj->direction, &gameObj->direction);*/

					gameObj->position.x += out.x * AEFrameRateControllerGetFrameTime() * 100;
					gameObj->position.y += out.y * AEFrameRateControllerGetFrameTime() * 100;


					AEVec2 leng{};
					leng.x = gameObj->position.x - gameObj->Path[0].x;
					leng.y = gameObj->position.y - gameObj->Path[0].y;

					if (AEVec2Length(&leng) <= 5)
					{
						gameObj->Path.erase(gameObj->Path.begin());
					}
				}

				break;
			}
			case (GameObject::GAMEOBJECT_TYPE::GO_PLAYER):
			{
				if (player_moving && !player->Path.empty())
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
			case (GameObject::GAMEOBJECT_TYPE::GO_TURRET):
			{
				AEVec2 result{ 0,0 };
				AEVec2Sub(&result, &absMousePos, &turret->position);
				turret->rotation = AERadToDeg(atan2f(result.x, result.y));

				break;
			}
			case (GameObject::GAMEOBJECT_TYPE::GO_BULLET):
			{
				gameObj->position.x += gameObj->direction.x * AEFrameRateControllerGetFrameTime() * 600;
				gameObj->position.y += gameObj->direction.y * AEFrameRateControllerGetFrameTime() * 600;

				if (gameObj->position.x > AEGetWindowWidth() || gameObj->position.x < 0 || gameObj->position.y > AEGetWindowHeight() || gameObj->position.y < 0)
					gameObj->active = false;

				for (GameObject* go : go_list)
				{
					if (go->active && go->type == GameObject::GAMEOBJECT_TYPE::GO_ENEMY)
					{
						if (AEVec2Distance(&gameObj->position, &go->position) <= go->scale.x * 0.5)
						{
							go->active = false;
							gameObj->active = false;
						}
					}
				}

				break;
			}
			}
		}
	}

	// GameObject Collision (NON-GRID BASED, SHOULD CHANGE)
	//bool hoverCollided = false;
	//for (GameObject* gameObj : go_list)
	//{
	//	if (!gameObj->active)
	//		continue;
	//
	//	if (gameObj != hoverStructure)
	//	{
	//		if (gameObj->type == GameObject::GO_TILE)
	//			continue;
	//		if (hoverStructure->position.x == gameObj->position.x &&
	//			hoverStructure->position.y == gameObj->position.y)
	//		{
	//			hoverStructure->color.Set(1.0f, 0.2f, 0.2f);
	//			hoverCollided = true;
	//			validPlacement = false;
	//			break;
	//		}
	//	}
	//}
	//
	//if (!hoverCollided && hoverStructure->active)
	//{
	//	hoverStructure->color.Set(1.0f, 1.0f, 1.0f);
	//	validPlacement = true;
	//}
}

void TestScene_Draw()
{
	s32 cursorX, cursorY;
	//s8 text[] = "TEST";
	AEInputGetCursorPosition(&cursorX, &cursorY);
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

	AEGfxSetBackgroundColor(0.3f, 0.3f, 0.3f);
	for (GameObject* gameObj : go_list)
	{
		//Gameobjects Render
		if (gameObj->active)
			gameObj->Render();
	}

	// Render above
	if (hoverStructure->active)
		hoverStructure->Render();

	f32 screenWidthX = AEGfxGetWinMaxX() - AEGfxGetWinMinX();
	f32 screenWidthY = AEGfxGetWinMaxY() - AEGfxGetWinMinY();

	{
		// THE WHERE'S MY CURSOR TEST?
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetTintColor(1.f, 1.f, 1.f, 1.0f);
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxSetTransparency(1.f);
		AEGfxTextureSet(texTest, 0, 0);

		AEMtx33 scale = { 0 };
		AEMtx33Scale(&scale, 50.f, 50.f);

		AEMtx33 rotate = { 0 };
		AEMtx33Rot(&rotate, 0.f);

		AEMtx33 translate = { 0 };
		//std::cout << cursorX << ", " << cursorY << std::endl;
		AEMtx33Trans(&translate, cursorX - screenWidthX / 2, -cursorY + screenWidthY / 2 - 50);

		AEMtx33 transform = { 0 };
		AEMtx33Concat(&transform, &rotate, &scale);
		AEMtx33Concat(&transform, &translate, &transform);

		AEGfxSetTransform(transform.m);
		AEGfxMeshDraw(uiManager->m_mesh[0], AE_GFX_MDM_TRIANGLES);
		// Render UI
		uiManager->Draw();
	}
}

void TestScene_Free()
{
	delete uiManager;
}

void TestScene_Unload()
{

}

#pragma region UI_CALLBACK_DEFINITIONS
void EndTurnButton() {
	buildPhase = false;
	hoverStructure->active = false;
	for (GameObject* tile : go_list)
	{
		if (tile->type == GameObject::GO_TILE)
			tile->tex = grassBorderlessTex;
	}
}
#pragma endregion