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
struct skill_necessity
{
	f64 cooldown;
	f32 damage;
	int on_cd;
	bool active;
};

struct skill_stuff
{
	skill_necessity first_tier;
	skill_necessity second_tier;
	skill_necessity third_tier;
	u32 skill_bit;
	f64 timer;
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
		GO_CAR,
		GO_CLONE,
		GO_ENEMY,
		GO_DANGER_SIGN,
		GO_TOTAL,
	};



	GAMEOBJECT_TYPE type;
	bool active;
	float alpha;
	Color color;
	f64	  timer;
	u32	  skill_flag;

	skill_stuff Utility;
	skill_stuff Range;
	skill_stuff Melee;

	AEVec2 position;
	AEVec2 scale;
	AEVec2 gridScale;
	AEVec2 direction;
	double rotation;

	std::vector<int> gridIndex;

	std::vector<AEVec2> Path;

	GameObject* target;
	GameObject* smallTarget;

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

