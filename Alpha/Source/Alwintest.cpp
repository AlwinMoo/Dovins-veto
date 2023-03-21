#include "Alwintest.h"
#include <iostream>
#include "Rendering.hpp"
#include "GameStateList.h"
#include "GameStateManager.h"
#include "Pathfinding/pathfinder.h"

namespace
{


	std::vector<GameObject*> go_list;
	game_map* test_map;
	int object_count;
	GameObject* hoverStructure;
	bool validPlacement;

	UI::UI_Manager* uiManagers[UI::NUM_UI_TYPE];

	UI::UI_Manager* gameUiManager;
	UI::UI_Manager* skillTreeManager;

	// Input
	AEVec2 mouse_pos{};
	AEVec2 absMousePos{};

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
	AEGfxTexture* dangerTex;
	AEGfxVertexList* meshTest;

	UI::UI_Button* nexusButton;
	UI::UI_Button* playerButton;
	bool nexusPlaced;
	bool playerPlaced;
	bool buildPhase;

	int buildResource;

	static const int WALL_COST = 50;
	static const int TOWER_COST = 500;

	//player
	GameObject* player;
	GameObject* Nexus;

	bool player_moving{ false };
	AEVec2 player_goal{ 0, 0 };

	float turret_shoot_timer{};

	// Enemies
	int	enemiesToSpawn;
	float enemySpawnRate;
	float enemySpawnTimer;
	int	enemiesSpawned;
	int	enemiesRemaining;
	int currentWave;

	// TEXT TEST
	UI::UI_TextAreaTable* textTable;
	
	//Helper Functions
	GameObject* FetchGO(GameObject::GAMEOBJECT_TYPE value);

	void LoadTextures();

	void InitializeUIManager();
	void InitializeUIButtons();
	void InitializeUIElements();
	void InitializeTestMap();
	void InitialFetchGos();
	void InitializeDangerSigns();
	void InitializeVariables();
	void EnableDangerSigns();
	void DisableDangerSigns();

	void UpdateMousePos();
	void UpdateUIManager();
	void PlaceStructure();
	void EraseBuiltStructure();
	void UpdateHoverStructure();

	void SetPlayerGoal();
	void PlayerReachGoalCheck();
	void SpawnEnemies();
	void NextWaveCheck();
	void TempTestUpdateFunctions();

	void NexusEnemyUpdate(GameObject* gameObj);
	void PlayerEnemyUpdate(GameObject* gameObj);
	void UpdateEnemyPath(GameObject* gameObj);
	void UpdateEnemyRotation(GameObject* gameObj);
	void UpdateEnemyPosition(GameObject* gameObj);

	void UpdatePlayerPosition(GameObject* gameObj);
	
	AEVec2 FindClosestEnemy(GameObject* gameObj);
	void UpdateTurretShooting(AEVec2 target, GameObject* gameObj);

#pragma region UI_CALLBACK_DECLARATIONS
	void EndTurnButton(UI::UI_Button*);
	void PlaceNexusButton(UI::UI_Button*);
	void PlacePlayerButton(UI::UI_Button*);
	void PlaceTowerButton(UI::UI_Button*);
	void PlaceWallButton(UI::UI_Button*);
	void EraseButton(UI::UI_Button*);
#pragma endregion
}

void Alwintest_Load()
{
	LoadTextures();
}

void Alwintest_Initialize()
{
	srand(time(NULL));

	InitializeUIManager();
	InitializeUIButtons();
	InitializeUIElements();

	InitializeTestMap();
	InitialFetchGos();

	InitializeVariables();
}

