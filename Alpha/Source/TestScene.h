#ifndef TEST_SCENE_H
#define TEST_SCENE_H
#include "GameObject.h"
#include "AEEngine.h"
namespace test_scene
{
	class TestScene
	{
	private:
	public:
		s32 m_fontId;
		TestScene();
		~TestScene();

		int object_count;

		std::vector<GameObj::GameObject*> go_list;
		GameObj::GameObject* FetchGO(GameObj::GameObject::GAMEOBJECT_TYPE value);

		void update();
		void Render();
	};
}

#endif // !TEST_SCENE_H
