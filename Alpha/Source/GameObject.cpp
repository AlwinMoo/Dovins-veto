#include "GameObject.h"

namespace GameObj
{
	GameObject::GameObject(GAMEOBJECT_TYPE default_type) : type(default_type), health(100.f), active(false)
	{
	}

	GameObject::~GameObject()
	{
	}
}