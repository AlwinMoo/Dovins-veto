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

const float CAR_VEL{ 5.0f };
const int MAX_SPREAD{ 4};

enum which_skill
{
	shooting = 0,
	AOEing,
	car,
	TOTAL_SKILLS
};

enum upgrade_level : u32
{
	base  = 0b0000'0000,
	tier1 = 0b0000'0001,
	tier2 = 0b0000'0010
};

using skill_func = void (*)(GameObject*, GameObject*);
void skills_upgrade_check(GameObject*);
void skills_update(GameObject*, GameObject*);

void shoot_bullet(GameObject*, GameObject*);
void spreadshot(GameObject*, GameObject*, int);
void car_move(GameObject*, GameObject*);
void random_shoot(GameObject*, GameObject*);

void AOE_move(GameObject*, GameObject*);
void player_blink(GameObject*, f32, f32);

int skill_input_check(GameObject*);
