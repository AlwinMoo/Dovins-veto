#ifndef TEST_SCENE_H
#define TEST_SCENE_H
#include "GameObject.h"
#include "AEEngine.h"
#include "Map/Map.h"

//namespace test_scene
//{
//	class TestScene
//	{
//	private:
//		game_map *test_map;
//	public:
//		s32 m_fontId;
//		TestScene();
//		~TestScene();
//
//		int object_count;
//
//		std::vector<GameObject*> go_list;
//		GameObject* FetchGO(GameObject::GAMEOBJECT_TYPE value);
//
//		void Init();
//		void Update();
//		void Render();
//	};
//}

void TestScene_Load();

void TestScene_Initialize();

void TestScene_Update();

void TestScene_Draw();

void TestScene_Free();

void TestScene_Unload();

#endif // !TEST_SCENE_H
