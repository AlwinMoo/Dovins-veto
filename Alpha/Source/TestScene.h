#ifndef TEST_SCENE_H
#define TEST_SCENE_H
#include "GameObject.h"

namespace test_scene
{
	class TestScene
	{
	public:
		TestScene();
		~TestScene();

		int object_count;

		std::vector<GameObj::GameObject*> go_list;
		GameObj::GameObject* FetchGO(GameObj::GameObject::GAMEOBJECT_TYPE value);

		void update();
	};
}

#endif // !TEST_SCENE_H
