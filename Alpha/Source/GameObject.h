#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <vector>
#include <AEEngine.h>

#include "CharacterStats.h"

struct Color
{
	float r;
	float g;
	float b;

	void Set(float r, float g, float b);
};

class GameObject
{
public:
	enum GAMEOBJECT_TYPE
	{
		GO_NONE = 0,
		GO_PLAYER,
		GO_PLANET,
		GO_GRASS,
		GO_HOVER_STRUCTURE,
		GO_TILE,
		GO_TURRET,
		GO_WALL,
		GO_NEXUS,
		GO_BULLET,
		GO_AOE,
		GO_ENEMY,
		GO_DANGER_SIGN,
		GO_TOTAL,
	};

	GAMEOBJECT_TYPE type;
	bool active;
	float alpha;
	Color color;

	AEVec2 position;
	AEVec2 scale;
	AEVec2 gridScale;
	AEVec2 direction;
	double rotation;

	std::vector<AEVec2> Path;

	//user only by enemy/turrets
	CharacterStats Stats;
	AEVec2 target_pos;

	AEGfxTexture* tex;
	AEGfxVertexList* mesh;

	void Update();
	void Render();

	GameObject(GAMEOBJECT_TYPE default_type = GO_NONE);
	~GameObject();
};

#endif // !OBJECT_BASE_H

