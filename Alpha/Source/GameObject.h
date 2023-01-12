#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <vector>
#include <AEEngine.h>

class GameObject
{
public:
	enum GAMEOBJECT_TYPE
	{
		GO_NONE = 0,
		GO_PLANET,
		GO_TOTAL,
	};

	GAMEOBJECT_TYPE type;
	bool active;

	AEVec2 position;
	AEVec2 scale;
	double rotation;

	AEGfxTexture* tex;
	AEGfxVertexList* mesh;

	void Update();
	void Render();

	GameObject(GAMEOBJECT_TYPE default_type = GO_NONE);
	~GameObject();
};

#endif // !OBJECT_BASE_H