void Alwintest_Update()
{
	turret_shoot_timer += AEFrameRateControllerGetFrameRate();

	// Player Input
	UpdateMousePos();
	UpdateUIManager();

	if (buildPhase)
	{
		if (AEInputCheckCurr(AEVK_LBUTTON) && hoverStructure->active)
		{
			if (hoverStructure->tex == eraseTex)
			{
				if (!validPlacement && test_map->IsInGrid(mouse_pos))
				{
					EraseBuiltStructure();
				}
			}
			else if (validPlacement)
			{
				PlaceStructure();
			}
		}

		UpdateHoverStructure();
	}
	else
	{
		if (AEInputCheckTriggered(AEVK_RBUTTON) && !test_map->IsOccupied(test_map->WorldToPreOffsetIndex(mouse_pos)))
		{
			if (test_map->IsInGrid(absMousePos))
				SetPlayerGoal();
		}

		if (player_moving && !player->Path.empty())
			PlayerReachGoalCheck();

		TempTestUpdateFunctions();

		SpawnEnemies();
		NextWaveCheck();
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

					if (gameObj->Stats.target_type == CharacterStats::TARGET_TYPE::TAR_NEXUS)
						NexusEnemyUpdate(gameObj);
					else if (gameObj->Stats.target_type == CharacterStats::TARGET_TYPE::TAR_PLAYER)
						PlayerEnemyUpdate(gameObj);

					UpdateEnemyPath(gameObj);
					UpdateEnemyRotation(gameObj);
					//UpdateEnemyPosition(gameObj);

					break;
				}
				case (GameObject::GAMEOBJECT_TYPE::GO_PLAYER):
				{
					UpdatePlayerPosition(gameObj);

					for (GameObject* go : go_list)
					{
						if (go->active && go->type == GameObject::GAMEOBJECT_TYPE::GO_ENEMY)
						{
							if (AEVec2Distance(&gameObj->position, &go->position) <= go->scale.x * 0.5)
							{
								std::cout << "Remain: " << enemiesRemaining << std::endl;
								enemiesRemaining--;
								go->active = false;
							}
						}
					}
					break;
				}
				case (GameObject::GAMEOBJECT_TYPE::GO_TURRET):
				{
					gameObj->Stats.SetStat(STAT_ATTACK_SPEED, gameObj->Stats.GetStat(STAT_ATTACK_SPEED) + AEFrameRateControllerGetFrameTime());
					
					AEVec2 result = FindClosestEnemy(gameObj);

					if (result.x != 0.0 && result.y != 0.0) // only if have enemy
						UpdateTurretShooting(result, gameObj);

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
								std::cout << "Remain: " << enemiesRemaining << std::endl;
								enemiesRemaining--;
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

void Alwintest_Draw()
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
		gameUiManager->Draw(cursorX, cursorY);

		char buff[30]{};
		sprintf_s(buff, "Resources Left: %d", buildResource);
		AEGfxPrint(1, buff, .7f, .9f, 1.5f, 1.f, 1.f, 0.f);
		//std::cout << "Resource Left:" << buildResource << std::endl;
	}
}

void Alwintest_Free()
{
	for (auto i : go_list)
	{
		delete i;
	}
	delete textTable;
	delete skillTreeManager;
	delete gameUiManager;
	delete test_map;
}

