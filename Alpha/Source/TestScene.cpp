#include "TestScene.h"
#include <iostream>
namespace test_scene
{
	TestScene::TestScene()
	{
		AEGfxTextureLoad("PlanetTexture.png");
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
		//std::cout << "Cursor pos: " << x << ", " << y << std::endl;
		
	}
	void TestScene::Render()
	{
		s32 cursorX, cursorY;
		s8 text[]  = "TEST";
		AEInputGetCursorPosition(&cursorX, &cursorY);
		f32 fcursorX, fcursorY;
		//fcursorX = cursorX/AE
		//AEGfxSetBackgroundColor(0.3f, 0.3f, 0.3f);
		AEGfxPrint(m_fontId, text, 0.5f, 0.f, 10.f, 1.f, 0.f, 0.f);
	}
}