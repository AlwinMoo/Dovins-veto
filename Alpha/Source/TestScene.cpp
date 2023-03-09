#include "TestScene.h"
#include <iostream>
#include "Rendering.hpp"
#include "GameStateList.h"
#include "GameStateManager.h"
#include "Pathfinding/pathfinder.h"

#include <cstdio>

namespace
{
	GameObject* FetchGO(GameObject::GAMEOBJECT_TYPE value)
	{
		for (auto it : go_list)
		{
			GameObject* go = (GameObject*)it;
			if (!go->active && go->type == value)
			{
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

	static const int WALL_COST			= 50;
	static const int TOWER_COST			= 500;

	//player
	GameObject* player;

	bool player_moving{ false };
	AEVec2 player_goal{ 0, 0 };

	float debounce{};

	// TEXT TEST
	UI::UI_TextArea endTurnHoverText;
	UI::UI_TextArea buildTowerHoverText;
	UI::UI_TextArea buildNexusHoverText;
	UI::UI_TextArea buildWallHoverText;
	UI::UI_TextArea buildNexusPlacedHoverText;
	UI::UI_TextArea eraseHoverText;
}

#pragma region UI_CALLBACK_DECLARATIONS
void EndTurnButton();
void PlaceNexusButton();
void PlaceTowerButton();
void PlaceWallButton();
void EraseButton();
#pragma endregion

void TestScene_Load()
{
	turretTex = AEGfxTextureLoad("Assets/Tower.png");
	grassTex = AEGfxTextureLoad("Assets/GrassTile.png");
	nexusTex = AEGfxTextureLoad("Assets/Nexus.png");
	wallTex = AEGfxTextureLoad("Assets/Wall.png");
	bulletTex = AEGfxTextureLoad("Assets/YellowTexture.png");
	grassBorderlessTex = AEGfxTextureLoad("Assets/GrassTileBorderless.png");
	playerTex = AEGfxTextureLoad("Assets/PlayerTexture.png");
	enemyTex = AEGfxTextureLoad("Assets/EnemyTexture.png");
	eraseTex = AEGfxTextureLoad("Assets/Eraser.png");
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

	test_map = new game_map(20, 20, AEGetWindowWidth(), AEGetWindowHeight(), 8);

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
	hoverStructure->rotation = 0;
	hoverStructure->scale.x = test_map->GetTileSize() * 3;
	hoverStructure->scale.y = hoverStructure->scale.x;
	hoverStructure->tex = nexusTex;
	hoverStructure->gridScale = { 3, 3 };
	validPlacement = false;

	// UI MANAGER ELEMENTS INITIALIZER
	{
		endTurnHoverText			= { .3f, 1.f, "Ends The Build Phase. BE WARNED: YOU CANNOT BUILD DURING DEFENDING PHASE"};
		buildTowerHoverText			= { .3f, 1.f, "Builds a tower. Automatically attacks enemies from range. COST: 500"};
		buildWallHoverText			= { .3f, 1.f, "Builds a wall. Most enemies walk around them. COST: 50" };
		buildNexusHoverText			= { .3f, 1.f, "Builds the nexus. Protect it with your life." };
		buildNexusPlacedHoverText	= { .3f, 1.f, "Nexus already built. You only get one." };
		eraseHoverText				= { .3f, 1.f, "Erase your building. No shame in mistakes. 100% cost refunded." };
		f32 screenWidthX = AEGfxGetWinMaxX() - AEGfxGetWinMinX();
		f32 screenWidthY = AEGfxGetWinMaxY() - AEGfxGetWinMinY();

		AEVec2 const endButtonPos{ screenWidthX * .115f, screenWidthY * .2f };
		AEVec2 const endButtonSize{ screenWidthX * .2f, screenWidthY * .15f };
		uiManager->CreateButton(endButtonPos, endButtonSize, UI::END_PHASE_BUTTON, nullptr, EndTurnButton, &endTurnHoverText);

		AEVec2 const buildButtonStartPos{ screenWidthX * .115f, screenWidthY * .9f };
		AEVec2 const buildButtonSize{ screenWidthY * .12f, screenWidthY * .12f };
		AEVec2 buildButtonPos{ buildButtonStartPos };
		nexusButton = uiManager->CreateButton(buildButtonPos, buildButtonSize, UI::BUILD_NEXUS_BUTTON, nullptr, PlaceNexusButton, &buildNexusHoverText);
		buildButtonPos.y -= buildButtonSize.y * 1.5f;	// Offset button y pos downwards!
		uiManager->CreateButton(buildButtonPos, buildButtonSize, UI::BUILD_WALL_BUTTON, nullptr, PlaceWallButton, &buildWallHoverText);
		buildButtonPos.y -= buildButtonSize.y * 1.5f;	// Offset button y pos downwards!
		uiManager->CreateButton(buildButtonPos, buildButtonSize, UI::BUILD_TOWER_BUTTON, nullptr, PlaceTowerButton, &buildTowerHoverText);
		buildButtonPos.y -= buildButtonSize.y * 1.5f;	// Offset button y pos downwards!
		uiManager->CreateButton(buildButtonPos, buildButtonSize, UI::ERASE_BUTTON, nullptr, EraseButton, &eraseHoverText);
	}

	player = FetchGO(GameObject::GO_PLAYER);
	player->scale.x = test_map->GetTileSize();
	player->scale.y = player->scale.x;
	player->active = false;
	player->tex = playerTex;

	buildPhase = true;
	nexusPlaced = false;
	buildResource = 3000;
}

void TestScene_Update()
{
	debounce += AEFrameRateControllerGetFrameTime() * AEFrameRateControllerGetFrameRate(); // 1 sec

	// Player Input
	s32 mouseX, mouseY;
	AEInputGetCursorPosition(&mouseX, &mouseY);
	AEVec2 absMousePos{};
	AEVec2Set(&absMousePos, static_cast<f32>(mouseX), static_cast<f32>(mouseY));

	AEVec2 mouse_pos{};
	AEVec2Set(&mouse_pos, static_cast<f32>(mouseX), static_cast<f32>(mouseY));
	//float mouseYGrid = static_cast<int>(mouseY / test_map->get_tile_size()) * test_map->get_tile_size() + (test_map->get_tile_size() * 0.5);
	{
		AEVec2 invert_mouse = mouse_pos;
		invert_mouse.y = uiManager->m_winDim.y - mouse_pos.y;
		uiManager->Update(invert_mouse, AEInputCheckTriggered(AEVK_LBUTTON));
	}
	mouse_pos = test_map->SnapCoordinates(mouse_pos);

	if (buildPhase)
	{
		// Place Structure

		if (AEInputCheckTriggered(AEVK_LBUTTON))
		{
			if (hoverStructure->tex == eraseTex)
			{
				if (!validPlacement && test_map->IsInGrid(mouse_pos))
				{
					AEVec2 topRightPos;
					AEVec2 deleteGridScale;
					for (GameObject* gameObj : go_list)
					{
						if (gameObj->active)
						{
							if (gameObj->type == GameObject::GO_TILE || gameObj == hoverStructure)
								continue;
							if (AEVec2Distance(&gameObj->position, &hoverStructure->position) <= (gameObj->scale.x * 0.5f + hoverStructure->scale.x * 0.5f))
							{
								gameObj->active = false;
								if (gameObj->type == GameObject::GO_WALL)
									buildResource += WALL_COST;
								else if (gameObj->type == GameObject::GO_TURRET)
									buildResource += TOWER_COST;

								topRightPos = gameObj->position;
								topRightPos.x -= (gameObj->gridScale.x - 1) * 0.5 * test_map->GetTileSize();
								topRightPos.y -= (gameObj->gridScale.y - 1) * 0.5 * test_map->GetTileSize();
								deleteGridScale = gameObj->gridScale;

								if (gameObj->type == GameObject::GO_NEXUS)
								{
									nexusPlaced = false;
									nexusButton->texID = UI::TEX_NEXUS;
									nexusButton->hoverText = &buildNexusHoverText;
								}

								break;
							}
						}
					}

					test_map->RemoveItem(test_map->WorldToIndex(topRightPos), deleteGridScale.x, deleteGridScale.y);
				}
			}
			else if (validPlacement)
			{
				GameObject* test = nullptr;

				if (hoverStructure->tex == wallTex)
				{
					if (buildResource < WALL_COST)
						return;
					test = FetchGO(GameObject::GO_WALL);
					buildResource -= WALL_COST;
				}
				else if (hoverStructure->tex == turretTex)
				{
					if (buildResource < TOWER_COST)
						return;
					test = FetchGO(GameObject::GO_TURRET);
					buildResource -= TOWER_COST;
				}
				else if (hoverStructure->tex == nexusTex)
					test = FetchGO(GameObject::GO_NEXUS);

				test->position = hoverStructure->position;
				test->rotation = hoverStructure->rotation;
				test->scale = hoverStructure->scale;
				test->tex = hoverStructure->tex;
				test->gridScale = hoverStructure->gridScale;

				test_map->AddItem(game_map::TILE_TYPE::TILE_PLANET, test_map->WorldToIndex(mouse_pos), hoverStructure->gridScale.x, hoverStructure->gridScale.y);

				if (test->type == GameObject::GO_NEXUS)
				{
					nexusPlaced = true;
					nexusButton->texID = UI::TEX_NEXUS_PLACED;
					nexusButton->hoverText = &buildNexusPlacedHoverText;
					PlaceWallButton();
				}
			}
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

		//if (AEInputCheckTriggered(AEVK_SPACE))
		//{
		//	GameObject* temp = FetchGO(GameObject::GO_BULLET);
		//	temp->position = turret->position;
		//	temp->active = true;
		//	temp->scale.x = 10;
		//	temp->scale.y = 10;
		//	temp->tex = bulletTex;
		//
		//	AEVec2Set(&temp->direction, AESinDeg(turret->rotation), AECosDeg(turret->rotation));
		//	AEVec2Normalize(&temp->direction, &temp->direction);
		//}

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

				for (GameObject* go : go_list)
				{
					if (go->active && go->type == GameObject::GAMEOBJECT_TYPE::GO_ENEMY)
					{
						if (AEVec2Distance(&gameObj->position, &go->position) <= go->scale.x * 0.5)
						{
							go->active = false;
						}
					}
				}
				break;
			}
			case (GameObject::GAMEOBJECT_TYPE::GO_TURRET):
			{
				AEVec2 result{ 0,0 };
				AEVec2Sub(&result, &gameObj->position, &absMousePos);
				gameObj->rotation = AERadToDeg(atan2f(result.x, result.y));

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
	//f32 winX{ AEGfxGetWinMaxX() - AEGfxGetWinMinX() }, winY{ AEGfxGetWinMaxY() - AEGfxGetWinMinY() };
	//f32 cursorXN{ cursorX / winX * 2 - 1.f }, cursorYN{ cursorY / winY * -2 + 1.f }; // NORMALIZED COORDINATES
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

	{
#ifdef CURSOR_TEST
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
#endif
		// Render UI
		uiManager->Draw(cursorX, cursorY);

		char buff[30]{};
		sprintf_s(buff, "Resources Left: %d", buildResource);
		AEGfxPrint(1, buff, .7f, .9f, 1.5f, 1.f, 1.f, 0.f);
		//std::cout << "Resource Left:" << buildResource << std::endl;
	}
}

void TestScene_Free()
{
	for (auto i : go_list)
	{
		delete i;
	}
}

void TestScene_Unload()
{
	delete uiManager;
	delete test_map;
	AEGfxTextureUnload(grassBorderlessTex); 
	AEGfxTextureUnload(turretTex);
 	AEGfxTextureUnload(grassTex);
 	AEGfxTextureUnload(bulletTex);
 	AEGfxTextureUnload(playerTex);
 	AEGfxTextureUnload(enemyTex);
	AEGfxTextureUnload(wallTex);
	AEGfxTextureUnload(nexusTex);
	AEGfxTextureUnload(eraseTex);
 	//AEGfxTextureUnload(texTest);
 }

#pragma region UI_CALLBACK_DEFINITIONS
void EndTurnButton() {
	if (nexusPlaced)
	{
		buildPhase = false;
		hoverStructure->active = false;
		for (GameObject* tile : go_list)
		{
			if (tile->type == GameObject::GO_TILE)
				tile->tex = grassBorderlessTex;
		}
	}
	else
	{
		//FEEDBACK IF PLAYER TRIES TO END TURN WITHOUT PLACING NEXUS
	}
}

void PlaceNexusButton()
{
	if (!nexusPlaced)
	{
		hoverStructure->gridScale = { 3, 3 };
		hoverStructure->scale = { test_map->GetTileSize() * 3, test_map->GetTileSize() * 3 };
		hoverStructure->position.x += test_map->GetTileSize() * 0.5f * (hoverStructure->gridScale.x - 1);
		hoverStructure->position.y += test_map->GetTileSize() * 0.5f * (hoverStructure->gridScale.y - 1);
		hoverStructure->tex = nexusTex;
	}
}

void PlaceTowerButton()
{
	hoverStructure->gridScale = { 2, 2 };
	hoverStructure->scale = { test_map->GetTileSize() * 2, test_map->GetTileSize() * 2 };
	hoverStructure->position.x += test_map->GetTileSize() * 0.5f * (hoverStructure->gridScale.x - 1);
	hoverStructure->position.y += test_map->GetTileSize() * 0.5f * (hoverStructure->gridScale.y - 1);
	hoverStructure->tex = turretTex;
}

void PlaceWallButton()
{
	hoverStructure->gridScale = { 1, 1 };
	hoverStructure->scale = { test_map->GetTileSize() * 1, test_map->GetTileSize() * 1 };
	hoverStructure->position.x += test_map->GetTileSize() * 0.5f * (hoverStructure->gridScale.x - 1);
	hoverStructure->position.y += test_map->GetTileSize() * 0.5f * (hoverStructure->gridScale.y - 1);
	hoverStructure->tex = wallTex;
}

void EraseButton()
{
	hoverStructure->gridScale = { 1, 1 };
	hoverStructure->scale = { test_map->GetTileSize() * 1 - EPSILON, test_map->GetTileSize() * 1 - EPSILON };
	hoverStructure->position.x += test_map->GetTileSize() * 0.5f * (hoverStructure->gridScale.x - 1);
	hoverStructure->position.y += test_map->GetTileSize() * 0.5f * (hoverStructure->gridScale.y - 1);
	hoverStructure->tex = eraseTex;
}
#pragma endregion