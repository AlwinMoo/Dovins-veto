#include "TestScene.h"
#include <iostream>
namespace test_scene
{
	TestScene::TestScene()
	{
	}
	TestScene::~TestScene()
	{
	}

	GameObject* TestScene::FetchGO(GameObject::GAMEOBJECT_TYPE value)
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

	void TestScene::Init()
	{
		srand(time(NULL));

		test_map = new game_map(10, 10, 800, 600);
	}

	void TestScene::Update()
	{
		// Player Input
		s32 mouseX, mouseY;
		AEInputGetCursorPosition(&mouseX, &mouseY);
		if (AEInputCheckTriggered(AEVK_LBUTTON))
		{
			GameObject* test = FetchGO(GameObject::GO_PLANET);
			test->position.x = static_cast<int>(mouseX / test_map->get_tile_size()) * test_map->get_tile_size();
			test->position.y = static_cast<int>(mouseY / test_map->get_tile_size()) * test_map->get_tile_size();
			test->rotation = rand() % 360;
			test->scale.x = test_map->get_tile_size();
			test->scale.y = test->scale.x;
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
	}

	void TestScene::Render()
	{
		s32 cursorX, cursorY;
		s8 text[]  = "TEST";
		AEInputGetCursorPosition(&cursorX, &cursorY);
		f32 fcursorX, fcursorY;

		AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);

		//Gameobjects Render
		for (GameObject* gameObj : go_list)
		{
			if (gameObj->active)
			{
				if(gameObj->type == GameObject::GO_PLANET)
					gameObj->Render();
			}
		}
	}
}