#pragma once
#include "Bevantest.h"
#include "Skills.h"
#include "AEEngine.h"
#include <iostream>
#include <iomanip>
#include "UI_Manager.h"
#include <vector>
#include "Map/Map.h"
#include "GameObject.h"
#include "GameStateManager.h"
#include "GameStateList.h"
#include <iostream>
#include "Pathfinding/pathfinder.h"
#include <cmath>

namespace skill_vals
{
	const float AOE_VEL{ 1000.f };
	const float CAR_VEL{ 750.0f };
	const float BULLET_VEL{ 1750.0f };
	const int MAX_SPREAD{ 4 };

	//Range stuff
	const float		BULLET_SIZE{ 10.0f };
	const float		CAR_SIZE{ 40.0f };

	//Melee stuff
	const float AOE_SIZE{ 200.0f };
	const float AOE_ALPHA{ 0.5f };

	//Utility stuff
	const float BLINK_RANGE{ 250.f };

	//afterimage
	const float CLONE_ALPHA{ 0.25f };
}

enum which_skill
{
	shooting = 0,
	AOEing,
	car,
	taunt,
	blink,
	TOTAL_SKILLS
};

enum upgrade_level : u32
{
	base  = 0b0000'0001,
	tier1 = 0b0000'0010,
	tier2 = 0b0000'0100
};

//overall skill funcs
using skill_func = void (*)(GameObject*, GameObject*);
void skills_upgrade_check(GameObject*);
void skills_update(GameObject*, GameObject*);
int skill_input_check(GameObject*);

//range functions
void shoot_bullet(GameObject*, GameObject*);
void spreadshot(GameObject*, GameObject*, int);
void car_move(GameObject*, GameObject*);
void random_shoot(GameObject*, GameObject*);

//melee functions
void AOE_move(GameObject*, GameObject*);
void AOE_ready(GameObject*, GameObject*);

//utility
void taunt_move(GameObject*, GameObject*);
void player_blink(GameObject*);

//praticles
void afterimage(GameObject*, GameObject*);
