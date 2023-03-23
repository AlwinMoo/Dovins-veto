#include "Bevantest.h"
#include "Skills.h"
#include "AEEngine.h"
#include <iostream>
#include <iomanip>
#include "UI_Manager.h"
#include <vector>
#include "Map/Map.h"
#include "GameObject.h"
#include "GameStateManager.h"
#include "GameStateList.h"
#include <iostream>
#include "Pathfinding/pathfinder.h"
#include <cmath>
#include "UI_TextAreaTable.h"
f32 winSizeX, winSizeY;
s8 m_fontId;


namespace
{
	std::vector<GameObject*> go_list;
	game_map* test_map;
	int object_count;
	f32 len_check;

	UI::UI_Manager* gameUIManager;
	UI::UI_TextAreaTable* textTable;
	//textures
	AEGfxTexture* pTex;
	AEGfxTexture* Grass;
	AEGfxTexture* Player;
	AEGfxTexture* Enemy;
	AEGfxTexture* Bullet;
	AEGfxVertexList* pMesh;
	//mouse stuff
	s32 click_pos_x;
	s32 click_pos_y;
	s32 pos_x, pos_y;
	s32 x, y;
	//player
	GameObject* player;
	bool playermove;
	AEVec2 player_goal{ 0,0 };

	//enemy
	GameObject* enemy;

