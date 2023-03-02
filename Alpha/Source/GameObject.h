#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <vector>
#include <AEEngine.h>

#include "Turret/Turret.h"

struct Color
{
	float r;
	float g;
	float b;

	void Set(float r, float g, float b);
};

struct Enemy_GO
{
	AEVec2 target;
	bool at_goal;
	f32 path_timer;

	enum class TARGET_TYPE
	{
		TAR_PLAYER,
		TAR_NEXUS,
		TAR_WALL,

		NUM_TAR
	};
	TARGET_TYPE target_type;
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
	Enemy_GO enemy_stats;

	AEGfxTexture* tex;
	AEGfxVertexList* mesh;

	void Update();
	void Render();

	GameObject(GAMEOBJECT_TYPE default_type = GO_NONE);
	~GameObject();
};

#endif // !OBJECT_BASE_H

