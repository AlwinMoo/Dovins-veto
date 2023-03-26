#include "Alwintest.h"
#include <iostream>
#include "Rendering.hpp"
#include "GameStateList.h"
#include "GameStateManager.h"
#include "Pathfinding/pathfinder.h"
#include "CharacterStats.h"
#include "Skills.h"
namespace
{


	std::vector<GameObject*> go_list;
	game_map* test_map;
	int object_count;
	GameObject* hoverStructure;
	bool validPlacement;

	// Health bars
	UI::UI_StatElement*	goHealthBar;

	// UI MANAGERS
	UI::UI_TYPE			UICurrLayer;	// Current layer to update & render
	UI::UI_Manager*		uiManagers[UI::NUM_UI_TYPE]; // Array of pointers to UI managers

	UI::UI_Manager*		gameUiManager;
	UI::UI_Manager*		skillTreeManager;

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
	AEGfxTexture* enemy_nexusTex;
	AEGfxTexture* enemy_tankTex;
	AEGfxTexture* dangerTex;
	AEGfxTexture* targetedTex;
	AEGfxVertexList* squareMesh;

	UI::UI_Button* nexusButton;
	UI::UI_Button* playerButton;
	bool nexusPlaced;
	bool playerPlaced;
	bool buildPhase;

	int buildResource;

	static const int WALL_COST = 50;
	static const int TOWER_COST = 500;

	static const int NEXUS_HEALTH = 15;
	static const int WALL_HEALTH = 5;

	const static int ENEMY_BASE_MOVESPEED = 100;

	static const int TANK_HEALTH = 35;
	const static float TANK_ATTACK_SPEED = 0.5;
	const static int TANK_MOVE_SPEED = 65;
	const static int TANK_DAMAGE = 10;

	static const int INFANTRY_HEALTH = 1;
	const static int INFANTRY_DAMAGE = 5;
	const static float INFANTRY_ATTACK_SPEED = 0.5;

	static const int TURRET_HEALTH = 10;
	static const float TURRET_DAMAGE = 5.0f;

	static const int PLAYER_HEALTH = 20;

	//player
	GameObject* player;
	GameObject* Nexus;

	bool player_moving{ false };
	AEVec2 player_goal{ 0, 0 };

	float turret_shoot_timer{};

	// UI variables
	int uiEnemiesCount;

	// Enemies
	int	enemiesToSpawn;
	float enemySpawnRate;
	float enemySpawnTimer;
	int	enemiesSpawned;
	int	enemiesRemaining;
	int currentWave;
	int enemyTankInGame;
	int tank_count;

	//Skills
	int skill_input;
	skill_func skills_array[TOTAL_SKILLS]{ shoot_bullet, AOE_move, car_move, taunt_move};

	const int RangeBase_cost		{ 200 };
	const int RangeTier1_cost		{ 200 };
	const int RangeTier2_cost		{ 300 };
	const int RangeTier3_cost		{ 300 };
	const int RangeTier4_cost		{ 500 };

	const int MeleeBase_cost		{ 200 };
	const int MeleeTier1_cost		{ 200 };
	const int MeleeTier2_cost		{ 300 };
	const int MeleeTier3_cost		{ 300 };
	const int MeleeTier4_cost		{ 500 };
	
	const int UtilityBase_cost		{ 200 };
	const int UtilityTier1_cost		{ 200 };
	// TEXT TEST
	UI::UI_TextAreaTable* textTable;
	
	//Helper Functions
#pragma region Helper functions
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
	void UpdateGOGridIndex(GameObject* gameObj, int index);
	GameObject* IndexToGO(int index);
	void PlaceStructure();
	void EraseBuiltStructure();
	void UpdateHoverStructure();

	void SetPlayerGoal();
	void PlayerReachGoalCheck();
	void SpawnEnemies();
	void NextWaveCheck();
	void TempTestUpdateFunctions();

	void UpdateEnemyState(GameObject* gameObj);
	void UpdatePlayerPosition(GameObject* gameObj);

	void RenderTexture(AEGfxTexture* texture, AEVec2 pos_, AEVec2 scale_, float rotation_);
	
	GameObject* FindClosestGO(GameObject*, GameObject::GAMEOBJECT_TYPE);
	void UpdateTurretShooting(AEVec2 target, GameObject* gameObj);
#pragma endregion

#pragma region UI_CALLBACK_DECLARATIONS
	void EndTurnButton(UI::UI_Button*);
	void PlaceNexusButton(UI::UI_Button*);
	void PlacePlayerButton(UI::UI_Button*);
	void PlaceTowerButton(UI::UI_Button*);
	void PlaceWallButton(UI::UI_Button*);
	void EraseButton(UI::UI_Button*);
	void SkillTreeButton(UI::UI_Button*);

	void MeleeSkillUpgrade_tier0(UI::UI_Button*);
	void MeleeSkillUpgrade_tier1(UI::UI_Button*);
	void MeleeSkillUpgrade_tier2(UI::UI_Button*);
	void MeleeSkillUpgrade_tier3(UI::UI_Button*);
	void MeleeSkillUpgrade_tier4(UI::UI_Button*);

	void RangeSkillUpgrade_tier0(UI::UI_Button*);
	void RangeSkillUpgrade_tier1(UI::UI_Button*);
	void RangeSkillUpgrade_tier2(UI::UI_Button*);
	void RangeSkillUpgrade_tier3(UI::UI_Button*);
	void RangeSkillUpgrade_tier4(UI::UI_Button*);

	void UtilitySkillUpgrade_tier0(UI::UI_Button*);
	void UtilitySkillUpgrade_tier1(UI::UI_Button*);
	//void UtilitySkillUpgrade_tier2(UI::UI_Button*);
#pragma endregion
}

