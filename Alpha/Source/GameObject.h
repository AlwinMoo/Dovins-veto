#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <vector>

namespace GameObj
{
	class GameObject
	{
	public:
		enum GAMEOBJECT_TYPE
		{
			GO_NONE = 0,
			GO_TOTAL,
		};

		GAMEOBJECT_TYPE type;
		bool active;

		float health;

		GameObject(GAMEOBJECT_TYPE default_type = GO_NONE);
		~GameObject();
	};
}
#endif // !OBJECT_BASE_H