void Alwintest_Unload()
{
	AEGfxTextureUnload(grassBorderlessTex);
	AEGfxTextureUnload(turretTex);
	AEGfxTextureUnload(grassTex);
	AEGfxTextureUnload(bulletTex);
	AEGfxTextureUnload(playerTex);
	AEGfxTextureUnload(enemyTex);
	AEGfxTextureUnload(wallTex);
	AEGfxTextureUnload(nexusTex);
	AEGfxTextureUnload(eraseTex);
	AEGfxTextureUnload(dangerTex);
}

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

	void EnableDangerSigns()
	{
		for (auto go : go_list)
		{
			if (go->type == GameObject::GO_DANGER_SIGN && !go->active)
			{
				go->active = true;
				test_map->AddItem(game_map::TILE_TYPE::TILE_PLANET, test_map->WorldToPreOffsetIndex(go->position));
			}
		}
	}

	void DisableDangerSigns()
	{
		for (auto go : go_list)
		{
			if (go->type == GameObject::GO_DANGER_SIGN && go->active)
			{
				go->active = false;
				test_map->RemoveItem(test_map->WorldToPreOffsetIndex(go->position));
			}
		}
	}

	void UpdateMousePos()
	{
		s32 mouseX, mouseY;
		AEInputGetCursorPosition(&mouseX, &mouseY);
		AEVec2Set(&absMousePos, static_cast<f32>(mouseX), static_cast<f32>(mouseY));
		AEVec2Set(&mouse_pos, static_cast<f32>(mouseX), static_cast<f32>(mouseY));
		//float mouseYGrid = static_cast<int>(mouseY / test_map->get_tile_size()) * test_map->get_tile_size() + (test_map->get_tile_size() * 0.5);
		mouse_pos = test_map->SnapCoordinates(mouse_pos);
	}

	void UpdateUIManager()
	{
		AEVec2 invert_mouse = mouse_pos;
		invert_mouse.y = ::gameUiManager->m_winDim.y - mouse_pos.y;
		::gameUiManager->Update(invert_mouse, AEInputCheckTriggered(AEVK_LBUTTON));
	}

	void PlaceStructure()
	{
		GameObject* temp = nullptr;
		AEVec2 hoverTopLeftPos = hoverStructure->position;
		hoverTopLeftPos.x -= hoverStructure->scale.x / 2.f;
		if ((int)hoverStructure->gridScale.x % 2)
			hoverTopLeftPos.x += test_map->GetTileSize() / 2.f;
		hoverTopLeftPos.y -= hoverStructure->scale.y / 2.f;
		if ((int)hoverStructure->gridScale.y % 2)
			hoverTopLeftPos.y += test_map->GetTileSize() / 2.f;


		if (hoverStructure->tex == wallTex)
		{
			if (buildResource < WALL_COST)
				return;
			temp = FetchGO(GameObject::GO_WALL);
			buildResource -= WALL_COST;

			test_map->AddItem(game_map::TILE_TYPE::TILE_PLANET, test_map->WorldToPreOffsetIndex(hoverTopLeftPos), hoverStructure->gridScale.x, hoverStructure->gridScale.y);
		}
		else if (hoverStructure->tex == turretTex)
		{
			if (buildResource < TOWER_COST)
				return;
			temp = FetchGO(GameObject::GO_TURRET);
			buildResource -= TOWER_COST;

			test_map->AddItem(game_map::TILE_TYPE::TILE_PLANET, test_map->WorldToPreOffsetIndex(hoverTopLeftPos), hoverStructure->gridScale.x, hoverStructure->gridScale.y);
		}
		else if (hoverStructure->tex == nexusTex)
		{
			temp = FetchGO(GameObject::GO_NEXUS);

			//duplicate with nexus object below
			Nexus = temp;
			test_map->AddItem(game_map::TILE_TYPE::TILE_NEXUS, test_map->WorldToPreOffsetIndex(hoverTopLeftPos), hoverStructure->gridScale.x, hoverStructure->gridScale.y);
		}
		else if (hoverStructure->tex == playerTex)
		{
			player->active = true;
			temp = player;
			test_map->AddItem(game_map::TILE_TYPE::TILE_PLANET, test_map->WorldToPreOffsetIndex(hoverTopLeftPos), hoverStructure->gridScale.x, hoverStructure->gridScale.y);
		}

		temp->position = hoverStructure->position;
		temp->rotation = hoverStructure->rotation;
		temp->scale = hoverStructure->scale;
		temp->tex = hoverStructure->tex;
		temp->gridScale = hoverStructure->gridScale;

		if (temp->type == GameObject::GO_NEXUS)
		{
			nexusPlaced = true;
			nexusButton->texID = UI::TEX_NEXUS_PLACED;
			nexusButton->hoverText = &textTable->buildNexusPlacedHoverText;
			PlaceWallButton(nullptr);
		}
		else if (temp->type == GameObject::GO_PLAYER)
		{
			playerPlaced = true;
			playerButton->texID = UI::TEX_PLAYER_PLACED;
			playerButton->hoverText = &textTable->buildPlayerPlacedHoverText;
			PlaceWallButton(nullptr);
		}
	}

	void EraseBuiltStructure()
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
					if (gameObj->type == GameObject::GO_DANGER_SIGN)
						return;
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
						nexusButton->hoverText = &textTable->buildNexusHoverText;

						Nexus = nullptr;
					}
					else if (gameObj->type == GameObject::GO_PLAYER)
					{
						playerPlaced = false;
						playerButton->texID = UI::TEX_PLAYER;
						playerButton->hoverText = &textTable->buildPlayerHoverText;
					}

					break;
				}
			}
		}
		test_map->RemoveItem(test_map->WorldToPreOffsetIndex(topRightPos), deleteGridScale.x, deleteGridScale.y);
	}

	void UpdateHoverStructure()
	{
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
		if (test_map->IsOccupied(test_map->WorldToPreOffsetIndex(mouse_pos), hoverStructure->gridScale.x, hoverStructure->gridScale.y))
		{
			hoverStructure->color.Set(1.0f, 0.2f, 0.2f);
			validPlacement = false;
		}
		else if (hoverStructure->active)
		{
			hoverStructure->color.Set(1.0f, 1.0f, 1.0f);
			validPlacement = true;
		}
	}

	void SetPlayerGoal()
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

	void PlayerReachGoalCheck()
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

	void SpawnEnemies()
	{
		enemySpawnTimer += AEFrameRateControllerGetFrameTime();

		if (enemiesSpawned < enemiesToSpawn)
		{
			if (enemySpawnTimer > enemySpawnRate)
			{
				GameObject* temp = FetchGO(GameObject::GO_ENEMY);

				switch (rand() % 4)
				{
				case 0:
					temp->position = test_map->GetWorldPos(test_map->GetIndex(rand() % test_map->width + test_map->tile_offset, 0));
					break;
				case 1:
					temp->position = test_map->GetWorldPos(test_map->GetIndex(rand() % test_map->width + test_map->tile_offset, test_map->height - 1));
					break;
				case 2:
					temp->position = test_map->GetWorldPos(test_map->GetIndex(rand() % test_map->height + test_map->tile_offset, 0));
					break;
				case 3:
					temp->position = test_map->GetWorldPos(test_map->GetIndex(rand() % test_map->height + test_map->tile_offset, test_map->width - 1));
					break;
				}

				temp->scale.x = test_map->GetTileSize();
				temp->scale.y = test_map->GetTileSize();
				temp->tex = enemyTex;
				temp->active = true;
				temp->Stats.path_timer = 5.0f;

				if (rand() % 2)
				{
					temp->Stats.target = Nexus->position;
					temp->Stats.target_type = CharacterStats::TARGET_TYPE::TAR_NEXUS;
				}
				else
				{
					temp->Stats.target = player->position;
					temp->Stats.target_type = CharacterStats::TARGET_TYPE::TAR_PLAYER;
				}

				++enemiesRemaining;
				++enemiesSpawned;
				enemySpawnTimer = 0.0f;
				std::cout << enemiesSpawned << "/" << enemiesToSpawn << std::endl;
			}
		}
	}

	void NextWaveCheck()
	{
		//std::cout << "Remain: " << enemiesRemaining << std::endl;
		if (enemiesRemaining == 0 && enemiesSpawned == enemiesToSpawn)
		{
			enemiesToSpawn += 5;
			if(enemySpawnRate >= 0.1f)
				enemySpawnRate -= 0.05f;
			currentWave++;
			enemiesSpawned = 0;
			buildPhase = true;
			buildResource += 500;
			for (GameObject* tile : go_list)
			{
				if (tile->type == GameObject::GO_TILE)
					tile->tex = grassTex;
			}
			hoverStructure->active = true;
			EnableDangerSigns();
			playerPlaced = false;
			playerButton->texID = UI::TEX_PLAYER;
			playerButton->hoverText = &textTable->buildPlayerHoverText;
			player->active = 0;
			player->Path.clear();
		}
	}

	void TempTestUpdateFunctions()
	{
		if (AEInputCheckTriggered(AEVK_SPACE))
		{
			GameObject* temp = FetchGO(GameObject::GO_BULLET);
			temp->position = player->position;
			temp->active = true;
			temp->scale.x = 10;
			temp->scale.y = 10;
			temp->tex = bulletTex;

			AEVec2Set(&temp->direction, AESinDeg(player->rotation), AECosDeg(player->rotation));
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

	void NexusEnemyUpdate(GameObject* gameObj)
	{
		if (AEVec2Distance(&gameObj->position, &gameObj->Stats.target) <= Nexus->scale.x)
		{
			Nexus->active = false;
		}
	}

	void PlayerEnemyUpdate(GameObject* gameObj)
	{
		gameObj->Stats.target = player->position;
	}

	void UpdateEnemyPath(GameObject* gameObj)
	{
		gameObj->Stats.path_timer += AEFrameRateControllerGetFrameTime();
		if (gameObj->Stats.path_timer >= 2.0f)
		{
			PathManager pathmaker(test_map);
			gameObj->Path = pathmaker.GetPath(AEVec2{ (float)test_map->GetX(test_map->WorldToIndex(gameObj->position)), (float)test_map->GetY(test_map->WorldToIndex(gameObj->position)) }, AEVec2{ (float)test_map->GetX(test_map->WorldToIndex(gameObj->Stats.target)), (float)test_map->GetY(test_map->WorldToIndex(gameObj->Stats.target)) });
			gameObj->Path.erase(gameObj->Path.begin());
			
			if (!gameObj->Path.empty())
			{
				gameObj->Path.erase(gameObj->Path.end() - 1); // remove last 2 check points so we're out of the nexus
				for (auto& pos : gameObj->Path) // converting grid pos to world pos
				{
					pos = test_map->GetWorldPos(test_map->GetIndex(pos.x + test_map->tile_offset, pos.y));
				}
			}

			gameObj->Stats.path_timer = 0.0f;
		}
	}

	void UpdateEnemyRotation(GameObject* gameObj)
	{
		if (gameObj->Stats.target_type == CharacterStats::TARGET_TYPE::TAR_NEXUS)
		{
			AEVec2 result{ 0,0 };
			AEVec2Sub(&result, &Nexus->position, &gameObj->position);
			gameObj->rotation = AERadToDeg(atan2f(result.x, result.y)); // rotate to face player
		}
		else if (gameObj->Stats.target_type == CharacterStats::TARGET_TYPE::TAR_PLAYER)
		{
			AEVec2 result{ 0,0 };
			AEVec2Sub(&result, &player->position, &gameObj->position);
			gameObj->rotation = AERadToDeg(atan2f(result.x, result.y)); // rotate to face player
		}
	}

	void UpdatePlayerPosition(GameObject* gameObj)
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
	}

	AEVec2 FindClosestEnemy(GameObject* gameObj)
	{
		AEVec2 result{ 0,0 };
		f32 smallest_dist{ FLT_MAX };
		for (GameObject* other : go_list)
		{
			if (other->active && other->type == GameObject::GAMEOBJECT_TYPE::GO_ENEMY)
			{
				if (smallest_dist > AEVec2Distance(&gameObj->position, &other->position)) // old distance larger than new distance
				{
					smallest_dist = AEVec2Distance(&gameObj->position, &other->position);
					result = other->position;
				}
			}
		}
		return result;
	}

	void UpdateTurretShooting(AEVec2 target, GameObject* gameObj)
	{
		AEVec2Sub(&target, &gameObj->position, &target);
		gameObj->rotation = AERadToDeg(atan2f(target.x, target.y));

		if (gameObj->Stats.GetStat(STAT_ATTACK_SPEED) >= 2.0f)
		{
			gameObj->Stats.SetStat(STAT_ATTACK_SPEED, 0.0f);

			GameObject* temp = FetchGO(GameObject::GO_BULLET);
			temp->position = gameObj->position;
			temp->active = true;
			temp->scale.x = 10;
			temp->scale.y = 10;
			temp->tex = bulletTex;

			AEVec2Set(&temp->direction, -AESinDeg(gameObj->rotation), -AECosDeg(gameObj->rotation)); // @TODO: ROTATION BANDAGE
			AEVec2Normalize(&temp->direction, &temp->direction);
		}
	}

	void LoadTextures()
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
		dangerTex = AEGfxTextureLoad("Assets/Danger.png");
	}

	void InitializeUIManager()
	{
		f32 screenWidthX = AEGfxGetWinMaxX() - AEGfxGetWinMinX();
		f32 screenHeightY = AEGfxGetWinMaxY() - AEGfxGetWinMinY();
		::gameUiManager = new UI::UI_Manager{};	// New ui manager
		::gameUiManager->SetWinDim(screenWidthX, screenHeightY);
		::skillTreeManager = new UI::UI_Manager{};
		textTable = new UI::UI_TextAreaTable{};
	}

	void InitializeUIButtons()
	{
		f32 screenWidthX = AEGfxGetWinMaxX() - AEGfxGetWinMinX();
		f32 screenWidthY = AEGfxGetWinMaxY() - AEGfxGetWinMinY();

		AEVec2 const endButtonPos{ screenWidthX * .115f, screenWidthY * .2f };
		AEVec2 const endButtonSize{ screenWidthX * .2f, screenWidthY * .15f };
		::gameUiManager->CreateButton(endButtonPos, endButtonSize, UI::END_PHASE_BUTTON, nullptr, EndTurnButton, &textTable->endTurnHoverText);

		AEVec2 const buildButtonStartPos{ screenWidthX * .115f, screenWidthY * .9f };
		AEVec2 const buildButtonSize{ screenWidthY * .12f, screenWidthY * .12f };
		AEVec2 buildButtonPos{ buildButtonStartPos };
		buildButtonPos.x -= buildButtonSize.y * 0.75f;
		nexusButton = ::gameUiManager->CreateButton(buildButtonPos, buildButtonSize, UI::BUILD_NEXUS_BUTTON, nullptr, PlaceNexusButton, &textTable->buildNexusHoverText);
		buildButtonPos.x += buildButtonSize.y * 1.5f;
		playerButton = ::gameUiManager->CreateButton(buildButtonPos, buildButtonSize, UI::BUILD_PLAYER_BUTTON, nullptr, PlacePlayerButton, &textTable->buildPlayerHoverText);
		buildButtonPos.x -= buildButtonSize.y * 0.75f;
		buildButtonPos.y -= buildButtonSize.y * 1.5f;
		::gameUiManager->CreateButton(buildButtonPos, buildButtonSize, UI::BUILD_WALL_BUTTON, nullptr, PlaceWallButton, &textTable->buildWallHoverText);
		buildButtonPos.y -= buildButtonSize.y * 1.5f;
		::gameUiManager->CreateButton(buildButtonPos, buildButtonSize, UI::BUILD_TOWER_BUTTON, nullptr, PlaceTowerButton, &textTable->buildTowerHoverText);
		buildButtonPos.y -= buildButtonSize.y * 1.5f;
		::gameUiManager->CreateButton(buildButtonPos, buildButtonSize, UI::ERASE_BUTTON, nullptr, EraseButton, &textTable->eraseHoverText);

		// Initialize skill tree buttons
	}

	void InitializeUIElements()
	{
		//elementTestText = { .2f, 0.f, "HEALTH:" };
		//AEVec2 healthBarPos{ -35.f, 0.f }, healthBarScale{ 50.f, 50.f };
		//uiManager->CreateUIStat(healthBarPos, healthBarScale, &elementTestText);
		//auto tempPtr = gameUiManager->CreateUIStat(healthBarPos, healthBarScale, &textTable->elementTestText);
		//tempPtr->SetValue(1.f);
		//tempPtr->SetColor(UI::UI_Color{ 1.f, 0.f, 0.f, 1.f });
	}

	void InitializeTestMap()
	{
		test_map = new game_map(20, 20, AEGetWindowWidth(), AEGetWindowHeight(), 8);

		for (int i = 0; i < test_map->width * test_map->height; i++)
		{
			GameObject* temp = FetchGO(GameObject::GO_TILE);
			temp->scale.x = test_map->GetTileSize();
			temp->scale.y = test_map->GetTileSize();
			temp->position = test_map->GetWorldPos(i);
			temp->tex = grassTex;
		}
	}

	void InitialFetchGos()
	{
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

		player = FetchGO(GameObject::GO_PLAYER);
		player->scale.x = test_map->GetTileSize();
		player->scale.y = player->scale.x;
		player->active = false;
		player->tex = playerTex;

		InitializeDangerSigns();
	}

	void InitializeDangerSigns()
	{
		GameObject* temp;
		int indexCount = 0;
		// Top
		for (int i = 0; i < test_map->width; i++)
		{
			temp = FetchGO(GameObject::GO_DANGER_SIGN);
			temp->scale.x = test_map->GetTileSize();
			temp->scale.y = test_map->GetTileSize();
			temp->tex = dangerTex;
			temp->position.x = test_map->GetTileSize() * i + test_map->GetTileSize() / 2.f + test_map->world_offset;
			temp->position.y = test_map->GetTileSize() / 2.f;
			temp->active = true;
			test_map->AddItem(game_map::TILE_TYPE::TILE_PLANET, test_map->WorldToPreOffsetIndex(temp->position));
		}
		// Bottom
		for (int i = 0; i < test_map->width; i++)
		{
			temp = FetchGO(GameObject::GO_DANGER_SIGN);
			temp->scale.x = test_map->GetTileSize();
			temp->scale.y = test_map->GetTileSize();
			temp->tex = dangerTex;
			temp->position.x = test_map->GetTileSize() * i + test_map->GetTileSize() / 2.f + test_map->world_offset;
			temp->position.y = test_map->GetTileSize() * test_map->height - test_map->GetTileSize() / 2.f;
			temp->active = true;
			test_map->AddItem(game_map::TILE_TYPE::TILE_PLANET, test_map->WorldToPreOffsetIndex(temp->position));
		}
		// Left
		for (int i = 1; i < test_map->height - 1; i++)
		{
			temp = FetchGO(GameObject::GO_DANGER_SIGN);
			temp->scale.x = test_map->GetTileSize();
			temp->scale.y = test_map->GetTileSize();
			temp->tex = dangerTex;
			temp->position.y = test_map->GetTileSize() * i + test_map->GetTileSize() / 2.f;
			temp->position.x = test_map->GetTileSize() / 2.f + test_map->world_offset;
			temp->active = true;
			test_map->AddItem(game_map::TILE_TYPE::TILE_PLANET, test_map->WorldToPreOffsetIndex(temp->position));
		}
		// Right
		for (int i = 1; i < test_map->height - 1; i++)
		{
			temp = FetchGO(GameObject::GO_DANGER_SIGN);
			temp->scale.x = test_map->GetTileSize();
			temp->scale.y = test_map->GetTileSize();
			temp->tex = dangerTex;
			temp->position.y = test_map->GetTileSize() * i + test_map->GetTileSize() / 2.f;
			temp->position.x = test_map->GetTileSize() * test_map->width - test_map->GetTileSize() / 2.f + test_map->world_offset;
			temp->active = true;
			test_map->AddItem(game_map::TILE_TYPE::TILE_PLANET, test_map->WorldToPreOffsetIndex(temp->position));
		}

		EnableDangerSigns();
	}

	void InitializeVariables()
	{
		buildPhase = true;
		nexusPlaced = false;
		buildResource = 3000;
		enemiesToSpawn = 10;
		enemySpawnRate = 0.5f;
		enemiesSpawned = 0;
		enemiesRemaining = 0;
		currentWave = 1;
		enemySpawnTimer = 0.f;
	}

