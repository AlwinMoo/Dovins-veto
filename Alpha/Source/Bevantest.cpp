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

f32 winSizeX, winSizeY;
s8 m_fontId;


namespace
{
	std::vector<GameObject*> go_list;
	game_map* test_map;
	int object_count;
	f32 len_check;

	UI::UI_Manager* uiManager;
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
	int bullet_flag{};
	int shoot_flag{};
	f64 bullet_cooldown {};
	f32 burst_fire_x;
	f32 burst_fire_y;

	//AOE
	int AOE_flag{};
	bool AOE_begincd{ false };
	f64 AOE_cooldown{};

	GameObject* FetchGO(GameObject::GAMEOBJECT_TYPE value)
	{
		for (auto it : go_list)
		{
			GameObject* go = (GameObject*)it;
			if (!go->active)
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
}

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
		uiManager = new UI::UI_Manager();
		uiManager->SetWinDim(screenWidthX, screenHeightY);
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

	//enemy init
	enemy = FetchGO(GameObject::GO_ENEMY);
	enemy->position = test_map->GetWorldPos(25);
	enemy->scale.x = test_map->GetTileSize();
	enemy->scale.y = enemy->scale.x;
	enemy->tex = Enemy;
	enemy->active = true;

	//bullet init
	//for (int i{}; i < MAX_BULLET_INST; ++i)
	//{
	//	GameObject* bullet = FetchGO(GameObject::GO_BULLET);
	//	bullet->scale.x = BULLET_SIZE;
	//	bullet->scale.y = BULLET_SIZE;
	//	bullet->tex = Bullet;
	//	bullet->active = false;
	//}
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
		invert_mouse.y = uiManager->m_winDim.y - mousepos.y;
		uiManager->Update(invert_mouse, AEInputCheckTriggered(AEVK_LBUTTON));
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
	
	//z-muscle to shoot
	if (AEInputCheckTriggered(AEVK_Z) && shoot_flag == 0)
	{
		shoot_flag = 1;
		bullet_cooldown = 0;
		GameObject* bullet = FetchGO(GameObject::GO_BULLET);

		bullet->tex = Bullet;
		shoot_bullet(bullet, player, static_cast<f32>(mouseX), static_cast<f32>(mouseY), bullet_flag);
		if (bullet_flag == upgrade2)
		{
			burst_fire_x = static_cast<f32> (mouseX);
			burst_fire_y = static_cast<f32> (mouseY);
		}
	}

	bullet_cooldown += AEFrameRateControllerGetFrameTime();

	if (shoot_flag == 1)
	{
		if (bullet_flag == upgrade2)
		{
			if (bullet_cooldown == 3 * AEFrameRateControllerGetFrameTime() || bullet_cooldown == 5 * AEFrameRateControllerGetFrameTime())
			{
				GameObject* bullet = FetchGO(GameObject::GO_BULLET);
				bullet->tex = Bullet;
				shoot_bullet(bullet, player, burst_fire_x, burst_fire_y, bullet_flag);
			}
		}
	}

	if (bullet_cooldown >= static_cast<f64> (1))
	{
		shoot_flag = 0;
		bullet_cooldown = 0;
	}
	//end shoot
	
	//x muscle to AOE
	if (AEInputCheckTriggered(AEVK_X) && AOE_flag >= 1 && AOE_begincd == false)
	{
		if (AOE_flag == 1)
		{
			GameObject* AOE = FetchGO(GameObject::GO_AOE);
			AOE->tex = Bullet;
			AOE_move(AOE, static_cast<double> (player->position.x), static_cast<double> (player->position.y));
			AOE_begincd = true;
		}
	}
	
	if (AOE_begincd == true)
	{
		AOE_cooldown += AEFrameRateControllerGetFrameTime();
		if (AOE_cooldown > static_cast<f64> (2.0f))
		{
			AOE_cooldown = 0;
			AOE_begincd = false;
		}
	}
	//end AOE
	// GameObject Update
	if (len_check <= 75) enemy->active = false;

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
				break;
			case (GameObject::GAMEOBJECT_TYPE::GO_BULLET) :
				gameObj->position.x += gameObj->direction.x * BULLET_VEL;
				gameObj->position.y += gameObj->direction.y * BULLET_VEL;
				if (gameObj->position.x > winSizeX || gameObj->position.x < 0 || gameObj->position.y > winSizeY || gameObj->position.y < 0)
				{
					gameObj->active = false;
					std::cout << "bullet destroyed" << std::endl;
				}

			case (GameObject::GAMEOBJECT_TYPE::GO_AOE) :
				if (gameObj->alpha <= 0) gameObj->active = false;
				gameObj->alpha -= 0.05f;
			}
		}
	}


	if(AEInputCheckTriggered(AEVK_Q)) next = GS_QUIT;
	if (AEInputCheckTriggered(AEVK_K)) next = GS_LEVEL3;

	//m muscle for bullet upgrades
	if (AEInputCheckTriggered(AEVK_M))
	{
		if (bullet_flag == 1) bullet_flag = 2;
		else bullet_flag = 1;
	}
	
	//n muscle for AOE upgrades
	if (AEInputCheckTriggered(AEVK_N))
	{
		if (AOE_flag == 0) AOE_flag = 1;
		else if (AOE_flag == 1) AOE_flag = 2;
	}
	if (AEInputCheckTriggered(AEVK_LBUTTON))
	{
		next = GS_LEVEL2;
	}
	//if (AEInputCheckTriggered(AEVK_W)) next = GS_RESTART;
	// Your own update logic goes here
	if (AEInputCheckTriggered(AEVK_RBUTTON))
	{
		if (test_map->IsInGrid(absmousepos))
		{
			playermove = true;
			//player_goal = test_map->SnapCoordinates(mousePos);
			PathManager pathingObj(test_map);
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
				default: break;

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
	uiManager->Draw();

	// IMPORTANT SNIPPET:
	/***********************************************************************/
	s32 cursorX, cursorY;
	AEInputGetCursorPosition(&cursorX, &cursorY);
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
		delete curr;
	}
}

void Bevantest_Unload()
{
	delete uiManager;
	delete test_map;
	AEGfxTextureUnload(Bullet);
	AEGfxTextureUnload(Enemy);
	AEGfxTextureUnload(Player);
	AEGfxTextureUnload(Grass);
	AEGfxTextureUnload(pTex);

}