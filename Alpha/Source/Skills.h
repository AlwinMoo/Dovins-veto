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
	TOTAL_SKILLS
};

using skill_func = void (*)(GameObject*, std::vector<GameObject*>&, AEGfxTexture*);
void skills_upgrade_check(GameObject*);
void shoot_bullet(GameObject*, std::vector<GameObject*>&, AEGfxTexture*);
void AOE_move(GameObject*, std::vector<GameObject*>&, AEGfxTexture*);
void player_blink(GameObject*, f32, f32);
int skill_input_check(GameObject*);