#pragma region UI_CALLBACK_DEFINITIONS
	void EndTurnButton(UI::UI_Button*) {
		if (nexusPlaced && playerPlaced)
		{
			buildPhase = false;
			hoverStructure->active = false;
			for (GameObject* tile : go_list)
			{
				if (tile->type == GameObject::GO_TILE)
					tile->tex = grassBorderlessTex;
			}
			Nexus->active = true;

			// Remove player from collision map
			test_map->RemoveItem(test_map->WorldToPreOffsetIndex(player->position));

			DisableDangerSigns();
		}
		else
		{
			//TODO: FEEDBACK IF PLAYER TRIES TO END TURN WITHOUT PLACING NEXUS
		}
	}

	void PlaceNexusButton(UI::UI_Button*)
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

	void PlacePlayerButton(UI::UI_Button*)
	{
		if (!playerPlaced)
		{
			hoverStructure->gridScale = { 1, 1 };
			hoverStructure->scale = { test_map->GetTileSize(), test_map->GetTileSize()};
			hoverStructure->position.x += test_map->GetTileSize() * 0.5f * (hoverStructure->gridScale.x - 1);
			hoverStructure->position.y += test_map->GetTileSize() * 0.5f * (hoverStructure->gridScale.y - 1);
			hoverStructure->tex = playerTex;
		}
	}

	void PlaceTowerButton(UI::UI_Button*)
	{
		hoverStructure->gridScale = { 2, 2 };
		hoverStructure->scale = { test_map->GetTileSize() * 2, test_map->GetTileSize() * 2 };
		hoverStructure->position.x += test_map->GetTileSize() * 0.5f * (hoverStructure->gridScale.x - 1);
		hoverStructure->position.y += test_map->GetTileSize() * 0.5f * (hoverStructure->gridScale.y - 1);
		hoverStructure->tex = turretTex;
	}

	void PlaceWallButton(UI::UI_Button*)
	{
		hoverStructure->gridScale = { 1, 1 };
		hoverStructure->scale = { test_map->GetTileSize() * 1, test_map->GetTileSize() * 1 };
		hoverStructure->position.x += test_map->GetTileSize() * 0.5f * (hoverStructure->gridScale.x - 1);
		hoverStructure->position.y += test_map->GetTileSize() * 0.5f * (hoverStructure->gridScale.y - 1);
		hoverStructure->tex = wallTex;
	}

	void EraseButton(UI::UI_Button*)
	{
		hoverStructure->gridScale = { 1, 1 };
		hoverStructure->scale = { test_map->GetTileSize() * 1 - EPSILON, test_map->GetTileSize() * 1 - EPSILON };
		hoverStructure->position.x += test_map->GetTileSize() * 0.5f * (hoverStructure->gridScale.x - 1);
		hoverStructure->position.y += test_map->GetTileSize() * 0.5f * (hoverStructure->gridScale.y - 1);
		hoverStructure->tex = eraseTex;
	}
#pragma endregion
}