void Alwintest_Load()
{
	LoadTextures();
	squareMesh = render::GenerateQuad();
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
	f64 dt = AEFrameRateControllerGetFrameTime();
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
		skills_upgrade_check(player);
		UpdateHoverStructure();
	}
	else
	{
		if (AEInputCheckTriggered(AEVK_RBUTTON) && !test_map->IsOccupied(test_map->WorldToIndex(mouse_pos)))
		{
			if (test_map->IsInGrid(absMousePos))
				SetPlayerGoal();
		}

		if (player_moving && !player->Path.empty())
			PlayerReachGoalCheck();

		TempTestUpdateFunctions();

		GameObject* player_clone = FetchGO(GameObject::GAMEOBJECT_TYPE::GO_CLONE);
		player_clone->tex = playerTex;

		afterimage(player_clone, player);

		SpawnEnemies();
		NextWaveCheck();

		

		//skill stuff
		skill_input = skill_input_check(player);

		if (skill_input >= 0)
		{
			skill_func to_exec{ skills_array[skill_input] };

			switch (skill_input)
			{
				GameObject* skill_inst;
			case (shooting):
				skill_inst = FetchGO(GameObject::GAMEOBJECT_TYPE::GO_BULLET);
				skill_inst->tex = bulletTex;
				to_exec(player, skill_inst);
				break;
			case(AOEing):
				skill_inst = FetchGO(GameObject::GAMEOBJECT_TYPE::GO_AOE);
				skill_inst->tex = bulletTex;
				to_exec(player, skill_inst);
				break;
			case(car):
				skill_inst = FetchGO(GameObject::GAMEOBJECT_TYPE::GO_CAR);
				skill_inst->tex = bulletTex;
				to_exec(player, skill_inst);
				break;
			case(taunt):
				for (int i{}; i < 5; ++i) // taunt nearest five
				{
					for (GameObject* gameobj : go_list)
					{
						if (gameobj->active && (gameobj->target != player) && (gameobj->type == GameObject::GAMEOBJECT_TYPE::GO_ENEMY))
						{
							to_exec(player, gameobj);
							break;
						}
					}
				}
				break;
			case(blink):
				player_blink(player);
				break;
			default:
				break;
			}
		}

		if (player->Range.second_tier.active)
		{
			GameObject* skill_inst = FetchGO(GameObject::GAMEOBJECT_TYPE::GO_BULLET);
			for (GameObject* go : go_list)
			{
				if (go->active && go->type == GameObject::GAMEOBJECT_TYPE::GO_CAR)
				{
					skill_inst->tex = bulletTex;
					random_shoot(go, skill_inst);
					break;
				}
			}
		}

		// GameObject Update
		for (GameObject* gameObj : go_list)
		{
			if (!gameObj->active || gameObj->type == GameObject::GAMEOBJECT_TYPE::GO_TILE)
				continue;

			gameObj->Update();

			switch (gameObj->type)
			{
			case (GameObject::GAMEOBJECT_TYPE::GO_ENEMY):
			{
				if (gameObj->position.x > AEGetWindowWidth() || gameObj->position.x < 0 || gameObj->position.y > AEGetWindowHeight() || gameObj->position.y < 0)
					gameObj->active = false;

				//if (gameObj->Stats.target_type == CharacterStats::TARGET_TYPE::TAR_NEXUS)
				//{
				//	//NexusEnemyUpdate(gameObj);
				//	gameObj->target = Nexus;
				//}
				//else if (gameObj->Stats.target_type == CharacterStats::TARGET_TYPE::TAR_PLAYER)
				//{
				//	gameObj->target = player;
				//}

				UpdateEnemyState(gameObj);

				if (gameObj->smallTarget != nullptr)
				{
					AEVec2 target{ gameObj->smallTarget->position };
					AEVec2Sub(&target, &gameObj->position, &target);
					gameObj->rotation = AERadToDeg(atan2f(target.x, target.y));
				}
				else
				{
					AEVec2 target{ gameObj->target->position };
					AEVec2Sub(&target, &gameObj->position, &target);
					gameObj->rotation = AERadToDeg(atan2f(target.x, target.y));
				}

				//UpdateEnemyPath(gameObj);
				//UpdateEnemyRotation(gameObj);
				//UpdateEnemyPosition(gameObj);

				break;
			}
			case (GameObject::GAMEOBJECT_TYPE::GO_PLAYER):
			{
				UpdatePlayerPosition(gameObj);

				/*for (GameObject* go : go_list)
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
				}*/
				break;
			}
			case (GameObject::GAMEOBJECT_TYPE::GO_TURRET):
			{
				gameObj->Stats.SetStat(STAT_ATTACK_SPEED, gameObj->Stats.GetStat(STAT_ATTACK_SPEED) + AEFrameRateControllerGetFrameTime());

				GameObject* temp = FindClosestGO(gameObj, GameObject::GO_ENEMY);
				if (!temp)
					break;

				AEVec2 result = temp->position;
				UpdateTurretShooting(result, gameObj);

				break;
			}
			case (GameObject::GAMEOBJECT_TYPE::GO_BULLET):
			{
				gameObj->position.x += gameObj->direction.x * AEFrameRateControllerGetFrameTime() * skill_vals::BULLET_VEL;
				gameObj->position.y += gameObj->direction.y * AEFrameRateControllerGetFrameTime() * skill_vals::BULLET_VEL;

				if (gameObj->position.x > ((test_map->tile_offset + test_map->width) * test_map->GetTileSize()) || gameObj->position.x < (test_map->tile_offset * test_map->GetTileSize()) || gameObj->position.y > AEGetWindowHeight() || gameObj->position.y < 0)
					gameObj->active = false;

				for (GameObject* go : go_list)
				{
					if (!go->active)
						continue;
					if (go->type == GameObject::GAMEOBJECT_TYPE::GO_ENEMY)
					{
						if (AEVec2Distance(&gameObj->position, &go->position) <= go->scale.x * 0.5)
						{
							std::cout << "Remain: " << enemiesRemaining << std::endl;
							go->Stats.SetStat(STAT_HEALTH, go->Stats.GetStat(STAT_HEALTH) - gameObj->Range.damage);//gameObj->Stats.GetStat(STAT_DAMAGE)); // we just say 1 bullet does 1 damage for now
							gameObj->active = false;

							if ((gameObj->Range.skill_bit & tier2))
							{
								//implement spread shot function here
								for (int i{}; i < skill_vals::MAX_SPREAD; ++i)
								{
									GameObject* skill_inst = FetchGO(GameObject::GAMEOBJECT_TYPE::GO_BULLET);
									skill_inst->tex = bulletTex;
									spreadshot(gameObj, skill_inst, i);
								}
							}

							if (go->Stats.GetStat(STAT_HEALTH) <= 0)
							{
								go->active = false;
								enemiesRemaining--;
								uiEnemiesCount--;
							}
						}
					}
					else if (go->type == GameObject::GAMEOBJECT_TYPE::GO_WALL || go->type == GameObject::GAMEOBJECT_TYPE::GO_TURRET || go->type == GameObject::GAMEOBJECT_TYPE::GO_NEXUS)
					{
						if (AEVec2Distance(&gameObj->position, &go->position) <= go->scale.x * 0.5)
							gameObj->active = false;
					}
				}

				break;
			}
			case (GameObject::GAMEOBJECT_TYPE::GO_AOE):
			{
				//update positions

				player->Melee.lifetime += AEFrameRateControllerGetFrameTime();

					//check collision
				for (GameObject* go : go_list)
				{
					if (go->active && go->type == GameObject::GAMEOBJECT_TYPE::GO_ENEMY)
					{
						if (AEVec2Distance(&gameObj->position, &go->position) <= (gameObj->scale.x * 0.5 + go->scale.x * 0.5))
						{
							std::cout << "Remain: " << enemiesRemaining << std::endl;
							enemiesRemaining--;
							uiEnemiesCount--;
							go->active = false;
						}
					}
				}

				if (player->Melee.skill_bit & tier4)
				{
					AOE_ready(player, gameObj);
					if (gameObj->skill_flag)
					{
						gameObj->position.x += skill_vals::AOE_VEL * gameObj->direction.x * AEFrameRateControllerGetFrameTime();
						gameObj->position.y += skill_vals::AOE_VEL * gameObj->direction.y * AEFrameRateControllerGetFrameTime();
					}
					else
					{
						gameObj->position.x = player->position.x;
						gameObj->position.y = player->position.y;
					}


					if (gameObj->position.x > AEGetWindowWidth() || gameObj->position.x < 0 || gameObj->position.y > AEGetWindowHeight() || gameObj->position.y < 0)
					{
						gameObj->active = false;
						gameObj->skill_flag = false;
						std::cout << "AOE destroyed";
					}
				}

				else
				{
					gameObj->position.x = player->position.x;
					gameObj->position.y = player->position.y;
					if (player->Melee.lifetime > static_cast<f64> (0.2f))
					{
						gameObj->alpha -= 0.10f;
						player->Melee.lifetime = 0;
					}

					if (gameObj->alpha < 0)
					{
						gameObj->active = false;
					}
				}
				break;


			}
			case (GameObject::GAMEOBJECT_TYPE::GO_CAR):
			{
				gameObj->position.x += gameObj->direction.x * skill_vals::CAR_VEL * AEFrameRateControllerGetFrameTime();
				gameObj->position.y += gameObj->direction.y * skill_vals::CAR_VEL * AEFrameRateControllerGetFrameTime();

				if (gameObj->position.x > ((test_map->tile_offset + test_map->width) * test_map->GetTileSize()) || gameObj->position.x < (test_map->tile_offset * test_map->GetTileSize()) || gameObj->position.y > AEGetWindowHeight() || gameObj->position.y < 0)
				{
					gameObj->active = false;
					player->Range.second_tier.active = false;
					//std::cout << "car destroyed";
				}
				break;
			}
			case(GameObject::GAMEOBJECT_TYPE::GO_CLONE):
			{
				gameObj->timer += AEFrameRateControllerGetFrameTime();

				if (gameObj->timer > 0.2)
				{
					gameObj->timer = 0.0;
					gameObj->alpha -= 0.25f;
				}

				if (gameObj->alpha < 0.f)
				{
					gameObj->active = false;
				}
				break;
			}
			}

		}
	}

	if (player->Range.second_tier.active)
	{
		GameObject* skill_inst = FetchGO(GameObject::GAMEOBJECT_TYPE::GO_BULLET);
		for (GameObject* go : go_list)
		{
			if (go->active && go->type == GameObject::GAMEOBJECT_TYPE::GO_CAR)
			{
				skill_inst->tex = bulletTex;
				random_shoot(go, skill_inst);
				break;
			}
		}
	}


	// end skill stuff
	// 
	// Quit Game
	if (AEInputCheckTriggered(AEVK_Q))
	{
		next = GS_QUIT;
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
		if (!gameObj->active)
			continue;

		gameObj->Render();

		//
		if (gameObj->type == GameObject::GO_ENEMY && gameObj->Stats.GetCurrState() == STATE::STATE_ENEMY_ATTACK)
			RenderTexture(targetedTex, gameObj->smallTarget->position, gameObj->smallTarget->scale, gameObj->smallTarget->rotation);
	}

	if (!buildPhase)
	for (GameObject* gameObj : go_list)
	{
		//Gameobjects Render
		if (!gameObj->active)
			continue;

		goHealthBar->SetValue(gameObj->Stats.GetNormalisedRemaining(STAT_HEALTH));
		if (goHealthBar->GetValue() < 1.f)
		{
			float const yOffset = gameObj->scale.y * 0.5f;
			AEVec2 pos{ gameObj->position.x, -gameObj->position.y };
			pos.x -= uiManagers[UI::UI_TYPE_GAME]->m_winDim.x * 0.5f;
			pos.y += uiManagers[UI::UI_TYPE_GAME]->m_winDim.y * 0.5f - yOffset;
			goHealthBar->SetElementPos(pos);
			//goHealthBar->CalculatePositions(); TODO
			goHealthBar->Draw();
		}
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
		uiManagers[UICurrLayer]->Draw(cursorX, cursorY);

		char buff[30]{};
		sprintf_s(buff, "Resources Left: %d", buildResource);
		AEGfxPrint(1, buff, .65f, .9f, 1.5f, 1.f, 1.f, 0.f);

		sprintf_s(buff, "Current Wave: %d", currentWave);
		AEGfxPrint(1, buff, .65f, .7f, 1.5f, 1.f, 1.f, 0.f);

		sprintf_s(buff, "Enemies Remaining: %d", uiEnemiesCount);
		AEGfxPrint(1, buff, .65f, .5f, 1.5f, 1.f, 1.f, 0.f);
		//std::cout << "Resource Left:" << buildResource << std::endl;
	}
}

