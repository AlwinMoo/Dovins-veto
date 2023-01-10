#include "TestScene.h"

namespace test_scene
{
	TestScene::TestScene()
	{
	}
	TestScene::~TestScene()
	{
	}

	GameObj::GameObject* TestScene::FetchGO(GameObj::GameObject::GAMEOBJECT_TYPE value)
	{
		for (auto it : go_list)
		{
			GameObj::GameObject* go = (GameObj::GameObject*)it;
			if (!go->active)
			{
				go->active = true;
				++object_count;
				return go;
			}
		}
		GameObj::GameObject* go{ new GameObj::GameObject(value) };
		go_list.push_back(go);

		//CODE TO INITIALISE GO SPECIFIC VARIABLES

		return FetchGO(value);

	}
	void TestScene::update()
	{
		GameObj::GameObject* specific_go;

		specific_go = FetchGO(GameObj::GameObject::GO_NONE);
	}
}