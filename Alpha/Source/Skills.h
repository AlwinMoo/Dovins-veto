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

using skill_func = void (*)(GameObject*, std::vector<GameObject*>&, AEGfxTexture*);
void skills_upgrade_check(GameObject*);
void shoot_bullet(GameObject*, std::vector<GameObject*>&, AEGfxTexture*);
void spreadshot(GameObject*, std::vector<GameObject*>&, AEGfxTexture*);
void car_move(GameObject*, std::vector<GameObject*>&, AEGfxTexture*);
void AOE_move(GameObject*, std::vector<GameObject*>&, AEGfxTexture*);
void player_blink(GameObject*, f32, f32);
int skill_input_check(GameObject*);
void car_shoot_bullet(GameObject*, std::vector<GameObject*>&, AEGfxTexture*);