	//bullet
	const float BULLET_VEL{ 20.0f };
	const double my_PI{ 3.14159265359 };
	const int MAX_BULLET_INST{ 50 };
	const float BULLET_SIZE{ 10.0f };
	int bullet_flag{};
	const u32 shoot_flag{ 0b0000'0010 };
	f64 bullet_cooldown{};
	f32 burst_fire_x;
	f32 burst_fire_y;
	//AEGfxVertexList* bull_mesh{nullptr};

	//AOE
	int AOE_flag{};
	const float AOE_SIZE{ 200.0f };
	bool AOE_begincd{ false };
	//bool AOE_active{ false };
	f64 AOE_cooldown{};
	f64 AOE_timer{};
	f32 AOE_posx;
	f32 AOE_posy;
	//car stuff

	//blink
	bool blink_ok{ false };
	f64 blink_cd{};

	//skill stuff
	skill_func skills_array[TOTAL_SKILLS]{ shoot_bullet, AOE_move, car_move , taunt_move};
	int skill_input{};


	

	GameObject* FetchGO(GameObject::GAMEOBJECT_TYPE value)
	{
		for (auto it : go_list)
		{
			GameObject* go = (GameObject*)it;
			if (!go->active)
			{
				go->active = true;
				go->alpha = 1.0f;
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
void PlayButton(UI::UI_Button*);

void Bevantest_Load()
{
	pTex = AEGfxTextureLoad("Assets/PlanetTexture.png");
	Grass = AEGfxTextureLoad("Assets/GrassTile.png");
	Player = AEGfxTextureLoad("Assets/PlayerTexture.png");
	Enemy = AEGfxTextureLoad("Assets/EnemyTexture.png");
	Bullet = AEGfxTextureLoad("Assets/YellowTexture.png");

}

void Bevantest_Initialize()
{
	winSizeX = AEGfxGetWinMaxX() - AEGfxGetWinMinX();
	winSizeY = AEGfxGetWinMaxY() - AEGfxGetWinMinY();
	{
		f32 screenWidthX = AEGfxGetWinMaxX() - AEGfxGetWinMinX();
		f32 screenHeightY = AEGfxGetWinMaxY() - AEGfxGetWinMinY();
		//auto meshTest = render::GenerateQuad();
		gameUIManager = new UI::UI_Manager();
		textTable = new UI::UI_TextAreaTable;
		gameUIManager->SetWinDim(screenWidthX, screenHeightY);

		gameUIManager->CreateButton({ 100.f, 100.f }, {100.f, 100.f}, UI::SKILL_TREE_BUTTON,
			&textTable->playButton, PlayButton, &textTable->eraseHoverText);
	}

	test_map = new game_map(10, 10, (float)AEGetWindowWidth(), (float)AEGetWindowHeight(), true); // automatically destroyed in deconstructor


	for (int i = 0; i < test_map->map_size; ++i)
	{
		GameObject* test = FetchGO(GameObject::GO_TILE);
		test->position = test_map->GetWorldPos(i);
		test->scale.x = test_map->GetTileSize();
		test->scale.y = test->scale.x;
		test->tex = Grass;
	}
	//player init
	player = FetchGO(GameObject::GO_PLAYER);
	player->position = test_map->GetWorldPos(11);
	player->scale.x = test_map->GetTileSize();
	player->scale.y = player->scale.x;
	player->tex = Player;
	player->active = true;
	playermove = false;
	player->skill_flag = 0b0000'0000;

	//enemy init
	enemy = FetchGO(GameObject::GO_ENEMY);
	enemy->position = test_map->GetWorldPos(25);
	enemy->scale.x = test_map->GetTileSize();
	enemy->scale.y = enemy->scale.x;
	enemy->tex = Enemy;
	enemy->active = true;

	//instantiate 50 bullets?
	for (int i{}; i < MAX_BULLET_INST; ++i)
	{
		GameObject* bull_inst = FetchGO(GameObject::GO_BULLET);
		bull_inst->tex = Bullet;
		bull_inst->active = false;
	}

	//AOE init
	GameObject* AOE_inst = FetchGO(GameObject::GO_AOE);

	AOE_inst->tex = Bullet;
	AOE_inst->active = false;
}

void Bevantest_Update()
{
	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);

	// Player Input
	AEVec2 mousepos{};
	s32 mouseX, mouseY;
	AEInputGetCursorPosition(&mouseX, &mouseY);
	AEVec2 absmousepos;
	len_check = sqrt(((player->position.x - enemy->position.x) * (player->position.x - enemy->position.x)) + ((player->position.y - enemy->position.y) * (player->position.y - enemy->position.y)));
	AEVec2Set(&absmousepos, mouseX, mouseY);
	{
		AEVec2 invert_mouse = mousepos;
		invert_mouse.y = gameUIManager->m_winDim.y - mousepos.y;
		gameUIManager->Update(invert_mouse, AEInputCheckTriggered(AEVK_LBUTTON));
	}
	mousepos = test_map->SnapCoordinates(mousepos);

	if (AEInputCheckTriggered(AEVK_LBUTTON))
	{
		if (test_map->GetIndex(static_cast<int>(mouseX / test_map->GetTileSize()), static_cast<int>(mouseY / test_map->GetTileSize())) < test_map->map_size && test_map->GetIndex(static_cast<int>(mouseX / test_map->GetTileSize()), static_cast<int>(mouseY / test_map->GetTileSize())) >= 0)
		{
			std::cout << "Legal: " << test_map->GetIndex(static_cast<int>(mouseX / test_map->GetTileSize()), static_cast<int>(mouseY / test_map->GetTileSize())) << std::endl;
		}
		else
		{
			std::cout << "Illegal" << std::endl;
		}
	}
	
	GameObject* player_clone = FetchGO(GameObject::GAMEOBJECT_TYPE::GO_CLONE);
	player_clone->tex = Player;
	afterimage(player_clone, player);

	skills_upgrade_check(player);
	skill_input = skill_input_check(player);



	if (skill_input >= 0)
	{
		skill_func to_exec{ skills_array[skill_input] };

		switch (skill_input)
		{
			GameObject* skill_inst;
		case (shooting):
			skill_inst = FetchGO(GameObject::GAMEOBJECT_TYPE::GO_BULLET);
			skill_inst->tex = Bullet;
			to_exec(player, skill_inst);
			break;
		case(AOEing):
			skill_inst = FetchGO(GameObject::GAMEOBJECT_TYPE::GO_AOE);
			skill_inst->tex = Bullet;
			to_exec(player, skill_inst);
			break;
		case(car):
			skill_inst = FetchGO(GameObject::GAMEOBJECT_TYPE::GO_CAR);
			to_exec(player, skill_inst);
			break;
		case(taunt):
			for (int i{}; i < 5; ++i) // taunt nearest five
			{
				for (GameObject* gameobj : go_list)
				{
					if (!gameobj->active || (gameobj->target == player)) continue;

					to_exec(player, gameobj);
					break;
				}
			}
			break;
		default:
			break;
		}
	}


	//L-shift muscle to blink
	if (AEInputCheckTriggered(AEVK_LSHIFT) && !blink_ok) //cant blink while blinking
	{
		player_blink(player, mouseX, mouseY);
		blink_ok = true;
	}

	// GameObject Update
	if (len_check <= 75) enemy->active = false;

	if (player->Range.active)
	{
		GameObject* skill_inst = FetchGO(GameObject::GAMEOBJECT_TYPE::GO_BULLET);
		for (GameObject* go : go_list)
		{
			if (go->active && go->type == GameObject::GAMEOBJECT_TYPE::GO_CAR)
			{
				skill_inst->tex = Bullet;
				random_shoot(go, skill_inst);
				break;
			}
		}
	}
	for (GameObject* gameObj : go_list)
	{
		if (gameObj->active)
		{
			switch (gameObj->type)
			{
			case (GameObject::GAMEOBJECT_TYPE::GO_PLAYER):
				if (playermove && !player->Path.empty())
				{
					AEVec2 out{};
					AEVec2 norm{};
					AEVec2Set(&norm, (player_goal.x - player->position.x), (player_goal.y - player->position.y));
					AEVec2Normalize(&out, &norm);

					player->position.x += out.x * AEFrameRateControllerGetFrameTime() * 600;
					player->position.y += out.y * AEFrameRateControllerGetFrameTime() * 600;
				}
				if (blink_ok)
				{
					blink_cd += AEFrameRateControllerGetFrameTime();
					if (blink_cd >= static_cast<f64> (5.0))
					{
						blink_ok = false;
						blink_cd = 0;
					}
				}

				break;
			case (GameObject::GAMEOBJECT_TYPE::GO_BULLET):
				gameObj->position.x += gameObj->direction.x * BULLET_VEL;
				gameObj->position.y += gameObj->direction.y * BULLET_VEL;

				if (gameObj->position.x > winSizeX || gameObj->position.x < 0 || gameObj->position.y > winSizeY || gameObj->position.y < 0)
				{
					gameObj->active = false;
					std::cout << "bullet destoryed";
				}

				for (GameObject* go : go_list)
				{
					if (go->active && go->type == GameObject::GAMEOBJECT_TYPE::GO_ENEMY)
					{
						if (AEVec2Distance(&gameObj->position, &go->position) <= go->scale.x * 0.5)
						{
							go->active = false;
							gameObj->active = false;

							if ((gameObj->Range.skill_bit & tier1) == tier1)
							{
								//implement spread shot function here
								for (int i{}; i < MAX_SPREAD; ++i)
								{
									GameObject* skill_inst = FetchGO(GameObject::GAMEOBJECT_TYPE::GO_BULLET);
									skill_inst->tex = Bullet;
									spreadshot(gameObj, skill_inst, i);
								}
							}
						}
					}
				}
				break;

			case (GameObject::GAMEOBJECT_TYPE::GO_AOE):
				//update positions
				gameObj->position.x = player->position.x;
				gameObj->position.y = player->position.y;
				player->AOE.timer += AEFrameRateControllerGetFrameTime();

				//check collision
				for (GameObject* go : go_list)
				{
					if (go->active && go->type == GameObject::GAMEOBJECT_TYPE::GO_ENEMY)
					{
						if (AEVec2Distance(&gameObj->position, &go->position) <= (gameObj->scale.x * 0.5 + go->scale.x * 0.5)) go->active = false;
						break;
					}
				}

				if (player->AOE.timer > static_cast<f64> (0.2f))
				{
					gameObj->alpha -= 0.10f;
					player->AOE.timer = 0;
				}

				if (gameObj->alpha < 0)
				{
					gameObj->active = false;
				}
				break;

			case (GameObject::GAMEOBJECT_TYPE::GO_CAR) : 
			{
				gameObj->position.x += gameObj->direction.x * CAR_VEL;
				gameObj->position.y += gameObj->direction.y * CAR_VEL;

				if (gameObj->position.x > winSizeX || gameObj->position.x < 0 || gameObj->position.y > winSizeY || gameObj->position.y < 0)
				{
					gameObj->active = false;
					player->Range.active = false;
					std::cout << "car destroyed";
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
			default :
				break;
			}
		}
	}


	if(AEInputCheckTriggered(AEVK_Q)) next = GS_QUIT;
	if (AEInputCheckTriggered(AEVK_K)) next = GS_LEVEL3;



	if (AEInputCheckTriggered(AEVK_LBUTTON))
	{
		next = GS_LEVEL3;
	}
	//if (AEInputCheckTriggered(AEVK_W)) next = GS_RESTART;
	// Your own update logic goes here
	if (AEInputCheckTriggered(AEVK_RBUTTON))
	{
		if (test_map->IsInGrid(absmousepos))
		{
			playermove = true;
			//player_goal = test_map->SnapCoordinates(mousePos);
			PathManager pathingObj(test_map, true);
			player->Path = pathingObj.GetPath(AEVec2{ (float)test_map->GetX(test_map->WorldToIndex(player->position)), (float)test_map->GetY(test_map->WorldToIndex(player->position)) }, AEVec2{ (float)test_map->GetX(test_map->WorldToIndex(absmousepos)), (float)test_map->GetY(test_map->WorldToIndex(absmousepos)) });

			if (!player->Path.empty())
			{
				for (auto& pos : player->Path)
				{
					pos = test_map->GetWorldPos(test_map->GetIndex(pos.x + test_map->tile_offset, pos.y));
				}

				player->Path.erase(player->Path.end() - 1);
				player->Path.push_back(absmousepos);
				player_goal = player->Path.front();
			}
			else
			{
				AEVec2 leng{};
				leng.x = player->position.x - absmousepos.x;
				leng.y = player->position.y - absmousepos.y;

				if (AEVec2Length(&leng) <= 5)
				{
					player->Path.push_back(absmousepos);
					player_goal = player->Path.front();
				}
			}
		}
	}

	if (playermove && !player->Path.empty())
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
				playermove = false;
			}
			else
			{
				player_goal = player->Path.front();
			}
		}
	}

	AEVec2 invert_mouse = {static_cast<f32>(mouseX), static_cast<f32>(mouseY)}; // Getting inverted mouse pos to match world space
	invert_mouse.y = gameUIManager->m_winDim.y - mouseY;
	gameUIManager->Update(invert_mouse, AEInputCheckTriggered(AEVK_LBUTTON));
	//AEVec2Set(&absMousePos, static_cast<f32>(mouseX), static_cast<f32>(mouseY));
	//AEVec2Set(&mouse_pos, static_cast<f32>(mouseX), static_cast<f32>(mouseY));
	//AEVec2 invert_mouse = mouse_pos; // Getting inverted mouse pos to match world space
	//invert_mouse.y = uiManagers[UI::UI_TYPE_GAME]->m_winDim.y - mouse_pos.y;
	//gameUIManager.Update(invert_mouse, AEInputCheckTriggered(AEVK_LBUTTON));
}

void Bevantest_Draw()
{
	//Draw mesh

	//Gameobjects Render
	for (GameObject* gameObj : go_list)
	{
		if (gameObj->active)
		{
			switch (gameObj->type)
			{
				case GameObject::GO_TILE	:
												gameObj->Render();
												break;
				case  GameObject::GO_PLAYER :
												gameObj->Render();
												break;
				case GameObject:: GO_ENEMY	:
												gameObj->Render();
												break;
				case GameObject:: GO_BULLET :
												gameObj->Render();
												break;
				case GameObject:: GO_AOE	:
												gameObj->Render();
												break;
				case GameObject:: GO_CAR	:
												gameObj->Render();
												break;
				case GameObject:: GO_CLONE  :
												gameObj->Render();
												break;
				default						: 
												break;

			}
		}
	}
	//draw player
	//if (player->active)
	//{
	//	player->Render();
	//}

	//if (enemy->active)
	//{
	//	enemy->Render();
	//}

	// IMPORTANT SNIPPET:
	/***********************************************************************/
	s32 cursorX, cursorY;
	AEInputGetCursorPosition(&cursorX, &cursorY);
	gameUIManager->Draw(cursorX, cursorY);
	f32 winX{ AEGfxGetWinMaxX() - AEGfxGetWinMinX() }, winY{ AEGfxGetWinMaxY() - AEGfxGetWinMinY() };
	//std::cout << winX << ", " <<  winY << std::endl;
	f32 cursorXN{ cursorX / winX * 2 - 1.f }, cursorYN{ cursorY / winY * -2 + 1.f }; // NORMALIZED COORDINATES
	//std::cout << std::setprecision(2) << std::setw(5) << cursorXN << ", " << std::setw(5) << cursorYN << std::endl;
	/************************************************************************/
	s8 const* testStr = "CLICK TO START";
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxPrint(1, (s8*)testStr, 0.5f, 0.f, 2.f, 1.f, 0.f, 0.f);
}

void Bevantest_Free()
{
	//AEGfxMeshFree(pMesh);
	for (GameObject* curr : go_list) 
	{
		//if (curr->type == GameObject::GO_BULLET || curr->type == GameObject::GO_AOE) curr->mesh = nullptr;
		delete curr;
	}

}

void Bevantest_Unload()
{
	//AEGfxMeshFree(bull_mesh);
	delete gameUIManager;
	delete textTable;
	delete test_map;
	AEGfxTextureUnload(Bullet);
	AEGfxTextureUnload(Enemy);
	AEGfxTextureUnload(Player);
	AEGfxTextureUnload(Grass);
	AEGfxTextureUnload(pTex);

}

void PlayButton(UI::UI_Button*)
{
	next = GS_LEVEL3;
}