void Alwintest_Free()
{
	for (auto i : go_list)
	{
		delete i;
	}
	delete goHealthBar;
	delete textTable;
	delete uiManagers[UI::UI_TYPE_GAME];
	delete uiManagers[UI::UI_TYPE_SKILL];
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
	AEGfxTextureUnload(targetedTex);
	AEGfxTextureUnload(enemy_nexusTex);
	AEGfxTextureUnload(enemy_tankTex);
	AEGfxMeshFree(squareMesh);
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
				test_map->AddItem(game_map::TILE_TYPE::TILE_PLANET, test_map->WorldToIndex(go->position));
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
				test_map->RemoveItem(test_map->WorldToIndex(go->position));
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
		AEVec2 invert_mouse = mouse_pos; // Getting inverted mouse pos to match world space
		invert_mouse.y = uiManagers[UI::UI_TYPE_GAME]->m_winDim.y - mouse_pos.y;
		uiManagers[UICurrLayer]->Update(invert_mouse, AEInputCheckTriggered(AEVK_LBUTTON));
	}

	GameObject* IndexToGO(int index)
	{
		for (auto go : go_list)
		{
			if (!go->active)
				continue;
			for (auto index_ : go->gridIndex)
			{
				if (index_ == index)
					return go;
			}
		}
		return nullptr;
	}

	void UpdateGOGridIndex(GameObject* gameObj, int index)
	{
		int minXIndex = test_map->GetX(index);
		int minYIndex = test_map->GetY(index);
		int maxXIndex = minXIndex + gameObj->gridScale.x - 1;
		int maxYIndex = minYIndex + gameObj->gridScale.y - 1;

		for (int i = minXIndex; i <= maxXIndex; i++)
		{
			for (int j = minYIndex; j <= maxYIndex; j++)
				gameObj->gridIndex.push_back(test_map->GetIndex(i, j));
		}
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

			test_map->AddItem(game_map::TILE_TYPE::TILE_PLANET, test_map->WorldToIndex(hoverTopLeftPos), hoverStructure->gridScale.x, hoverStructure->gridScale.y);
		}
		else if (hoverStructure->tex == turretTex)
		{
			if (buildResource < TOWER_COST)
				return;
			temp = FetchGO(GameObject::GO_TURRET);
			buildResource -= TOWER_COST;

			test_map->AddItem(game_map::TILE_TYPE::TILE_PLANET, test_map->WorldToIndex(hoverTopLeftPos), hoverStructure->gridScale.x, hoverStructure->gridScale.y);
		}
		else if (hoverStructure->tex == nexusTex)
		{
			temp = FetchGO(GameObject::GO_NEXUS);

			//duplicate with nexus object below
			Nexus = temp;

			test_map->AddItem(game_map::TILE_TYPE::TILE_NEXUS, test_map->WorldToIndex(hoverTopLeftPos), hoverStructure->gridScale.x, hoverStructure->gridScale.y);
		}
		else if (hoverStructure->tex == playerTex)
		{
			player->active = true;
			temp = player;

			test_map->AddItem(game_map::TILE_TYPE::TILE_PLANET, test_map->WorldToIndex(hoverTopLeftPos), hoverStructure->gridScale.x, hoverStructure->gridScale.y);
		}

		temp->position = hoverStructure->position;
		temp->rotation = hoverStructure->rotation;
		temp->scale = hoverStructure->scale;
		temp->tex = hoverStructure->tex;
		temp->gridScale = hoverStructure->gridScale;

		UpdateGOGridIndex(temp, test_map->WorldToIndex(hoverTopLeftPos));

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
		GameObject* gameObj = IndexToGO(test_map->WorldToIndex(hoverStructure->position));
		if (!gameObj)
			return;
		gameObj->active = false;
		if (gameObj->type == GameObject::GO_WALL)
			buildResource += WALL_COST;
		else if (gameObj->type == GameObject::GO_TURRET)
			buildResource += TOWER_COST;

		gameObj->gridIndex.clear();
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

		test_map->RemoveItem(test_map->WorldToIndex(topRightPos), deleteGridScale.x, deleteGridScale.y);
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
	}

	void SetPlayerGoal()
	{
		player_moving = true;
		//player_goal = test_map->snap_coordinates(mousePos);
		PathManager pathingObj(test_map, true);
		player->Path = pathingObj.GetPath(AEVec2{ (float)test_map->GetX(test_map->WorldToIndex(player->position)), (float)test_map->GetY(test_map->WorldToIndex(player->position)) }, AEVec2{ (float)test_map->GetX(test_map->WorldToIndex(absMousePos)), (float)test_map->GetY(test_map->WorldToIndex(absMousePos)) });

		if (!player->Path.empty())
		{
			for (auto& pos : player->Path)
			{
				pos = test_map->GetWorldPos(test_map->GetIndex(pos.x, pos.y));
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

	// normalised time
	double easeInOutSine(double t)
	{
		return 0.5 * (1 + sin(PI * (t - 0.5)));
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
					temp->position = test_map->GetWorldPos(test_map->GetIndex(rand() % test_map->width, 0));
					break;
				case 1:
					temp->position = test_map->GetWorldPos(test_map->GetIndex(rand() % test_map->width, test_map->height - 1));
					break;
				case 2:
					temp->position = test_map->GetWorldPos(test_map->GetIndex(rand() % test_map->height, 0));
					break;
				case 3:
					temp->position = test_map->GetWorldPos(test_map->GetIndex(rand() % test_map->height, test_map->width - 1));
					break;
				}

				temp->scale.x = test_map->GetTileSize();
				temp->scale.y = test_map->GetTileSize();
				temp->tex = enemyTex;
				temp->active = true;
				temp->Stats.path_timer = 5.0f;
				temp->Stats.SetNextState(STATE::STATE_ENEMY_MOVE);
				temp->Stats.SetCurrInnerState(INNER_STATE::ISTATE_NONE);
				temp->Stats.SetCurrStateFromNext();
				// deafult values, can be overwritten later
				//temp->Stats.SetRawStat(STAT_HEALTH, ENEMY_HEALTH);
				temp->Stats.SetStat(STAT_MOVE_SPEED, ENEMY_BASE_MOVESPEED);
				temp->target = player;
				temp->Stats.SetDefault(INFANTRY_HEALTH, ENEMY_BASE_MOVESPEED, INFANTRY_ATTACK_SPEED, INFANTRY_DAMAGE);

				if (enemyTankInGame < tank_count)
				{
					temp->target = FindClosestGO(temp, GameObject::GO_TURRET);
					temp->Stats.target_type = CharacterStats::TARGET_TYPE::TAR_TURRET;
					temp->Stats.SetStat(STAT_HEALTH, TANK_HEALTH);
					temp->Stats.SetStat(STAT_MOVE_SPEED, TANK_MOVE_SPEED);
					temp->Stats.SetStat(STAT_ATTACK_SPEED, TANK_ATTACK_SPEED);
					temp->Stats.SetStat(STAT_DAMAGE, TANK_DAMAGE);

					temp->Stats.SetDefault(TANK_HEALTH, TANK_MOVE_SPEED, TANK_ATTACK_SPEED, TANK_DAMAGE);
					temp->tex = enemy_tankTex;

					++enemyTankInGame;
				}
				else
				{
					if (rand() % 2)
					{
						temp->target = Nexus;
						temp->Stats.target_type = CharacterStats::TARGET_TYPE::TAR_NEXUS;
						temp->Stats.SetStat(STAT_HEALTH, INFANTRY_HEALTH);
						temp->Stats.SetStat(STAT_DAMAGE, INFANTRY_DAMAGE);
						temp->Stats.SetStat(STAT_ATTACK_SPEED, INFANTRY_ATTACK_SPEED);

						temp->tex = enemy_nexusTex;
					}
					else
					{
						temp->target = player;
						temp->Stats.target_type = CharacterStats::TARGET_TYPE::TAR_PLAYER;
						temp->Stats.SetStat(STAT_HEALTH, INFANTRY_HEALTH);
						temp->Stats.SetStat(STAT_DAMAGE, INFANTRY_DAMAGE);
						temp->Stats.SetStat(STAT_ATTACK_SPEED, INFANTRY_ATTACK_SPEED);
					}
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
		if ((enemiesRemaining == 0 && enemiesSpawned == enemiesToSpawn) || !player->active || !Nexus->active)
		{
			enemiesToSpawn = static_cast<int>(std::floor(easeInOutSine(static_cast<double>(currentWave + 6) / 20) * 100));
			tank_count = static_cast<int>(std::round(easeInOutSine(static_cast<double>(currentWave + 6) / 20) * 10));
			uiEnemiesCount = enemiesToSpawn;
			for (GameObject* gameObj : go_list)
			{
				//Gameobjects Render
				if (!gameObj->active || gameObj->type != GameObject::GAMEOBJECT_TYPE::GO_ENEMY)
					continue;

				gameObj->Path.clear();
				gameObj->active = false;
			}

			enemiesRemaining = 0;

			if(enemySpawnRate >= 0.1f)
				enemySpawnRate -= 0.05f;
			currentWave++;
			enemiesSpawned = 0;
			enemyTankInGame = 0;
			buildPhase = true;
			buildResource += 500;
			for (GameObject* tile : go_list)
			{
				if (tile->type == GameObject::GO_TILE)
					tile->tex = grassTex;

				if (tile->type == GameObject::GO_BULLET)
					tile->active = false;

				if (tile->type == GameObject::GO_CLONE)
					tile->active = false;
				if (tile->type == GameObject::GO_CAR)
					tile->active = false;
			}
			hoverStructure->active = true;
			EnableDangerSigns();

			playerPlaced = false;
			playerButton->texID = UI::TEX_PLAYER;
			playerButton->hoverText = &textTable->buildPlayerHoverText;
			player->active = 0;
			player->gridIndex.clear();
			player->Path.clear();

			if (!Nexus->active)
			{
				nexusPlaced = false;
				nexusButton->texID = UI::TEX_NEXUS;
				nexusButton->hoverText = &textTable->buildNexusPlacedHoverText;
			}
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

	void UpdateEnemyState(GameObject* gameObj)
	{
		switch (gameObj->Stats.GetCurrState())
		{
			case (STATE::STATE_ENEMY_MOVE):
			{
				switch (gameObj->Stats.GetCurrInnerState())
				{
				case (INNER_STATE::ISTATE_ENTER):
				{
					// init
					// always set next state
					gameObj->Stats.SetCurrInnerState(INNER_STATE::ISTATE_UPDATE);
				}
				break;
				case (INNER_STATE::ISTATE_UPDATE):
				{
					// whack target if near target
					// if far from target, move to target,
					// if target down, choose next target
					gameObj->Stats.path_timer += AEFrameRateControllerGetFrameTime();

					if (gameObj->Stats.path_timer >= 1.0f && gameObj->target != nullptr)
					{
						PathManager pathmaker(test_map, false);
						gameObj->Path = pathmaker.GetPath(AEVec2{ (float)test_map->GetX(test_map->WorldToIndex(gameObj->position)), (float)test_map->GetY(test_map->WorldToIndex(gameObj->position)) }, AEVec2{ (float)test_map->GetX(test_map->WorldToIndex(gameObj->target->position)), (float)test_map->GetY(test_map->WorldToIndex(gameObj->target->position)) });

						if (!gameObj->Path.empty())
						{
							gameObj->Path.erase(gameObj->Path.begin());
							std::vector<AEVec2>::iterator it = gameObj->Path.begin();
							for (auto& pos : gameObj->Path) // converting grid pos to world pos
							{
								if (test_map->map_arr[test_map->GetIndex(pos.x, pos.y)] != game_map::TILE_TYPE::TILE_NONE)
								{
									gameObj->smallTarget = IndexToGO(test_map->GetIndex(pos.x, pos.y));
									if (gameObj->smallTarget != nullptr) break;
								}

								pos = test_map->GetWorldPos(test_map->GetIndex(pos.x, pos.y));
								++it;
							}

							gameObj->Path.erase(it, gameObj->Path.end()); // delete everything from small target onwards

							gameObj->Stats.path_timer = 0.0f;
						}

						// @TODO CHANGE MELEE RANGE
						if (gameObj->smallTarget == nullptr)
							gameObj->smallTarget = gameObj->target;

						if (AEVec2Distance(&gameObj->smallTarget->position, &gameObj->position) <= test_map->GetTileSize() * gameObj->smallTarget->scale.x * 0.5f)
						{
							gameObj->Stats.SetNextState(STATE::STATE_ENEMY_ATTACK);
							gameObj->Stats.SetCurrInnerState(INNER_STATE::ISTATE_EXIT);
						}
					}
					else if (gameObj->target == nullptr)
					{
						gameObj->Stats.target_type = CharacterStats::TARGET_TYPE::TAR_NEXUS;
						gameObj->target = Nexus;
					}
				}
					break;
				case (INNER_STATE::ISTATE_EXIT):
				{
					// clean up
					// always set next state
					// if next state is the same, as curr then we will just run as usual
					gameObj->Stats.SetCurrStateFromNext();
				}
				break;
				
				}
			}
			break;

			case (STATE::STATE_ENEMY_ATTACK):
			{
				switch (gameObj->Stats.GetCurrInnerState())
				{
					case (INNER_STATE::ISTATE_ENTER):
					{
						// init
						// always set next state
						gameObj->Stats.SetCurrInnerState(INNER_STATE::ISTATE_UPDATE);
						std::cout << "attack enter" << std::endl;
					}
					break;
					case (INNER_STATE::ISTATE_UPDATE):
					{
						if (!gameObj->Path.size() && gameObj->smallTarget != nullptr) // we are at our target
						{
							// whack small target
							gameObj->smallTarget->Stats.SetStat(STAT_HEALTH, gameObj->smallTarget->Stats.GetStat(STAT_HEALTH) - gameObj->Stats.GetStat(STAT_DAMAGE) * AEFrameRateControllerGetFrameTime() * gameObj->Stats.GetStat(STAT_ATTACK_SPEED));

							if (gameObj->smallTarget->Stats.GetStat(STAT_HEALTH) <= 0.0f && gameObj->smallTarget->active)
							{
								gameObj->smallTarget->active = false;
								if (gameObj->smallTarget != player)
								{
									test_map->RemoveItem(gameObj->smallTarget->gridIndex.front(), gameObj->smallTarget->gridScale.x, gameObj->smallTarget->gridScale.y);
									gameObj->smallTarget->gridIndex.clear();
								}
							}
						}

						// whack target if near target
						// if far from target, move to target,
						// if target down, choose next target
						if (gameObj->smallTarget->Stats.GetStat(STAT_HEALTH) <= 0.0f || !gameObj->smallTarget->active) // target is down
						{
							switch (gameObj->Stats.target_type)
							{
								case (CharacterStats::TARGET_TYPE::TAR_PLAYER):
									gameObj->target = player;
									break;
								case (CharacterStats::TARGET_TYPE::TAR_NEXUS):
									gameObj->target = Nexus;
									break;
								case (CharacterStats::TARGET_TYPE::TAR_TURRET):
								{
									GameObject* turret{ FindClosestGO(gameObj, GameObject::GO_TURRET) };

									if (!turret)
									{
										gameObj->Stats.target_type = CharacterStats::TARGET_TYPE::TAR_NEXUS;
										gameObj->target = Nexus;
										break;
									}
										
									gameObj->target = turret;
								}
									break;
							}
							
							gameObj->Stats.SetNextState(STATE::STATE_ENEMY_MOVE);
							gameObj->Stats.SetCurrInnerState(INNER_STATE::ISTATE_EXIT);
							break;
						}
					}
					break;
					case (INNER_STATE::ISTATE_EXIT):
					{
						// clean up
				// always set next state
				// if next state is the same, as curr then we will just run as usual
						gameObj->Stats.SetCurrStateFromNext();
					}
					break;
				}
			}
			break;
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

	GameObject* FindClosestGO(GameObject* gameObj, GameObject::GAMEOBJECT_TYPE type)
	{
		GameObject* result{};
		f32 smallest_dist{ FLT_MAX };
		for (GameObject* other : go_list)
		{
			if (other->active && other->type == type)
			{
				if (smallest_dist > AEVec2Distance(&gameObj->position, &other->position)) // old distance larger than new distance
				{
					smallest_dist = AEVec2Distance(&gameObj->position, &other->position);
					result = other;
				}
			}
		}

		return result;
	}

	void UpdateTurretShooting(AEVec2 target, GameObject* gameObj)
	{
		AEVec2Sub(&target, &gameObj->position, &target);
		gameObj->rotation = AERadToDeg(atan2f(target.x, target.y));

		if (gameObj->Stats.GetStat(STAT_ATTACK_SPEED) >= 1.0f)
		{
			gameObj->Stats.SetStat(STAT_ATTACK_SPEED, 0.0f);

			GameObject* temp = FetchGO(GameObject::GO_BULLET);
			temp->active = true;
			temp->scale.x = 10;
			temp->scale.y = 10;
			temp->tex = bulletTex;
			temp->Range.damage = TURRET_DAMAGE;
			AEVec2Set(&temp->direction, -AESinDeg(gameObj->rotation), -AECosDeg(gameObj->rotation));
			AEVec2Normalize(&temp->direction, &temp->direction);

			temp->position.x = gameObj->position.x + (temp->direction.x * (gameObj->scale.x + temp->scale.x) * 0.5f);
			temp->position.y = gameObj->position.y + (temp->direction.y * (gameObj->scale.y + temp->scale.y) * 0.5f);
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
		enemy_nexusTex = AEGfxTextureLoad("Assets/nexus_enemy.png");
		enemy_tankTex = AEGfxTextureLoad("Assets/tank.png");
		eraseTex = AEGfxTextureLoad("Assets/Eraser.png");
		dangerTex = AEGfxTextureLoad("Assets/Danger.png");
		targetedTex = AEGfxTextureLoad("Assets/Targeted.png");
	}

	void RenderTexture(AEGfxTexture* texture, AEVec2 pos_, AEVec2 scale_, float rotation_)
	{
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxTextureSet(texture, 0, 0);

		AEMtx33 scale = { 0 };
		AEMtx33Scale(&scale, scale_.x, scale_.x);

		AEMtx33 rotate = { 0 };
		AEMtx33Rot(&rotate, AEDegToRad(rotation_ + 180.f));

		AEMtx33 translate = { 0 };
		AEMtx33Trans(&translate, pos_.x - AEGetWindowWidth() / 2.f, -pos_.y + AEGetWindowHeight() / 2.f);

		AEMtx33 transform = { 0 };
		AEMtx33Concat(&transform, &rotate, &scale);
		AEMtx33Concat(&transform, &translate, &transform);

		AEGfxSetTransform(transform.m);
		AEGfxMeshDraw(squareMesh, AE_GFX_MDM_TRIANGLES);
	}

	void InitializeUIManager()
	{
		f32 screenWidthX = AEGfxGetWinMaxX() - AEGfxGetWinMinX();
		f32 screenHeightY = AEGfxGetWinMaxY() - AEGfxGetWinMinY();
		uiManagers[UI::UI_TYPE_GAME] = new UI::UI_Manager{};	// New ui manager for gameplay
		uiManagers[UI::UI_TYPE_GAME]->SetWinDim(screenWidthX, screenHeightY);
		uiManagers[UI::UI_TYPE_SKILL] = new UI::UI_Manager{};	// New ui manager for skill tree
		uiManagers[UI::UI_TYPE_SKILL]->SetWinDim(screenWidthX, screenHeightY);
		textTable = new UI::UI_TextAreaTable{};	// Set up all UI button text description info
		UICurrLayer = UI::UI_TYPE_GAME;							// Display gameplay UI first
	}

	void InitializeUIButtons()
	{
		f32 screenWidthX = AEGfxGetWinMaxX() - AEGfxGetWinMinX();
		f32 screenHeightY = AEGfxGetWinMaxY() - AEGfxGetWinMinY();
		AEVec2 const endButtonPos{ screenWidthX * .115f, screenHeightY * .2f };
		AEVec2 const endButtonSize{ screenWidthX * .2f, screenHeightY * .15f };

		UI::UI_Manager& gameUIManager{ *uiManagers[UI::UI_TYPE_GAME] };
		UI::UI_Manager& skillUIManager{ *uiManagers[UI::UI_TYPE_SKILL] };

		uiManagers[UI::UI_TYPE_GAME]->CreateButton(endButtonPos, endButtonSize, UI::END_PHASE_BUTTON, nullptr, EndTurnButton, &textTable->endTurnHoverText);

		AEVec2 const buildButtonStartPos{ screenWidthX * .115f, screenHeightY * .9f };
		AEVec2 const buildButtonSize{ screenHeightY * .12f, screenHeightY * .12f };
		AEVec2 buildButtonPos{ buildButtonStartPos };
		buildButtonPos.x -= buildButtonSize.y * 0.75f;
		nexusButton = gameUIManager.CreateButton(buildButtonPos, buildButtonSize, UI::BUILD_NEXUS_BUTTON, 
			nullptr, PlaceNexusButton, &textTable->buildNexusHoverText);
		buildButtonPos.x += buildButtonSize.y * 1.5f;
		playerButton = gameUIManager.CreateButton(buildButtonPos, buildButtonSize, UI::BUILD_PLAYER_BUTTON,
			nullptr, PlacePlayerButton, &textTable->buildPlayerHoverText);
		buildButtonPos.x -= buildButtonSize.y * 0.75f;
		buildButtonPos.y -= buildButtonSize.y * 1.5f;
		gameUIManager.CreateButton(buildButtonPos, buildButtonSize, UI::BUILD_WALL_BUTTON, 
			nullptr, PlaceWallButton, &textTable->buildWallHoverText);
		buildButtonPos.y -= buildButtonSize.y * 1.5f;
		gameUIManager.CreateButton(buildButtonPos, buildButtonSize, UI::BUILD_TOWER_BUTTON, 
			nullptr, PlaceTowerButton, &textTable->buildTowerHoverText);
		buildButtonPos.y -= buildButtonSize.y * 1.5f;
		gameUIManager.CreateButton(buildButtonPos, buildButtonSize, UI::ERASE_BUTTON, 
			nullptr, EraseButton, &textTable->eraseHoverText);
		
		// Both skill tree and gameplay layers share same button pos
		buildButtonPos.x = screenWidthX - buildButtonSize.x;
		gameUIManager.CreateButton(buildButtonPos, buildButtonSize, UI::SKILL_TREE_BUTTON, 
			nullptr, SkillTreeButton, &textTable->Skill_Tree);
		skillUIManager.CreateButton(buildButtonPos, buildButtonSize, UI::CLOSE_BUTTON,
			nullptr, SkillTreeButton, &textTable->Skill_Tree);

		// Initialize skill tree buttons
		{
			float const tier1XPos{ screenWidthX * .115f };
			AEVec2 skillPos{ tier1XPos, screenHeightY * .7f };
			float tier1YOffset{ screenHeightY * 0.2f };
			float xOffset{ screenWidthX * 0.1f };
			// FIRST SKILL
			{
				// TIER 1
				skillUIManager.CreateButton(skillPos, buildButtonSize, UI::AOE_SKILL_BUTTON,
					nullptr, MeleeSkillUpgrade_tier0, &textTable->MeleeBase);
				skillPos.x += xOffset;
				// TIER 2
				skillUIManager.CreateButton(skillPos, buildButtonSize, UI::AOE_SKILL_BUTTON,
					nullptr, MeleeSkillUpgrade_tier1, &textTable->MeleeTier1);
				skillPos.x += xOffset;
				// TIER 3
				skillUIManager.CreateButton(skillPos, buildButtonSize, UI::AOE_SKILL_BUTTON,
					nullptr, MeleeSkillUpgrade_tier2, &textTable->MeleeTier2);
				skillPos.x += xOffset;
				// TIER 4
				skillUIManager.CreateButton(skillPos, buildButtonSize, UI::AOE_SKILL_BUTTON,
					nullptr, MeleeSkillUpgrade_tier3, &textTable->MeleeTier3);
				skillPos.x += xOffset;
				// TIER 5
				skillUIManager.CreateButton(skillPos, buildButtonSize, UI::AOE_SKILL_BUTTON,
					nullptr, MeleeSkillUpgrade_tier4, &textTable->MeleeTier4);
			}
			skillPos.x = tier1XPos;		// Reset to x pos
			skillPos.y -= tier1YOffset; // Offset y for skill 2
			// SECOND SKILL
			{
				// TIER 1
				skillUIManager.CreateButton(skillPos, buildButtonSize, UI::GUN_SKILL_BUTTON,
					nullptr, RangeSkillUpgrade_tier0, &textTable->RangeBase);
				skillPos.x += xOffset;
				// TIER 2
				skillUIManager.CreateButton(skillPos, buildButtonSize, UI::GUN_SKILL_BUTTON,
					nullptr, RangeSkillUpgrade_tier1, &textTable->RangeTier1);
				skillPos.x += xOffset;
				// TIER 3
				skillUIManager.CreateButton(skillPos, buildButtonSize, UI::GUN_SKILL_BUTTON,
					nullptr, RangeSkillUpgrade_tier2, &textTable->RangeTier2);
				skillPos.x += xOffset;
				// TIER 4
				skillUIManager.CreateButton(skillPos, buildButtonSize, UI::GUN_SKILL_BUTTON,
					nullptr, RangeSkillUpgrade_tier3, &textTable->RangeTier3);
				skillPos.x += xOffset;
				// TIER 5
				skillUIManager.CreateButton(skillPos, buildButtonSize, UI::GUN_SKILL_BUTTON,
					nullptr, RangeSkillUpgrade_tier4, &textTable->RangeTier4);
			}
			skillPos.x = tier1XPos;		// Reset to x pos
			skillPos.y -= tier1YOffset; // Offset y for skill 3
			// THIRD SKILL
			{
				// TIER 1
				skillUIManager.CreateButton(skillPos, buildButtonSize, UI::SKILL_TREE_BUTTON,
					nullptr, UtilitySkillUpgrade_tier0, &textTable->UtiliyBase);
				skillPos.x += xOffset;
				// TIER 2
				skillUIManager.CreateButton(skillPos, buildButtonSize, UI::SKILL_TREE_BUTTON,
					nullptr, UtilitySkillUpgrade_tier1, &textTable->UtilityTier1);
				skillPos.x += xOffset;
				// TIER 3
				//skillUIManager.CreateButton(skillPos, buildButtonSize, UI::SKILL_TREE_BUTTON,
				//	nullptr, UtilitySkillUpgrade_tier2, &textTable->playButton);
			}
		}
			
	}

	void InitializeUIElements()
	{
		AEVec2 healthBarPos{ -35.f, 0.f }, healthBarScale{ 50.f, 5.f };
		UI::UI_Manager& skillUIManager{ *uiManagers[UI::UI_TYPE_SKILL] };
		skillUIManager.CreateUIStat(healthBarPos, healthBarScale, &textTable->elementTestText);
		goHealthBar = skillUIManager.CreateUIStat(healthBarPos, healthBarScale, nullptr);
		goHealthBar->SetValue(1.f);
		goHealthBar->SetColor(UI::UI_Color{ 1.f, 0.f, 0.f, 1.f });
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
		player->Stats.SetStat(STAT_HEALTH, PLAYER_HEALTH);
		player->Stats.SetDefault(PLAYER_HEALTH, 0.0f, 0.0f, 0.0f);

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
			test_map->AddItem(game_map::TILE_TYPE::TILE_PLANET, test_map->WorldToIndex(temp->position));
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
			test_map->AddItem(game_map::TILE_TYPE::TILE_PLANET, test_map->WorldToIndex(temp->position));
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
			test_map->AddItem(game_map::TILE_TYPE::TILE_PLANET, test_map->WorldToIndex(temp->position));
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
			test_map->AddItem(game_map::TILE_TYPE::TILE_PLANET, test_map->WorldToIndex(temp->position));
		}

		EnableDangerSigns();
	}

	void InitializeVariables()
	{
		buildPhase = true;
		nexusPlaced = false;
		buildResource = 3000;
		enemiesToSpawn = 10;
		uiEnemiesCount = enemiesToSpawn;
		tank_count = 2;
		enemySpawnRate = 0.5f;
		enemiesSpawned = 0;
		enemiesRemaining = 0;
		currentWave = 1;
		enemySpawnTimer = 0.f;
		enemyTankInGame = 0;
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
			test_map->RemoveItem(test_map->WorldToIndex(player->position));

			DisableDangerSigns();

			for (auto go : go_list)
			{
				if (!go->active)
					continue;
				if (go->type == GameObject::GO_WALL)
				{
					go->Stats.SetRawStat(STAT_HEALTH, WALL_HEALTH);
					go->Stats.SetDefault(WALL_HEALTH, 0.0f, 0.0f, 0.0f);
				}
				else if (go->type == GameObject::GO_TURRET)
				{
					go->Stats.SetStat(STAT_HEALTH, TURRET_HEALTH);
					go->Stats.SetDefault(TURRET_HEALTH, 0.0f, 0.0f, 0.0f);
				}
				else if (go->type == GameObject::GO_NEXUS)
				{
					go->Stats.SetRawStat(STAT_HEALTH, NEXUS_HEALTH);
					go->Stats.SetDefault(NEXUS_HEALTH, 0.0f, 0.0f, 0.0f);
				}
			}
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

	void SkillTreeButton(UI::UI_Button* button) 
	{
		UICurrLayer = (UICurrLayer == UI::UI_TYPE_SKILL? UI::UI_TYPE_GAME : UI::UI_TYPE_SKILL);
	}

	void MeleeSkillUpgrade_tier0(UI::UI_Button*)
	{
		if ((player->Melee.skill_bit & base) != base && buildResource >= MeleeBase_cost)
		{
			player->Melee.skill_bit |= base;
			player->Melee.first_tier.cooldown = 0.0f;
			player->Melee.timer = 7.5f;
			buildResource -= MeleeBase_cost;
			std::cout << "AOE active\n";
		}
	}

	void MeleeSkillUpgrade_tier1(UI::UI_Button*)
	{
		if (!(player->Melee.skill_bit & tier1) && (player->Melee.skill_bit & base) && buildResource >= MeleeTier1_cost)
		{
			player->Melee.skill_bit |= tier1;
			buildResource -= MeleeTier1_cost;
			std::cout << "Melee tier1 active\n";
		}
	}
	void MeleeSkillUpgrade_tier2(UI::UI_Button*)
	{
		if (!(player->Melee.skill_bit & tier2) && (player->Melee.skill_bit & tier1) && buildResource >= MeleeTier2_cost)
		{
			player->Melee.skill_bit |= tier2;
			player->Melee.timer = 5.f;
			buildResource -= MeleeTier2_cost;
			std::cout << "Melee tier2 active\n";
		}
	}
	void MeleeSkillUpgrade_tier3(UI::UI_Button*)
	{
		if (!(player->Melee.skill_bit & tier3) && (player->Melee.skill_bit & tier2) && buildResource >= MeleeTier3_cost)
		{
			player->Melee.skill_bit |= tier3;
			buildResource -= MeleeTier3_cost;
			std::cout << "Melee tier3 active\n";
		}
	}
	void MeleeSkillUpgrade_tier4(UI::UI_Button*)
	{
		if (!(player->Melee.skill_bit & tier4) && (player->Melee.skill_bit & tier3) && buildResource >= MeleeTier4_cost)
		{
			buildResource -= MeleeTier4_cost;
			player->Melee.skill_bit |= tier4;
			std::cout << "Melee tier4 active\n";
		}
	}

	void RangeSkillUpgrade_tier0(UI::UI_Button*)
	{
		if ((player->Range.skill_bit & base) != base && buildResource >= RangeBase_cost)
		{
			player->Range.skill_bit |= base;
			player->Range.first_tier.cooldown = 0.0f;
			player->Range.timer = 0.2f;
			buildResource -= RangeBase_cost;
			std::cout << "shoot active\n";
		}
	}

	void RangeSkillUpgrade_tier1(UI::UI_Button*)
	{
		if ((player->Range.skill_bit & tier1) != tier1 && (player->Range.skill_bit & base) && buildResource >= RangeTier1_cost)
		{
			buildResource -= RangeTier1_cost;
			player->Range.skill_bit |= tier1;
			std::cout << "Range tier1 active\n";
		}
	}

	void RangeSkillUpgrade_tier2(UI::UI_Button*)
	{
		if ((player->Range.skill_bit & tier2) != tier2 && (player->Range.skill_bit & tier1) && buildResource >= RangeTier2_cost)
		{
			buildResource -= RangeBase_cost;
			player->Range.skill_bit |= tier2;
			std::cout << "Range tier2 active\n";
		}
	}
	void RangeSkillUpgrade_tier3(UI::UI_Button*)
	{
		if ((player->Range.skill_bit & tier3) != tier3 && (player->Range.skill_bit & tier2) && buildResource >= RangeTier3_cost)
		{
			buildResource -= RangeTier3_cost;
			player->Range.skill_bit |= tier3;
			std::cout << "Range tier3 active\n";
		}
	}
	void RangeSkillUpgrade_tier4(UI::UI_Button*)
	{
		if ((player->Range.skill_bit & tier4) != tier4 && (player->Range.skill_bit & tier3) && buildResource >= RangeTier4_cost)
		{
			buildResource -= RangeTier4_cost;
			player->Range.skill_bit |= tier4;
			std::cout << "Range tier4 active\n";
		}
	}

	void UtilitySkillUpgrade_tier0(UI::UI_Button*)
	{
		if ((player->Utility.skill_bit & base) != base && buildResource >= UtilityBase_cost)
		{
			buildResource -= UtilityBase_cost;
			player->Utility.skill_bit |= base;
			player->Utility.first_tier.cooldown = 0;
			std::cout << "blink active\n";
		}
	}

	void UtilitySkillUpgrade_tier1(UI::UI_Button*)
	{
		if ((player->Utility.skill_bit & tier1) != tier1 && buildResource >= UtilityTier1_cost)
		{
			buildResource -= UtilityTier1_cost;
			player->Utility.skill_bit |= tier1;
			player->Utility.second_tier.cooldown = 0;
			std::cout << "taunt active\n";
		}
	}
	//void UtilitySkillUpgrade_tier2(UI::UI_Button*)
	//{
	//	if ((player->Utility.skill_bit & tier2) != tier2)
	//	{
	//		player->Utility.skill_bit |= tier2;
	//		player->Utility.third_tier.cooldown = 0;
	//		std::cout << "blink active\n";
	//	}
	//}
#pragma endregion
}
